#
# Project configuration for UserAnalysis.
#

# Set the minimum required CMake version:
cmake_minimum_required( VERSION 3.6 FATAL_ERROR )

file( READ ${CMAKE_SOURCE_DIR}/version.txt _version )
string( STRIP ${_version} _version )

project(UserAnalysis VERSION ${_version} LANGUAGES C CXX )

unset( _version )

# Set Athena path
#set(ATHENA_PATH "${CMAKE_SOURCE_DIR}/athena" CACHE STRING "Local Athena path")

# Try to figure out what project is our parent. Just using a hard-coded list
# of possible project names. Basically the names of all the other
# sub-directories inside the Projects/ directory in the repository.
set( _parentProjectNames Athena AthenaP1 AnalysisBase AthAnalysis AthSimulation AthDerivation )
set( _defaultParentProject AnalysisBase )
foreach( _pp ${_parentProjectNames} )
   if( NOT "$ENV{${_pp}_DIR}" STREQUAL "" )
      set( _defaultParentProject ${_pp} )
      break()
   endif()
endforeach()

# Set the parent project name based on the previous findings:
set( ATLAS_PROJECT ${_defaultParentProject}
   CACHE STRING "The name of the parent project to build against" )

# Generate compile commands json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Clean up:
unset( _parentProjectNames )
unset( _defaultParentProject )

# Find the AnalysisBase project. This is what, amongst other things, pulls
# in the definition of all of the "atlas_" prefixed functions/macros.
find_package( ${ATLAS_PROJECT} REQUIRED )

# Set up CTest. This makes sure that per-package build log files can be
# created if the user so chooses.
atlas_ctest_setup()

# Set up the GitAnalysisTutorial project. With this CMake will look for "packages"
# in the current repository and all of its submodules, respecting the
# "package_filters.txt" file, and set up the build of those packages.
atlas_project( UserAnalysis 1.0.0 USE ${ATLAS_PROJECT} ${${ATLAS_PROJECT}_VERSION} )

# Set up the runtime environment setup script. This makes sure that the
# project's "setup.sh" script can set up a fully functional runtime environment,
# including all the externals that the project uses.
lcg_generate_env( SH_FILE ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/env_setup.sh )
install( FILES ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/env_setup.sh
   DESTINATION . )

# Setup IDE integration
# as in athena
set( ATLAS_ENABLE_IDE_HELPERS ON CACHE BOOL "Enable IDE helpers" )
if( ATLAS_ENABLE_IDE_HELPERS )
   set( ATLAS_IDEHELPERSCRIPTS_SETUP
      "${CMAKE_SOURCE_DIR}/../.vscode/IDEHelperScripts/Setup.cmake"
      CACHE FILEPATH "Setup file for the IDE / VS Code helpers" )
   include( "${ATLAS_IDEHELPERSCRIPTS_SETUP}" )
endif()

# Set up CPack. This call makes sure that an RPM or TGZ file can be created
# from the built project. Used by Panda to send the project to the grid worker
# nodes.
atlas_cpack_setup()

