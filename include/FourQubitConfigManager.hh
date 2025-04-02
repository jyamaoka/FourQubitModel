//
/// \file FourQubitConfigManager.hh
/// \brief Definition of the class 
/// 

#ifndef FourQubitConfigManager_hh
#define FourQubitConfigManager_hh 1

#include "globals.hh"

class FourQubitConfigMessenger;

class FourQubitConfigManager {
public:
  ~FourQubitConfigManager();	// Must be public for end-of-job cleanup
  static FourQubitConfigManager* Instance();   // Only needed by static accessors

  // Access current values
  static const G4String& GetHitOutput()  { return Instance()->Hit_file; }
  static const G4String& GetPrimaryOutput()  { return Instance()->Primary_file; }

  // Change values (e.g., via Messenger)
  static void SetHitOutput(const G4String& name)
    { Instance()->Hit_file=name; UpdateGeometry(); }

  // Change values (e.g., via Messenger)
  static void SetPrimaryOutput(const G4String& name)
    { Instance()->Hit_file=name; UpdateGeometry(); }

  
  static void UpdateGeometry();

private:
  FourQubitConfigManager();		// Singleton: only constructed on request
  FourQubitConfigManager(const FourQubitConfigManager&) = delete;
  FourQubitConfigManager(FourQubitConfigManager&&) = delete;
  FourQubitConfigManager& operator=(const FourQubitConfigManager&) = delete;
  FourQubitConfigManager& operator=(FourQubitConfigManager&&) = delete;

  static FourQubitConfigManager* theInstance;

private:
  G4String Hit_file;	// Output file of e/h hits ($G4CMP_HIT_FILE)
  G4String Primary_file;	// Output file of primaries

  FourQubitConfigMessenger* messenger;
};

#endif	/* FourQubitConfigManager_hh */
