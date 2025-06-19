#!/usr/bin/env python

# Copyright (C) 2002-2025 CERN for the benefit of the ATLAS collaboration
import re
import sys

import GridSubmission.logger as logger


try:
    import pyAMI.client
    import pyAMI.atlas.api
except ModuleNotFoundError:
    print("Maybe you didn't do localSetupPyAMI?")
    sys.exit()


# Call this function when the AMI client throws any exceptions.
# Quite likely, this is due to a missing VOMS proxy, so point
# that out to the user and exit with error code.
def exit_with_pyami_exception():
    print("")
    print(logger.FAIL + "is your voms proxy okay?" + logger.ENDC)
    print("Try voms-proxy-init -voms atlas")
    sys.exit(1)


# Exception to be thrown when information cannot be extracted
# from an LDN, e.g. the p-tag cannot be identified.
class ldn_decypher_error(Exception):
    """Raised when information cannot be extracted from an LDN"""

    pass


# Exception to be thrown when the wrong LDN type is fed into a
# function (e.g. data-type LDN into an MC-only function). This
# assumes there are only data and MC as types.
class ldn_wrong_type(Exception):
    """Raised when an LDN of a wrong type is used"""

    pass


# Take a single LDN, extract its dataset number and return it.
def get_dataset_number(ldn):
    if is_data(ldn):
        raise ldn_wrong_type
    regex = re.compile("\.[0-9]{6}\.")
    match = regex.search(ldn)
    if not match:
        raise ldn_decypher_error
    else:
        return match.group(0)[1:-1]


# Take a single LDN, extract its p-tag and return it.
def get_ptag(ldn):
    regex = re.compile("_p[0-9]+")
    match = regex.search(ldn)
    if not match:
        raise ldn_decypher_error
    else:
        return match.group(0)[1:]


# Take a single LDN, extract its r-tag and return it.
def get_rtag(ldn):
    regex = re.compile("_r[0-9]+")
    match = regex.search(ldn)
    if not match:
        raise ldn_decypher_error
    else:
        return match.group(0)[1:-1]


# Return the type of LDN we're looking at (e.g. simul.HITS,
# recon.AOD, DAOD_TOPQ1 or other derivation).
def get_type(ldn):
    match = re.search("\.recon\.AOD\.", ldn)
    if match:
        return match.group(0)[1:-1]
    match = re.search("\.simul.HITS\.", ldn)
    if match:
        return match.group(0)[1:-1]
    match = re.search("\.DAOD_[A-Z]+[0-9]+\.", ldn)
    if match:
        return match.group(0)[1:-1]
    match = re.search("\.DAOD_[A-Z]+[0-9]+[A-Z]+[0-9]+\.", ldn)
    if match:
        return match.group(0)[1:-1]
    match = re.search("\.DAOD_[A-Z]+\.", ldn)
    if match:
        return match.group(0)[1:-1]
    # If none of the above, raise exception.
    raise ldn_decypher_error


# Get the data scope of a data-type LDN (e.g.
# data15_13TeV.periodA or data16_13TeV.AllYear) and return it.
# This function should only be called on data LDNs.
def get_data_scope(ldn):
    if not is_data(ldn):
        raise ldn_wrong_type
    regex = re.compile("data[0-9]+.*?\..*?\.")
    match = regex.search(ldn)
    if not match:
        raise ldn_decypher_error
    else:
        return match.group(0)[:-1]


# Get a list of all derivations registered on AMI for a list of
# LDNs. This removes the p-tags from the LDNs and replaces them
# with wildcards to search on AMI. The function returns a
# dictionary with dataset numbers as keys, and a list of LDNs as
# values. The LDNs are assumed to be TOPQ derivations.
def get_derivations(ldns):
    amiclient = pyAMI.client.Client("atlas")
    regex = re.compile("_p[0-9]+")
    wildcards = [regex.sub("_p%", d) for d in ldns]
    try:
        results = pyAMI.atlas.api.list_datasets(amiclient, patterns=wildcards)
    except pyAMI.exception.Error:
        exit_with_pyami_exception()

    dictionary = dict()

    # Make sure the dictionary has empty lists prepared for all
    # LDNs that were searched for. If MC, use the dataset number
    # as dictionary key, if data, use the data scope.
    for ldn in ldns:
        try:
            dictionary[get_dataset_number(ldn)] = []
        except ldn_wrong_type:
            dictionary[get_data_scope(ldn)] = []

    # Go through the results and add them to appropriate
    # dictionary entries (appending to lists).
    for d in results:
        try:
            dictionary[get_dataset_number(d["ldn"])].append(d["ldn"])
        except ldn_wrong_type:
            dictionary[get_data_scope(d["ldn"])].append(d["ldn"])
    return dictionary


