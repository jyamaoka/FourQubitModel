
//
/// \file FourQubitSteppingAction.hh
/// \brief Definition of the class 
///    

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

