//
/// \file FourQubitCurve.hh
/// \brief Definition of the class defining Curve
///        

#ifndef FourQubitCurve_h
#define FourQubitCurve_h 1

#include "G4UnionSolid.hh"
#include "FourQubitDetectorParameters.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4PVPlacement;

/// Detector construction class to define materials and geometry.
class FourQubitCurve
{
  public:
    FourQubitCurve();
    ~FourQubitCurve();
  
    //Constructor
    FourQubitCurve(G4RotationMatrix * pRot,
					 const G4ThreeVector & tLate,
					 const G4String & pName,
					 G4LogicalVolume * pMotherLogical,
					 G4bool pMany,
					 G4int pCopyNo,
					 G4bool pSurfChk=false,
           G4float pRadius=1.0,
           G4float pStart=1.0,
           G4float pAngle=1.0);
  
    //Access functions
    G4VPhysicalVolume * GetPhysicalVolume(){ return fPhys_output; }
    G4LogicalVolume * GetLogicalVolume(){ return fLog_output; }
  
    //Acutally make the model
    void ConstructCurve(G4RotationMatrix * pRot,
				 const G4ThreeVector & tLate,
				   const G4String & pName,
				   G4LogicalVolume * pMotherLogical,
				   G4bool pMany,
				   G4int pCopyNo,
				   G4bool pSurfChk=false,
           G4float pRadius=1.0,
           G4float pStart=1.0,
           G4float pAngle=1.0);

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
