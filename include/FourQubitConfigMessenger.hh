/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

#ifndef FourQubitConfigMessenger_hh
#define FourQubitConfigMessenger_hh 1

// $Id$
// File:  FourQubitConfigMessenger.hh
//
// Description:	Macro command defitions to set user configuration in
//		FourQubitConfigManager.
//
// 20170816  Michael Kelsey

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
