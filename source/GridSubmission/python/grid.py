# Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration

# -*- coding: utf-8 -*-
import os
import sys
import distutils.spawn
import GridSubmission.logger as logger
import GridSubmission.analysis as analysis
import ROOT

availableDatasets = {}


class Sample:
    """
    Tiny class wrapping a group of samples into a named group.
    Internally, this contains a simple list.
    E.g. one can have samples grouped by process using this class.
    """
    def __init__(self, name):
        self.name = name
        self.datasets = []

    def shortNameDatasets(self):
        output = []
        for ds in self.datasets:
            name = ds.replace('/', '').split('.')
            output.append(name[1] + '.' + name[-1])
        return output


def Add(name):
    availableDatasets[name] = Sample(name)
    return availableDatasets[name]


def AvailableDatasets():
    return availableDatasets


def Samples(names):
    samples = []
    for n in names:
        # remove whitespaces from concatenated lines
        for ds in range(0,len(availableDatasets[n].datasets)):
            availableDatasets[n].datasets[ds]=availableDatasets[n].datasets[ds].replace(' ','')
        samples.append(availableDatasets[n])
    return samples


def basicInDSNameShortener(*args):
    # don't ask what args[0] is; just use args[1] (sorry for my lack of understanding of python)
    # if you want to use a different function defined outside this module, you don't need to bother
    inDSName = args[1]
    splitted = inDSName.split('.')

    runNumber = splitted[1]
    physicsName = splitted[2]
    if splitted[0] == "user" or splitted[0] == "group": #this is in case we run on private derivations, either produced with user or group role
        runNumber = splitted[2]
        physicsName = splitted[3]
    derivation = splitted[-2]
    tags = splitted[-1].replace('/','')

    # grid has a limit for dataset names
    if len(physicsName) > 20:
        physicsName = physicsName.split('_')[0]

    outDSName = runNumber + '.' + physicsName + '.' + derivation + '.' + tags
    return outDSName

class Config:
    code = 'runTop_el.py'

    gridUsername    = ''
    groupProduction = None
    suffix          = ''
    outputName      = 'output'
    excludedSites   = ''
    forceSite       = ''
    noSubmit        = False
    mergeType       = 'Default' #None, Default, xAOD 
    destSE          = ''
    memory          = '2000' #in MB
    maxNFilesPerJob = ''
    otherOptions    = ''
    # skipShowerCheck = False
    nameShortener   = basicInDSNameShortener # default shortener function
    # customTDPFile   = None
    reuseTarBall    = False
    # checkPRW        = False

    def details(self):
        txt = '('

        print(logger.OKBLUE + 'Analysis Settings:' + logger.ENDC)
        print(' -Code:          ', self.code)

        if self.gridUsername.find('phys-') == 0:
            self.groupProduction = True
            txt = ' (group production)'
        else:
            self.groupProduction = False
            txt = ''
        print(logger.OKBLUE + 'Grid Settings:' + logger.ENDC)
        print(' -GridUsername:  ', self.gridUsername, txt)
        print(' -Suffix:        ', self.suffix)
        print(' -ExcludedSites: ', self.excludedSites)
        print(' -ForceSite:     ', self.forceSite)
        print(' -NoSubmit:      ', self.noSubmit)
        print(' -MergeType:     ', self.mergeType, 'out of (None, Default, xAOD)')
        print(' -memory:        ', self.memory, 'in MB')
        print(' -maxNFilesPerJob', self.maxNFilesPerJob)       
        # print(' -skipShowerCheck', self.skipShowerCheck)       
        print(' -OtherOptions:  ', self.otherOptions)
        print(' -nameShortener: ', self.nameShortener)
        print(' -reuseTarBall:  ', self.reuseTarBall)
        # print(' -checkPRW:      ', self.checkPRW)

        txt = self.destSE
        if len(txt) == 0:
            txt = '<not set>'
        print(' -DestSE         ', txt)

        print('')

