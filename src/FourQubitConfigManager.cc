//
/// \file src/FourQubitConfigManager.cc
/// \brief Definition of FourQubitConfigManager class
///   Configure simulation

#include "G4RunManager.hh"

#include "FourQubitConfigManager.hh"
#include "FourQubitConfigMessenger.hh"

#include <stdlib.h>

// Constructor and Singleton Initializer
FourQubitConfigManager* FourQubitConfigManager::theInstance = 0;

FourQubitConfigManager* FourQubitConfigManager::Instance() {
  if (!theInstance) theInstance = new FourQubitConfigManager;
  return theInstance;
}

FourQubitConfigManager::FourQubitConfigManager()
  : Hit_file(getenv("G4CMP_HIT_FILE") ? 
      getenv("G4CMP_HIT_FILE"):"FourQubit_hits.txt"),
    Primary_file("FourQubit_primary.txt"),
    messenger(new FourQubitConfigMessenger(this)) { ;}

FourQubitConfigManager::~FourQubitConfigManager() {
  delete messenger; messenger = 0;
}

// Trigger rebuild of geometry if parameters change
void FourQubitConfigManager::UpdateGeometry() {
  G4RunManager::GetRunManager()->ReinitializeGeometry(true);
}