# Check whether an LDN is actually available and valid on AMI.
def is_available(ldn):
    amiclient = pyAMI.client.Client("atlas")
    try:
        results = pyAMI.atlas.api.list_datasets(
            amiclient, patterns=[ldn], fields=["ldn"]
        )
    except pyAMI.exception.Error:
        exit_with_pyami_exception()
    for d in results:
        if d["ldn"] == ldn:
            return True
    return False


# Check whether an LDN refers to data (and not MC).
def is_data(ldn):
    if ldn[0:4] == "data":
        return True
    return False


# From a given list of LDNs, pick the latest one (i.e. extract
# the p-tag and pick the LDN with the highest one). If the p-tag
# cannot be identified, just return None.
def pick_newest_derivation(ldn_list):
    if len(ldn_list) == 0:
        return ""
    try:
        get_ptag_as_int = lambda a: int(get_ptag(a)[1:])  # noqa: E731
        return sorted(ldn_list, key=get_ptag_as_int)[-1]
    except ldn_decypher_error:
        return None


# Print the status of a list of samples on AMI. This takes a list
# of objects of type TopExamples.grid.Sample, wich then contain
# lists of datasets. Set the second argument to "True" will make
# the function stop on any errors.
def check_sample_status(samples, stop_on_error=False):
    for s in samples:
        print(
            "\n"
            + logger.WARNING
            + s.name
            + logger.ENDC
            + " ({0} {1})".format(
                len(s.datasets), "dataset" if len(s.datasets) == 1 else "datasets"
            )
        )
        derivations = get_derivations(s.datasets)
        status_ok = True

        for ldn in s.datasets:
            # First try to understand what type of LDN we're looking
            # at (e.g. recon.AOD or DAOD_xx derivations). If this
            # fails, exit here.
            try:
                dataset_type = get_type(ldn)
            except Exception:
                print(
                    "Could not identify dataset type (e.g. recon.AOD or DAOD_TOPQ1) of LDN: %s"
                    % ldn
                )
                sys.exit(1)

            # Try to guess the name of the dictionary key. If the
            # LDN is of type MC, then the dataset number should be
            # the key, for data it's the data scope.
            try:
                dict_key = get_dataset_number(ldn)
            except ldn_wrong_type:
                dict_key = get_data_scope(ldn)
            latest_ldn = pick_newest_derivation(derivations[dict_key])

            status_msg = ""

            # First output whether the requested LDN exists and
            # has a valid entry in the AMI database.
            if is_available(ldn):
                status_msg = logger.OKGREEN + "found" + logger.ENDC
            else:
                status_msg = logger.FAIL + "not found" + logger.ENDC
                status_ok = False

            # If the dataset is _not_ a derivation, output its type
            # and mark it in red.
            if dataset_type[0:5] != "DAOD_":
                status_msg += (
                    ", " + logger.WARNING + "Type: " + dataset_type + logger.ENDC
                )
                status_ok = False

            # Then output the derivation status: (1) no derivation
            # available at all, (2) derivations available, but the
            # requested p-tag is not the latest, (3) requested p-tag
            # is the latest.
            if not latest_ldn:
                status_ok = False
            elif not ldn == latest_ldn:
                # Users may have mixed different r-tags (MC campaign) in the same dataset,
                # so we first check if the LDN and latest LDN actually belong to the same campaign.
                # If they don't, the current code is not really equipped to deal with it, so we don't check further.
                if not is_data(ldn) and get_rtag(ldn) != get_rtag(latest_ldn):
                    status_msg += ", " + logger.OKGREEN + "latest" + logger.ENDC
                else:
                    try:
                        status_msg += (
                            ", "
                            + logger.WARNING
                            + "latest p-tag: "
                            + get_ptag(latest_ldn)
                            + logger.ENDC
                        )
                        status_ok = False
                    except ldn_decypher_error:
                        print("Could not identify p-tag of LDN: %s" % latest_ldn)
                        sys.exit(1)
            else:
                status_msg += ", " + logger.OKGREEN + "latest" + logger.ENDC

            print(" - %s (%s)" % (ldn, status_msg))

        # Make the function stop if requested and any of the
        # datasets of the previous sample list had problems.
        if stop_on_error and not status_ok:
            print("\nRequested to stop on error. Please check the above samples ... ")
            sys.exit(1)
