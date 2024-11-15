cmake_minimum_required(VERSION 3.0)
project(FourQubit)

#----------------------------------------------------------------------------
# Find G4CMP package -- NOTE: Assumes user has run g4cmp_env.*sh script!
#
set(CMAKE_PREFIX_PATH $ENV{G4CMPINSTALL}/../..)
find_package(G4CMP REQUIRED)
include(${G4CMP_USE_FILE})

#----------------------------------------------------------------------------
# Find Geant4 package
# NOTE: WITH_GEANT4_UIVIS and USE_GEANT4_STATIC_LIBS are defined here
#
include($ENV{G4CMPINSTALL}/FindGeant4.cmake)

#----------------------------------------------------------------------------
# Setup include directories and compile definitions
# NOTE: Need to include G4CMP directories before G4.
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${G4CMP_INCLUDE_DIRS})
include(${G4CMP_USE_FILE})
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# RPATH stuff
#
# Automatically add paths of linked libraries to RPATH *unless* they are in the 
# build directory
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# Set the RPATH of the installed objects *unless* they are installing to a 
# system directory, in which case they shouldn't need an explicitly set RPATH.
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES 
     "${CMAKE_INSTALL_PREFIX}/lib" is_SYSTEM_DIR)
if("${is_SYSTEM_DIR}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif()

#----------------------------------------------------------------------------
# Build stuff
#
set(FourQubit_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FourQubitActionInitialization.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FourQubitSteppingAction.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FourQubitConfigManager.cc 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FourQubitConfigMessenger.cc 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FourQubitDetectorConstruction.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FourQubitPrimaryGeneratorAction.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FourQubitSensitivity.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FourQubitQubitHousing.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FourQubitPad.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FourQubitTransmissionLine.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FourQubitTransmon.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FourQubitXmon.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FourQubitResonatorAssembly.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FourQubitResonator.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FourQubitStraight.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FourQubitCurve.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FourQubitStraightFluxLine.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FourQubitCornerFluxLine.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FourQubitCurveFluxLine.cc
    )
    
if(USE_GEANT4_STATIC_LIBS)
    add_library(FourQubitLib STATIC ${FourQubit_SOURCES})
else()
    add_library(FourQubitLib SHARED ${FourQubit_SOURCES})
endif()
set_target_properties(FourQubitLib PROPERTIES OUTPUT_NAME g4cmpFourQubit)

message("G4CMP Libraries: ")
message(${G4CMP_LIBRARIES})

target_link_libraries(FourQubitLib ${G4CMP_LIBRARIES} ${Geant4_LIBRARIES})

add_executable(FourQubit FourQubit.cc)
target_link_libraries(FourQubit FourQubitLib)

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(SHIELDMODEL_SCRIPTS
  init_vis.mac
  vis.mac
  )

foreach(_script ${SHIELDMODEL_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/G4Macros/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()
#----------------------------------------------------------------------------

install(TARGETS FourQubitLib DESTINATION lib)
install(TARGETS FourQubit DESTINATION bin)
