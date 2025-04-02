//
/// \file src/FourQubitActionInitialization.cc
/// \brief Definition of FourQubitActionInitialization class
///   Initializes the Actions for the Simulation

#include "G4CMPStackingAction.hh"

#include "FourQubitActionInitialization.hh"
#include "FourQubitPrimaryGeneratorAction.hh"
#include "FourQubitSteppingAction.hh"

void FourQubitActionInitialization::Build() const {
  SetUserAction(new FourQubitPrimaryGeneratorAction);
  SetUserAction(new G4CMPStackingAction);
  SetUserAction(new FourQubitSteppingAction);
} 
