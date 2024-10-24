/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

// $Id$
// File:  FourQubitConfigMessenger.cc
//
// Description:	Macro command defitions to set user configuration in
//		FourQubitConfigManager.
//
// 20170816  Michael Kelsey

#include "FourQubitConfigMessenger.hh"
#include "FourQubitConfigManager.hh"
#include "G4UIcmdWithAString.hh"


// Constructor and destructor

FourQubitConfigMessenger::FourQubitConfigMessenger(FourQubitConfigManager* mgr)
  : G4UImessenger("/g4cmp/", "User configuration for G4CMP phonon example"),
    theManager(mgr), hitsCmd(0) {
  hitsCmd = CreateCommand<G4UIcmdWithAString>("HitsFile",
			      "Set filename for output of phonon hit locations");
}


FourQubitConfigMessenger::~FourQubitConfigMessenger() {
  delete hitsCmd; hitsCmd=0;
}


// Parse user input and add to configuration

void FourQubitConfigMessenger::SetNewValue(G4UIcommand* cmd, G4String value) {
  if (cmd == hitsCmd) theManager->SetHitOutput(value);
}
