//
/// \file src/FourQubitConfigMessenger.cc
/// \brief Definition of FourQubitConfigMessenger class
///   Messenger Configuation

#include "G4UIcmdWithAString.hh"

#include "FourQubitConfigMessenger.hh"
#include "FourQubitConfigManager.hh"

// Constructor and destructor
FourQubitConfigMessenger::FourQubitConfigMessenger(FourQubitConfigManager* mgr)
  : G4UImessenger("/g4cmp/", "User configuration for G4CMP phonon example"),
    theManager(mgr), hitsCmd(0) {
  hitsCmd = CreateCommand<G4UIcmdWithAString>("HitsFile",
			      "Set filename for output of phonon hit locations");
}

FourQubitConfigMessenger::~FourQubitConfigMessenger() {
  delete hitsCmd; hitsCmd = 0;
}

// Parse user input and add to configuration
void FourQubitConfigMessenger::SetNewValue(G4UIcommand* cmd, G4String value) {
  if (cmd == hitsCmd) theManager->SetHitOutput(value);
}
