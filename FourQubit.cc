//
/// \file FourQubit/FourQubit.cc
/// \brief Main program of the FourQubit example (based on G4CMP's phonon example)

#include "G4CMPConfigManager.hh"
#include "G4CMPPhysics.hh"
#include "G4CMPPhysicsList.hh"

#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"

#include "FTFP_BERT.hh"

#include "FourQubitActionInitialization.hh"
#include "FourQubitConfigManager.hh"
#include "FourQubitDetectorConstruction.hh"
#include "FourQubitDetectorParameters.hh"

using namespace FourQubitDetectorParameters;

int main(int argc, char** argv) {
  // Construct the run manager
  G4RunManager* runManager = new G4RunManager;

  // Set mandatory initialization classes
  FourQubitDetectorConstruction* detector = new FourQubitDetectorConstruction();
  runManager->SetUserInitialization(detector);

  FTFP_BERT* physics = new FTFP_BERT;
  physics->RegisterPhysics(new G4CMPPhysics);
  physics->SetCuts();
  runManager->SetUserInitialization(physics);

  // Set user action classes (different for Geant4 10.0)
  runManager->SetUserInitialization(new FourQubitActionInitialization);

  // Create configuration managers to ensure macro commands exist
  G4CMPConfigManager::Instance();
  FourQubitConfigManager::Instance();

  // Visualization manager
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc == 1) {  // Define UI session for interactive mode
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  } else {  // Batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  }

  delete visManager;
  delete runManager;

  return 0;
}