def checkMergeType(configuration):
    type = configuration.mergeType
    if type != "None" and type != "Default" and type != "xAOD":
        print("MergeType must be set to either None, Default or xAOD")
        print('Not', type, "Since that doesn't make sense")
        sys.exit()


def checkForFile(filename):
    return os.path.exists(filename)


def checkForPrun():
    if distutils.spawn.find_executable('prun') == None:
        print(logger.FAIL + 'DANGER DANGER DANGER' + logger.ENDC)
        print('Could not find prun. If you use setupATLAS (you should) then')
        print('"localSetupPandaClient --noAthenaCheck" and run this again')
        sys.exit()


#Given a list of datasets, the command to run and a mode (egamma, muons) this
#submits one prun job per run
#This is mostly for internal use of the code
def submit(config, allSamples):
    checkForPrun()
    checkMergeType(config)
    config.details()

    tarfile = 'top-el.tar.gz'

    # Delete the old tarball if requested
    if not config.reuseTarBall:
        try:
            os.remove(tarfile)
        except OSError as e:
            # error number 2 is 'file doesn't exist' which is okay for us
            if e.errno == 2:
                pass
            else:
                raise

    these = []
    print(logger.OKBLUE + 'For these samples' + logger.ENDC)
    for sample in allSamples:
        currentDatasets = sample.datasets
        actuallyExists = []
        for ds in currentDatasets:
            these.append(ds)

    plural = ''
    if len(these) > 1:
        plural = 's'

    print('')
    print(logger.OKBLUE + 'Starting submission of %d sample%s' % (len(these), plural) + logger.ENDC)
    print('')

    isfirst = True
    for i, d_concatenated in enumerate(these):
        d = getShortenedConcatenatedSample(d_concatenated) # in case of coma-separated list of samples with same DSID and same first tag of each type
        print(logger.OKBLUE + 'Submitting %d of %d' % (i+1, len(these)) + logger.ENDC)

        # Make the output dataset name
        # for group production it has to start with "group." and we asume that gridUsername is the name of the group (e.g. phys-top)
        if config.groupProduction:
            outputDS = f'group.{config.gridUsername}.{config.nameShortener(d)}.{config.suffix}'
        else:
            outputDS = f'user.{config.gridUsername}.{config.nameShortener(d)}.{config.suffix}'

        cmd = 'prun \\\n'
        #special care for group production - we assume that gridUsername is the name of the group (e.g. phys-top)
        if config.groupProduction:
            cmd += f'--official --voms atlas:/atlas/{config.gridUsername}/Role=production  \\\n'
        cmd += f'--inDS={d_concatenated} \\\n'
        cmd += f'--outDS={outputDS} \\\n'
        CMTCONFIG = os.getenv("CMTCONFIG")
        cmd += f'--useAthenaPackages --cmtConfig={CMTCONFIG} \\\n'
        cmd += '--writeInputToTxt=IN:in.txt \\\n'
        cmd += f'--outputs={config.outputName}:{config.outputName}.root \\\n'
        # write the --exec commands includes moving the output such that panda can pick it up
        cmd += '--exec="'
        cmd += f'{config.code} -i in.txt -o {config.outputName}'
        cmd += '" \\\n'

        # you might really hate a certain site
        if len(config.excludedSites) > 0:
            cmd += '--excludedSite=' + config.excludedSites + ' \\\n'

        #you might really like a certain site
        if len(config.forceSite) > 0:
            cmd += '--site ' + config.forceSite + ' \\\n'

        #tar-up the first time only, to save time when submitting
        if isfirst:
            if checkForFile(tarfile) and config.reuseTarBall:# reuse existing tarball if it already exists
                print(logger.OKBLUE + 'Reusing existing tarball %s' % (tarfile) + logger.ENDC)
                cmd += '--inTarBall=%s \\\n' % tarfile
            elif config.reuseTarBall:# reuse existing tarball if it already exists
                print(logger.WARNING + 'Tarball %s not found - will re-create it' % (tarfile) + logger.ENDC)
                cmd += '--outTarBall=%s \\\n' % tarfile
            else:
                cmd += '--outTarBall=%s \\\n' % tarfile
            isfirst = False
        else:
            cmd += '--inTarBall=%s \\\n' % tarfile


        #maybe you don't want to submit the job?
        if config.noSubmit:
            cmd += '--noSubmit \\\n'

        #fewer files = happy grid
        if config.mergeType != "None":
            cmd += '--mergeOutput \\\n'

        #Write the output to a specific storage element?
        if len(config.destSE) > 0:
            cmd += '--destSE=\"%s\" \\\n' % config.destSE

        #xAOD merging - slide 9 https://indico.cern.ch/event/401703/contribution/2/2/material/slides/0.pdf
        #Only add TriggerMenuMetaDataTool option when xAODTriggerCnv is in the release (not in 2.3.15)
        #--mergeScript="xAODMerge -m xAODMaker::TriggerMenuMetaDataTool %OUT \`echo %IN | sed \'s/,/ /g\'\`" \\\n'
        if config.mergeType == "xAOD":
            cmd += '--mergeScript="xAODMerge %OUT \`echo %IN | sed \'s/,/ /g\'\`" \\\n'
            
        #how much memory, not sure what default is when not set
        if len(config.memory) > 0:
            cmd += '--memory=%s \\\n' % config.memory
            
        #how many files
        if len(config.maxNFilesPerJob) > 0:
            cmd += '--maxNFilesPerJob=%s \\\n' % config.maxNFilesPerJob

        #other options
        if len(config.otherOptions) > 0:
            cmd += '%s \\\n' % config.otherOptions

        #Make sure the command actually ends
        cmd += '\n'

        #show people what you're about to do
        print(cmd)

        #then do it
        for l in os.popen(cmd).readlines():
            print('>> ', l.strip())


