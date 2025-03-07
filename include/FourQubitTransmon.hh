//
/// \file FourQubitTransmon.hh
/// \brief Definition of the class defining Transmon
///        

#ifndef FourQubitTransmon_h
#define FourQubitTransmon_h 1

#include "G4UnionSolid.hh"
#include "FourQubitDetectorParameters.hh"
#include "globals.hh"

#include "FourQubitComponentModel.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4PVPlacement;

/// Detector construction class to define materials and geometry.
class FourQubitTransmon : public FourQubitComponentModel
{
  public:
    FourQubitTransmon();
    ~FourQubitTransmon();
  
    //Constructor
    FourQubitTransmon(G4RotationMatrix * pRot,
					 const G4ThreeVector & tLate,
					 const G4String & pName,
					 G4LogicalVolume * pMotherLogical,
					 G4bool pMany,
					 G4int pCopyNo,
					 G4bool pSurfChk=false);
  
    //Access functions
    G4VPhysicalVolume * GetPhysicalVolume(){ return fPhys_output; }
    G4LogicalVolume * GetLogicalVolume(){ return fLog_output; }
  
    //Acutally make the model
    void ConstructTransmon(G4RotationMatrix * pRot,
				 const G4ThreeVector & tLate,
				   const G4String & pName,
				   G4LogicalVolume * pMotherLogical,
				   G4bool pMany,
				   G4int pCopyNo,
				   G4bool pSurfChk=false);

    std::vector<std::tuple<std::string,G4String,G4VPhysicalVolume*> > GetListOfAllFundamentalSubVolumes();  
  
  protected:

  private:

    //The final G4PVPlacement
    G4LogicalVolume * fLog_output;
    G4VPhysicalVolume * fPhys_output;
    std::vector<std::tuple<std::string,G4String,G4VPhysicalVolume*> > fFundamentalVolumeList;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
