# Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration

# Declare the name of this package:
atlas_subdir( GridSubmission )

# This package uses ROOT:
find_package( ROOT REQUIRED COMPONENTS Core Hist RIO Tree )

# Install the Python modules of the package:
atlas_install_python_modules( python/*.py )

atlas_install_scripts( scripts/* )