if __name__ == '__main__':
    print("You don't run this directly!")
    print('For an example, see 01SubmitToGrid.py')


## gets the first AMI tag of a kind
def getFirstTag(tags,letter):
    tagList = tags.split('_')
    first = ''
    for tag in tagList:
        if tag.find(letter,0,1) != -1 and tag[1:].isdigit() and first == '':
            first = tag
    return first


# In MC16, a given DSID can have been "split" into several datasets, were some have more ami-tags
# This function takes as input a coma-separated list of datasets, and returns the name of the first sample if we are in such case
# It throws an error if the DSID of these datasets is different, or if the first ami-tag of each time is different for the different datasets
def getShortenedConcatenatedSample(sampleName):
    samples = sampleName.split(',')
    if len(samples) == 1: # the simplest case
        return samples[0]
    
    # check if the DSIDs are all the same
    DSID = samples[0].split('.')[1]
    firstTagOfFirstSample = { 'e':'', 's':'', 'a':'', 'r':'', 'f':'', 'm':'', 'p':'', }
    isFirstSample = True
    for s in samples:
        if s.split('.')[1] != DSID:
            print(logger.FAIL + " Issue with this concatenated sample: " + sampleName + logger.ENDC)
            print(logger.FAIL + " This syntax can only work if all dataset containers have the same DSID " + logger.ENDC)
            raise RuntimeError("Issue with contatenated samples.")
        AmiTags = s.split('.')[-1]
        for tagType in firstTagOfFirstSample:
            if firstTagOfFirstSample[tagType] == '' and isFirstSample:
                firstTagOfFirstSample[tagType] = getFirstTag(AmiTags,tagType)
            elif firstTagOfFirstSample[tagType] != getFirstTag(AmiTags,tagType):
                print(logger.FAIL + " Issue with this concatenated sample: " + sampleName + logger.ENDC)
                print(logger.FAIL + " This syntax can only work if all dataset containers have the same first tag of each type " + logger.ENDC)
                print(logger.FAIL + " And it seems there are two samples in this list, one with " + firstTagOfFirstSample[tagType] + " and one with " + getFirstTag(AmiTags,tagType) + " as first " + tagType + "-tag" + logger.ENDC)
                raise RuntimeError("Issue with contatenated samples.")
        isFirstSample = False
    return samples[0] # if we survived all the tests, return the first of the list
