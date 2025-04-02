//
/// \file FourQubitConfigMessenger.hh
/// \brief Definition of the class 
/// 

#ifndef FourQubitConfigMessenger_hh
#define FourQubitConfigMessenger_hh 1

#include "G4UImessenger.hh"

class FourQubitConfigManager;
class G4UIcmdWithAString;
class G4UIcommand;

class FourQubitConfigMessenger : public G4UImessenger {
public:
  FourQubitConfigMessenger(FourQubitConfigManager* theData);
  virtual ~FourQubitConfigMessenger();

  void SetNewValue(G4UIcommand* cmd, G4String value);

private:
  FourQubitConfigManager* theManager;
  G4UIcmdWithAString* hitsCmd;

private:
  FourQubitConfigMessenger(const FourQubitConfigMessenger&);	// Copying is forbidden
  FourQubitConfigMessenger& operator=(const FourQubitConfigMessenger&);
};

#endif /* FourQubitConfigMessenger_hh */
