//
/// \file FourQubitResonatorAssembly.hh
/// \brief Definition of the class defining resonator assembly
///        

#ifndef FourQubitResonatorAssembly_hh
#define FourQubitResonatorAssembly_hh 1


#include "G4PVPlacement.hh"
#include "globals.hh"

#include "FourQubitComponentModel.hh"
#include "FourQubitDetectorParameters.hh"
#include "FourQubitPad.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4PVPlacement;

/// Detector construction class to define materials and geometry.
class FourQubitResonatorAssembly : public FourQubitComponentModel
{
  public:
    FourQubitResonatorAssembly();
    ~FourQubitResonatorAssembly();

    //This is the constructor that should be used in general. It does not have the current logical
    //volume included because that will be defined IN the Qubit housing implementation. All we need is
    //a set of info that is external to this, which should be self-contained.
    FourQubitResonatorAssembly(G4RotationMatrix * pRot,
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
    void ConstructResonatorAssembly(G4RotationMatrix * pRot,
				    const G4ThreeVector & tLate,
				    const G4String & pName,
				    G4LogicalVolume * pMotherLogical,
				    G4bool pMany,
				    G4int pCopyNo,
				    G4bool pSurfChk=false);

  
  void MakeResonatorLine(G4String pName, G4LogicalVolume * log_baseNbLayer);
  void MakeShuntCapacitorCross(G4String pName, G4LogicalVolume * log_baseNbLayer);

  std::vector< std::tuple<std::string,G4String,G4VPhysicalVolume*> > GetListOfAllFundamentalSubVolumes();

  private:

    //The final G4PVPlacement
    G4LogicalVolume * fLog_output;
    G4VPhysicalVolume * fPhys_output;
    std::vector<std::tuple<std::string,G4String,G4VPhysicalVolume*> > fFundamentalVolumeList; //List of all fundamental sub-volumes in the transmission line. String 1 is "material_description", String 2 should be unique identifier (name of the sub-physical volume)
  
};

#endif
