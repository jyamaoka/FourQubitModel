//
/// \file FourQubitQubitHousing.hh
/// \brief Definition of the class defining shield type A for the
///        QUIET fridge

#ifndef FourQubitQubitHousing_hh
#define FourQubitQubitHousing_hh 1

#include "globals.hh"

#include "FourQubitDetectorParameters.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4PVPlacement;

/// Detector construction class to define materials and geometry.
class FourQubitQubitHousing
{
  public:
    FourQubitQubitHousing();
    ~FourQubitQubitHousing();

    //This is the constructor that should be used in general. It does not have the current logical
    //volume included because that will be defined IN the Qubit housing implementation. All we need is
    //a set of info that is external to this, which should be self-contained.
    FourQubitQubitHousing(G4RotationMatrix * pRot,
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
    void ConstructQubitHousing(G4RotationMatrix * pRot,
			       const G4ThreeVector & tLate,
			       const G4String & pName,
			       G4LogicalVolume * pMotherLogical,
			       G4bool pMany,
			       G4int pCopyNo,
			       G4bool pSurfChk=false);
  
  private:

    //The final G4PVPlacement
    G4LogicalVolume * fLog_output;
    G4VPhysicalVolume * fPhys_output;
  
  
};

#endif
