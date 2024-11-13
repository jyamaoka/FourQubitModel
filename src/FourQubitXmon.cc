//
//

//Includes (G4)
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

//Includes (specific to this project)
#include "FourQubitXmon.hh"
#include "FourQubitDetectorParameters.hh"

using namespace FourQubitDetectorParameters;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Primary Constructor
FourQubitXmon::FourQubitXmon(G4RotationMatrix * pRot,
								       const G4ThreeVector & tLate,
								       const G4String & pName,
								       G4LogicalVolume * pMotherLogical,
								       G4bool pMany,
								       G4int pCopyNo,
								       G4bool pSurfChk)
{
  //Here, use the inputs to this to set up the geometry and fill out the PVPlacement data member,
  //which is the real output from this class (and which we'll access in our detector construction file
  //file.)

  ConstructXmon(pRot,
			    tLate,
			    pName,
			    pMotherLogical,
			    pMany,
			    pCopyNo,
			    pSurfChk);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Default Constructor
FourQubitXmon::FourQubitXmon()
{  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Destructor
FourQubitXmon::~FourQubitXmon()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//Moving implementation down here so it's not in the constructor
void FourQubitXmon::ConstructXmon(G4RotationMatrix * pRot,
							     const G4ThreeVector & tLate,
							     const G4String & pName,
							     G4LogicalVolume * pMotherLogical,
							     G4bool pMany,
							     G4int pCopyNo,
							     G4bool pSurfChk)
{

  //Start with some preliminaries - NIST manager
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* niobium_mat = nist->FindOrBuildMaterial("G4_Nb");
  G4Material* air_mat = nist->FindOrBuildMaterial("G4_AIR");
  bool checkOverlaps = true;

  //Set up the visualization
  G4VisAttributes* niobium_vis= new G4VisAttributes(G4Colour(0.0,1.0,1.0,0.5));
  niobium_vis->SetVisibility(true);
  G4VisAttributes* air_vis= new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.5));
  air_vis->SetVisibility(true);
  G4VisAttributes* attention_vis= new G4VisAttributes(G4Colour(1.0,0.0,0.0,1.0)); // used for debuging
  attention_vis->SetVisibility(true);
  

  //------------------------------------------------------------------------------------------
  //Start with a base layer of niobium into which our objects will fit. We'll return this in the end.
  G4String baseNbLayerName = pName;
  G4String baseNbLayerNameSolid = pName + "_solid";
  G4String baseNbLayerNameLog = pName + "_log";

  //Here, we can just make our baseNbLayer a simple box, since it won't interfere/overlap with any other
  //elements at this layer in the heirarchy
  G4Box * solid_baseNbLayer = new G4Box(baseNbLayerNameSolid,
                                        0.5*dp_xmonBaseNbLayerDimX,
                                        0.5*dp_xmonBaseNbLayerDimY,
                                        0.5*dp_xmonBaseNbLayerDimZ);

  //Now attribute a physical material to the housing
  G4LogicalVolume * log_baseNbLayer = new G4LogicalVolume(solid_baseNbLayer,
							                                            niobium_mat,
							                                            baseNbLayerNameLog);
  log_baseNbLayer->SetVisAttributes(attention_vis); // 
//  log_baseNbLayer->SetVisAttributes(G4VisAttributes::Invisible);//niobium_vis);

  //Now, create a physical volume and G4PVPlacement for storing as the final output. This is the
  //top volume.
  G4VPhysicalVolume* phys_baseNbLayer = new G4PVPlacement(pRot,
							                                            tLate,
							                                            log_baseNbLayer,
							                                            baseNbLayerName, 
							                                            pMotherLogical,
							                                            pMany,
							                                            pCopyNo,
							                                            pSurfChk);
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Niobium",baseNbLayerName,phys_baseNbLayer));


  //------------------------------------------------------------------------------------------
  
  // X-Mon
  // Coupler
  
  //------------------------------------------------------
  //xmon block (empty)
  G4String xmonEmptyName = pName + "_xmonEmpty";
  G4String xmonEmptyNameSolid = xmonEmptyName + "_solid";
  G4String xmonEmptyNameLog = xmonEmptyName + "_log";

  G4Box * solid_xmonVertBlock0Empty = new G4Box("xmonVertBlock0Empty",
                                                (0.5 * dp_xmonVertBlockConductorDimX) + 0.5*dp_xmonSpacing, 
                                                (0.5 * dp_xmonVertBlockConductorDimY) + 0.5*dp_xmonSpacing,
                                                 0.5 * dp_xmonVertBlockEmptyDimZ);
  G4Box * solid_xmonVertBlock1Empty = new G4Box("xmonVertBlock1Empty",
                                                 0.5 * dp_xmonVertBlockEmptyDimX,
                                                 (0.5 * dp_xmonVertBlockEmptyDimY)-dp_xmonVertBlockEmptyNubDimY,
                                                 0.5 * dp_xmonVertBlockEmptyDimZ);
  G4UnionSolid * solid_xmonVertBlockEmpty = new G4UnionSolid("xmonVertBlockEmpty",solid_xmonVertBlock0Empty,solid_xmonVertBlock1Empty,0,G4ThreeVector(0,dp_xmonVertBlockEmptyNubDimY,0));

  G4Box * solid_xmonHorizontalBlockEmpty = new G4Box("xmonHorizontalBlockEmpty",0.5 * dp_xmonHorizontalBlockEmptyDimX,0.5 * dp_xmonHorizontalBlockEmptyDimY,0.5 * dp_xmonHorizontalBlockEmptyDimZ);

  G4UnionSolid * solid_xmonEmpty = new G4UnionSolid(xmonEmptyNameSolid,solid_xmonVertBlockEmpty,solid_xmonHorizontalBlockEmpty,0,G4ThreeVector(0,0,0));

  G4LogicalVolume * log_xmonEmpty = new G4LogicalVolume(solid_xmonEmpty,air_mat,xmonEmptyNameLog);

  G4VPhysicalVolume * xmonEmpty = new G4PVPlacement(0,G4ThreeVector(0,0,0),log_xmonEmpty,xmonEmptyName,log_baseNbLayer,false,0,true);
  log_xmonEmpty->SetVisAttributes(air_vis);
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Vacuum",xmonEmptyName,xmonEmpty));

  //------------------------------------------------------
  //xmon block (conductor)
  G4String xmonConductorName = pName + "_xmonConductor";
  G4String xmonConductorNameSolid = xmonConductorName + "_solid";
  G4String xmonConductorNameLog = xmonConductorName + "_log";
  G4Box * solid_xmonVertBlockConductor = new G4Box("xmonVertBlockConductor",0.5 * dp_xmonVertBlockConductorDimX,0.5 * dp_xmonVertBlockConductorDimY,0.5 * dp_xmonVertBlockConductorDimZ);
  G4Box * solid_xmonHorizontalBlockConductor = new G4Box("xmonHorizontalBlockConductor",0.5 * dp_xmonHorizontalBlockConductorDimX,0.5 * dp_xmonHorizontalBlockConductorDimY,0.5 * dp_xmonHorizontalBlockConductorDimZ);
  G4UnionSolid * solid_xmonConductor = new G4UnionSolid(xmonConductorNameSolid,solid_xmonVertBlockConductor,solid_xmonHorizontalBlockConductor,0,G4ThreeVector(0,0,0));  
  G4LogicalVolume * log_xmonConductor = new G4LogicalVolume(solid_xmonConductor,niobium_mat,xmonConductorNameLog);
  G4VPhysicalVolume * xmonConductor = new G4PVPlacement(0,G4ThreeVector(0,0,0),log_xmonConductor,xmonConductorName,log_xmonEmpty,false,0,true);
  log_xmonConductor->SetVisAttributes(niobium_vis);
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Niobium",xmonConductorName,xmonConductor));
    
  //------------------------------------------------------
  //Coupler (empty)
  G4String xmonCouplerEmptyName = pName + "_xmonCouplerEmpty";
  G4String xmonCouplerEmptyNameSolid = xmonCouplerEmptyName + "_solid";
  G4String xmonCouplerEmptyNameLog = xmonCouplerEmptyName + "_log";
  G4Box * solid_xmonCouplerHorizontalEmpty = new G4Box("xmonCouplerHorizontalEmptySolid",0.5 * dp_xmonCouplerHorizontalEmptyDimX,0.5 * dp_xmonCouplerHorizontalEmptyDimY,0.5 * dp_xmonCouplerHorizontalEmptyDimZ);
  G4Box * solid_xmonCouplerLeftLobeEmpty = new G4Box("xmonCouplerLeftLobeEmpty",0.5 * dp_xmonCouplerLobeEmptyDimX,0.5* dp_xmonCouplerLobeEmptyDimY, 0.5* dp_xmonCouplerLobeEmptyDimZ);
  G4Box * solid_xmonCouplerRightLobeEmpty = new G4Box("xmonCouplerRightLobeEmpty",0.5 * dp_xmonCouplerLobeEmptyDimX,0.5* dp_xmonCouplerLobeEmptyDimY, 0.5* dp_xmonCouplerLobeEmptyDimZ);
  G4UnionSolid * solid_xmonCouplerMerge1Empty = new G4UnionSolid("xmonCouplerHorizontalPlusLeft",solid_xmonCouplerHorizontalEmpty,solid_xmonCouplerLeftLobeEmpty,0,G4ThreeVector(-0.5*dp_xmonCouplerHorizontalEmptyDimX + 0.5*dp_xmonCouplerLobeEmptyDimX,0.5*(dp_xmonCouplerHorizontalEmptyDimY+dp_xmonCouplerLobeEmptyDimY),0));
  G4UnionSolid * solid_xmonCouplerEmpty = new G4UnionSolid(xmonCouplerEmptyNameSolid,solid_xmonCouplerMerge1Empty,solid_xmonCouplerRightLobeEmpty,0,G4ThreeVector(0.5*dp_xmonCouplerHorizontalEmptyDimX - 0.5*dp_xmonCouplerLobeEmptyDimX,0.5*(dp_xmonCouplerHorizontalEmptyDimY+dp_xmonCouplerLobeEmptyDimY),0));    
  G4LogicalVolume * log_xmonCouplerEmpty = new G4LogicalVolume(solid_xmonCouplerEmpty,air_mat,xmonCouplerEmptyNameLog);  
  G4ThreeVector xmonCoupler(0,
                            - (0.5 * dp_xmonVertBlockConductorDimY) - dp_xmonSpacing - 0.5*dp_xmonCouplerHorizontalEmptyDimY,
                            0);
  G4VPhysicalVolume * xmonCouplerEmpty = new G4PVPlacement(0,xmonCoupler,log_xmonCouplerEmpty,xmonCouplerEmptyName,log_baseNbLayer,false,0,true);
  log_xmonCouplerEmpty->SetVisAttributes(air_vis);
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Vacuum",xmonCouplerEmptyName,xmonCouplerEmpty));
  
  //------------------------------------------------------
  //Coupler (conductor)
  G4String xmonCouplerConductorName = pName + "_xmonCouplerConductor";
  G4String xmonCouplerConductorNameSolid = xmonCouplerConductorName + "_solid";
  G4String xmonCouplerConductorNameLog = xmonCouplerConductorName + "_log";
  G4Box * solid_xmonCouplerHorizontalConductor0 = new G4Box("xmonCouplerHorizontalConductor0",
                                                           0.5 * dp_xmonCouplerHorizontalConductorDimX,
                                                           0.5 * dp_xmonCouplerHorizontalConductorDimY,
                                                           0.5 * dp_xmonCouplerHorizontalConductorDimZ);
  G4Box * solid_xmonCouplerHorizontalConductor1 = new G4Box("xmonCouplerHorizontalConductor1", // lead to connect to outside
                                                           0.5 * dp_tlCouplingConductorDimY,
                                                           0.25 * (dp_xmonCouplerHorizontalEmptyDimY - dp_xmonCouplerHorizontalConductorDimY),
                                                           0.5 * dp_xmonCouplerHorizontalConductorDimZ); 
  
  G4Box * solid_xmonCouplerLeftLobeConductor = new G4Box("xmonCouplerLeftLobeConductor",0.5 * dp_xmonCouplerLobeConductorDimX,0.5* dp_xmonCouplerLobeConductorDimY, 0.5* dp_xmonCouplerLobeConductorDimZ);
  G4Box * solid_xmonCouplerRightLobeConductor = new G4Box("xmonCouplerRightLobeConductor",0.5 * dp_xmonCouplerLobeConductorDimX,0.5* dp_xmonCouplerLobeConductorDimY, 0.5* dp_xmonCouplerLobeConductorDimZ);
  
  G4UnionSolid * solid_xmonCouplerHorizontalConductor = new  G4UnionSolid("xmonCouplerHorizontalConductor", solid_xmonCouplerHorizontalConductor0, solid_xmonCouplerHorizontalConductor1, 0, G4ThreeVector(0,-0.5*dp_xmonCouplerHorizontalConductorDimY - 0.25*(dp_xmonCouplerHorizontalEmptyDimY - dp_xmonCouplerHorizontalConductorDimY),0));
  G4UnionSolid * solid_xmonCouplerMerge2Conductor = new G4UnionSolid("xmonCouplerHorizontalPlusLeft",solid_xmonCouplerHorizontalConductor,solid_xmonCouplerLeftLobeConductor,0,G4ThreeVector(-0.5*dp_xmonCouplerHorizontalConductorDimX + 0.5*dp_xmonCouplerLobeConductorDimX,0.5*(dp_xmonCouplerHorizontalConductorDimY+dp_xmonCouplerLobeConductorDimY),0));
  G4UnionSolid * solid_xmonCouplerConductor = new G4UnionSolid(xmonCouplerConductorNameSolid,solid_xmonCouplerMerge2Conductor,solid_xmonCouplerRightLobeConductor,0,G4ThreeVector(0.5*dp_xmonCouplerHorizontalConductorDimX - 0.5*dp_xmonCouplerLobeConductorDimX,0.5*(dp_xmonCouplerHorizontalConductorDimY+dp_xmonCouplerLobeConductorDimY),0));      
  G4LogicalVolume * log_xmonCouplerConductor = new G4LogicalVolume(solid_xmonCouplerConductor,niobium_mat,xmonCouplerConductorNameLog);
  G4VPhysicalVolume * xmonCouplerConductor = new G4PVPlacement(0,G4ThreeVector(0,0,0),log_xmonCouplerConductor,xmonCouplerConductorName,log_xmonCouplerEmpty,false,0,true);
  log_xmonCouplerConductor->SetVisAttributes(niobium_vis);
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Niobium",xmonCouplerConductorName,xmonCouplerConductor));
  

  //------------------------------------------------------------------------------------------
  //Res line conductor. 
  
  // anchor to coupler
  G4double resLineDimY = 0.5*dp_xmonBaseNbLayerDimY + xmonCoupler.getY() - 0.5*dp_xmonCouplerHorizontalEmptyDimY;

  // Res Line Empty
  G4String rlNameEmpty = pName + "_xmonResLineEmpty";
  G4String rlNameEmptySolid = rlNameEmpty + "_solid";
  G4String rlNameEmptyLog = rlNameEmpty + "_log";
  G4Box * solid_xmonResLineEmpty = new G4Box(rlNameEmptySolid,
					                                   0.5 * dp_xmonResLineEmptyDimY,
					                                   0.5 * resLineDimY,
					                                   0.5 * dp_xmonResLineDimZ);  
  G4LogicalVolume * log_xmonResLineEmpty = new G4LogicalVolume(solid_xmonResLineEmpty,
								air_mat,
								rlNameEmptyLog);

  //G4ThreeVector reslineemptyloc(0.0, 
  //                             -0.5* dp_shuntVertBlockEmptyDimY - dp_shuntCouplerHorizontalEmptyDimY - 0.5 * dp_xmonResLineDimX, 
  //                              0.0);

  G4ThreeVector reslineemptyloc =  xmonCoupler - G4ThreeVector(0,0.5*dp_xmonCouplerHorizontalEmptyDimY+0.5*resLineDimY,0);                      

  G4VPhysicalVolume * phys_xmonResLineEmpty = new G4PVPlacement(0,
								                                                reslineemptyloc,
								                                                log_xmonResLineEmpty,
								                                                rlNameEmpty,
								                                                log_baseNbLayer,
								                                                false,
								                                                0,
								                                                true);
  log_xmonResLineEmpty->SetVisAttributes(air_vis);
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Vacuum",rlNameEmpty,phys_xmonResLineEmpty));

  // Res Line
  G4String rlNameConductor = pName + "_xmonResLineConductor";
  G4String rlNameConductorSolid = rlNameConductor + "_solid";
  G4String rlNameConductorLog = rlNameConductor + "_log";
  G4Box * solid_xmonResLineConductor = new G4Box(rlNameConductorSolid,
					                                       0.5 * dp_xmonResLineDimX,
					                                       0.5 * resLineDimY,
					                                       0.5 * dp_xmonResLineDimZ);  
  G4LogicalVolume * log_xmonResLineConductor = new G4LogicalVolume(solid_xmonResLineConductor,
								                                                   niobium_mat,
								                                                   rlNameConductorLog);

  G4ThreeVector reslineloc(0,0,0);

  G4VPhysicalVolume * phys_xmonResLineConductor = new G4PVPlacement(0,
								 reslineloc,
								 log_xmonResLineConductor,
								 rlNameConductor,
								 log_xmonResLineEmpty,
								 false,
								 0,
								 true);
  log_xmonResLineConductor->SetVisAttributes(niobium_vis);
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Niobium",rlNameConductor,phys_xmonResLineConductor));

  ///////////////////////////////////////////
  // Output logical/physical volume selection
  //-----------------------------------------

  fLog_output = log_baseNbLayer;
  fPhys_output = phys_baseNbLayer;

  /*

  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,0.647,0.0,0.9));
  simpleBoxVisAtt->SetVisibility(true);
  log_QubitHousing->SetVisAttributes(simpleBoxVisAtt);
  */


  

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector<std::tuple<std::string,G4String,G4VPhysicalVolume*> > FourQubitXmon::GetListOfAllFundamentalSubVolumes()
{
  return fFundamentalVolumeList;
}

