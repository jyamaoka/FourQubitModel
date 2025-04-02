//
/// \file FourQubitPad.hh
/// \brief Definition of the class defining how a pad is constructed on a qubit chip
///        

#ifndef FourQubitPad_hh
#define FourQubitPad_hh 1

#include "globals.hh"

#include "FourQubitComponentModel.hh"
#include "FourQubitDetectorParameters.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4PVPlacement;

/// Detector construction class to define materials and geometry.
class FourQubitPad
{
  public:
    FourQubitPad();
    ~FourQubitPad();

    //This is the constructor that should be used in general. It does not have the current logical
    //volume included because that will be defined IN the Qubit housing implementation. All we need is
    //a set of info that is external to this, which should be self-contained.
    FourQubitPad(G4RotationMatrix * pRot,
				     const G4ThreeVector & tLate,
				     const G4String & pName,
				     G4LogicalVolume * pMotherLogical,
				     G4bool pMany,
				     G4int pCopyNo,
				     G4bool pSurfChk=false);
  
    //Access functions
    G4VPhysicalVolume * GetPhysicalVolume(){ return fPhys_output; }
    G4LogicalVolume * GetLogicalVolume(){ return fLog_output; }
  
    //Misc
    void ConstructPad(G4RotationMatrix * pRot,
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
    std::vector<std::tuple<std::string,G4String,G4VPhysicalVolume*> > fFundamentalVolumeList; //List of all fundamental sub-volumes in the pad
  
};

#endif
