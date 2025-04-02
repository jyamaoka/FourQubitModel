//
/// \file FourQubitSensitivity.hh
/// \brief Definition of the class 
///     

#ifndef FourQubitSensitivity_hh
#define FourQubitSensitivity_hh 1

#include "G4CMPElectrodeSensitivity.hh"

class FourQubitSensitivity final : public G4CMPElectrodeSensitivity {
public:
  FourQubitSensitivity(G4String name);
  virtual ~FourQubitSensitivity();
  // No copies
  FourQubitSensitivity(const FourQubitSensitivity&) = delete;
  FourQubitSensitivity& operator=(const FourQubitSensitivity&) = delete;
  /* Move is disabled for now because old versions of GCC can't move ofstream
  // Move OK
  FourQubitSensitivity(FourQubitSensitivity&&);
  FourQubitSensitivity& operator=(FourQubitSensitivity&&);
  */
  FourQubitSensitivity(FourQubitSensitivity&&) = delete;
  FourQubitSensitivity& operator=(FourQubitSensitivity&&) = delete;

  virtual void EndOfEvent(G4HCofThisEvent*);

  void SetHitOutputFile(const G4String& fn);
  void SetPrimaryOutputFile(const G4String& fn);

protected:
  virtual G4bool IsHit(const G4Step*, const G4TouchableHistory*) const;

private:
  std::ofstream primaryOutput;
  std::ofstream hitOutput;
  G4String primaryFileName;
  G4String hitFileName;
  
};

#endif
