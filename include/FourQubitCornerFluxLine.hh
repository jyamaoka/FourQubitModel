//
//
/// \file FourQubitPad.hh
/// \brief Definition of the class defining how a transmission line is constructed on a qubit chip
///        

#ifndef FourQubitCornerFluxLine_h
#define FourQubitCornerFluxLine_h 1


//#include "G4PVPlacement.hh"
#include "G4UnionSolid.hh"
#include "FourQubitDetectorParameters.hh"
#include "FourQubitPad.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4PVPlacement;

/// Detector construction class to define materials and geometry.
class FourQubitCornerFluxLine
{
  public:
    FourQubitCornerFluxLine();
    ~FourQubitCornerFluxLine();
  
    //This is the constructor that should be used in general. It does not have the current logical
    //volume included because that will be defined IN the Qubit housing implementation. All we need is
    //a set of info that is external to this, which should be self-contained.
    FourQubitCornerFluxLine(G4RotationMatrix * pRot,
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
    void ConstructCornerFluxLine(G4RotationMatrix * pRot,
				 const G4ThreeVector & tLate,
				   const G4String & pName,
				   G4LogicalVolume * pMotherLogical,
				   G4bool pMany,
				   G4int pCopyNo,
				   G4bool pSurfChk=false);

    std::vector<std::tuple<std::string,G4String,G4VPhysicalVolume*> > GetListOfAllFundamentalSubVolumes();
    void AddComplexGeometryPadSubVolumesToThisList(FourQubitPad * pad); 

  
  protected:

  private:

    //The final G4PVPlacement
    G4LogicalVolume * fLog_output;
    G4VPhysicalVolume * fPhys_output;
    std::vector<std::tuple<std::string,G4String,G4VPhysicalVolume*> > fFundamentalVolumeList;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
