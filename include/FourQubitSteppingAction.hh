/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

#ifndef FourQubitSteppingAction_hh
#define FourQubitSteppingAction_hh 1

#include "G4UserSteppingAction.hh"

#include <fstream>

class G4Step;

class FourQubitSteppingAction : public G4UserSteppingAction
{
public:

  FourQubitSteppingAction();
  virtual ~FourQubitSteppingAction();
  virtual void UserSteppingAction(const G4Step* step);
  void ExportStepInformation( const G4Step * step );
  
private:

  //Step info output file
  std::ofstream fOutputFile;
  
  
  
  
};

#endif

