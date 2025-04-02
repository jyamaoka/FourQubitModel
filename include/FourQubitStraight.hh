//
/// \file FourQubitStraight.hh
/// \brief Definition of the class defining Straight
///        

#ifndef FourQubitStraight_hh
#define FourQubitStraight_hh 1

#include "G4UnionSolid.hh"
#include "globals.hh"

#include "FourQubitComponentModel.hh"
#include "FourQubitDetectorParameters.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4PVPlacement;

/// Detector construction class to define materials and geometry.
class FourQubitStraight : public FourQubitComponentModel
{
  public:
    FourQubitStraight();
    ~FourQubitStraight();
  
    //Constructor
    FourQubitStraight(G4RotationMatrix * pRot,
					 const G4ThreeVector & tLate,
					 const G4String & pName,
					 G4LogicalVolume * pMotherLogical,
					 G4bool pMany,
					 G4int pCopyNo,
					 G4bool pSurfChk=false,
           G4float pLength=0.0);
  
    //Access functions
    G4VPhysicalVolume * GetPhysicalVolume(){ return fPhys_output; }
    G4LogicalVolume * GetLogicalVolume(){ return fLog_output; }
  
    //Acutally make the model
    void ConstructStraight(G4RotationMatrix * pRot,
				 const G4ThreeVector & tLate,
				   const G4String & pName,
				   G4LogicalVolume * pMotherLogical,
				   G4bool pMany,
				   G4int pCopyNo,
				   G4bool pSurfChk=false,
           G4float pLength=0.0);

    std::vector<std::tuple<std::string,G4String,G4VPhysicalVolume*> > GetListOfAllFundamentalSubVolumes();  

  private:

    //The final G4PVPlacement
    G4LogicalVolume * fLog_output;
    G4VPhysicalVolume * fPhys_output;
    std::vector<std::tuple<std::string,G4String,G4VPhysicalVolume*> > fFundamentalVolumeList;
  
};

#endif
