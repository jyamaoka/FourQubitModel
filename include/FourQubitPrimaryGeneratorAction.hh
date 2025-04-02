//
/// \file exoticphysics/phonon/include/FourQubitPrimaryGeneratorAction.hh
/// \brief Definition of the FourQubitPrimaryGeneratorAction class
///

#ifndef FourQubitPrimaryGeneratorAction_hh
#define FourQubitPrimaryGeneratorAction_hh 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"


class G4ParticleGun;
class G4GeneralParticleSource;
class G4Event;

class FourQubitPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  FourQubitPrimaryGeneratorAction();    
  virtual ~FourQubitPrimaryGeneratorAction();

  public:
    virtual void GeneratePrimaries(G4Event*);

  private:
    G4GeneralParticleSource* fParticleGun;

};

#endif


