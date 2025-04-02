//
/// \file src/FourQubitPrimaryGeneratorAction.cc
/// \brief Definition of FourQubitPrimaryGeneratorAction class
///  

#include "G4Event.hh"
#include "G4Geantino.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4PhononLong.hh"
#include "G4PhononTransFast.hh"
#include "G4PhononTransSlow.hh"
#include "G4RandomDirection.hh"
#include "G4SystemOfUnits.hh"

#include "FourQubitPrimaryGeneratorAction.hh"

using namespace std;

FourQubitPrimaryGeneratorAction::FourQubitPrimaryGeneratorAction() { 
  fParticleGun  = new G4GeneralParticleSource();

  // default particle kinematics ("geantino" triggers random phonon choice)
  //  fParticleGun->SetParticleDefinition(G4Geantino::Definition());
  //  fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
  //  fParticleGun->SetParticlePosition(G4ThreeVector(0.0,0.0,0.4998095*CLHEP::cm));
  //  fParticleGun->SetParticleEnergy(0.0075*eV);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


FourQubitPrimaryGeneratorAction::~FourQubitPrimaryGeneratorAction() {
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

 
void FourQubitPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

  /*
  if (fParticleGun->GetParticleDefinition() == G4Geantino::Definition()) {
    G4double selector = G4UniformRand();
    if (selector<0.53539) {
      fParticleGun->SetParticleDefinition(G4PhononTransSlow::Definition()); 
    } else if (selector<0.90217) {
      fParticleGun->SetParticleDefinition(G4PhononTransFast::Definition());
    } else {
      fParticleGun->SetParticleDefinition(G4PhononLong::Definition());
    }
  }
  */
  
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));
  //  fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


