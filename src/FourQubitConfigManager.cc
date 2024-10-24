/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

// $Id$
// File:  FourQubitConfigManager.cc
//
// Description:	Singleton container class for user configuration of G4CMP
//		phonon example. Looks for environment variables	at
//		initialization to set default values; active values may be
//		changed via macro commands (see FourQubitConfigMessenger).
//
// 20170816  M. Kelsey -- Extract hit filename from G4CMPConfigManager.

#include "FourQubitConfigManager.hh"
#include "FourQubitConfigMessenger.hh"
#include "G4RunManager.hh"
#include <stdlib.h>


// Constructor and Singleton Initializer

FourQubitConfigManager* FourQubitConfigManager::theInstance = 0;

FourQubitConfigManager* FourQubitConfigManager::Instance() {
  if (!theInstance) theInstance = new FourQubitConfigManager;
  return theInstance;
}

FourQubitConfigManager::FourQubitConfigManager()
  : Hit_file(getenv("G4CMP_HIT_FILE")?getenv("G4CMP_HIT_FILE"):"FourQubit_hits.txt"),
    Primary_file("FourQubit_primary.txt"),
    messenger(new FourQubitConfigMessenger(this)) {;}

FourQubitConfigManager::~FourQubitConfigManager() {
  delete messenger; messenger=0;
}


// Trigger rebuild of geometry if parameters change

void FourQubitConfigManager::UpdateGeometry() {
  G4RunManager::GetRunManager()->ReinitializeGeometry(true);
}
