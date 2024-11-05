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
#include "FourQubitTransmon.hh"
#include "FourQubitDetectorParameters.hh"

using namespace FourQubitDetectorParameters;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Primary Constructor
FourQubitTransmon::FourQubitTransmon(G4RotationMatrix * pRot,
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

  ConstructTransmon(pRot,
			    tLate,
			    pName,
			    pMotherLogical,
			    pMany,
			    pCopyNo,
			    pSurfChk);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Default Constructor
FourQubitTransmon::FourQubitTransmon()
{  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Destructor
FourQubitTransmon::~FourQubitTransmon()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//Moving implementation down here so it's not in the constructor
void FourQubitTransmon::ConstructTransmon(G4RotationMatrix * pRot,
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
  //Start with a base layer of air into which our objects will fit. We'll return this in the end.
  G4String baseAirLayerName = pName;
  G4String baseAirLayerNameSolid = pName + "_solid";
  G4String baseAirLayerNameLog = pName + "_log";

  //Here, we can just make our baseAirLayer a simple box, since it won't interfere/overlap with any other
  //elements at this layer in the heirarchy
  G4Box * solid_baseAirLayer = new G4Box(baseAirLayerNameSolid,0.5*dp_transmonFieldDimX,0.5*dp_transmonFieldDimY,0.5*dp_transmonFieldDimZ);

  //Now attribute a physical material to the housing
  G4LogicalVolume * log_baseAirLayer = new G4LogicalVolume(solid_baseAirLayer,
							  air_mat,
							  baseAirLayerNameLog);
  log_baseAirLayer->SetVisAttributes(attention_vis); // 
//  log_baseAirLayer->SetVisAttributes(G4VisAttributes::Invisible);//niobium_vis);

  //Now, create a physical volume and G4PVPlacement for storing as the final output. This is the
  //top volume.
  G4VPhysicalVolume* phys_baseAirLayer = new G4PVPlacement(pRot,
							  tLate,
							  log_baseAirLayer,
							  baseAirLayerName, 
							  pMotherLogical,
							  pMany,
							  pCopyNo,
							  pSurfChk);
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Niobium",baseAirLayerName,phys_baseAirLayer));


  //------------------------------------------------------------------------------------------
  
  // Cap bar 0 
  // Cap bar 1
  // Coupler side 0
  // Coupler side 1
  // Resonator coupler
  // Res lead

  //------------------------------------------------------------------------------------------
  //Capacitor Bar 0 conductor.  No need for emppty space as field is already empty

  // Cap Bar 0
  G4String cb0NameConductor = pName + "_transmonBar0Conductor";
  G4String cb0NameConductorSolid = cb0NameConductor + "_solid";
  G4String cb0NameConductorLog = cb0NameConductor + "_log";
  G4Box * solid_transmonCapBar0Conductor = new G4Box(cb0NameConductorSolid,
					      0.5 * dp_transmonCapBar0DimX,
					      0.5 * dp_transmonCapBar0DimY,
					      0.5 * dp_transmonCapBar0DimZ);  
  G4LogicalVolume * log_transmonCapBar0Conductor = new G4LogicalVolume(solid_transmonCapBar0Conductor,
								niobium_mat,
								cb0NameConductorLog);

  G4ThreeVector bar0loc(0.5*dp_transmonCapBar0DimX + 0.5*dp_transmonSpacing, 0.0, 0.0);

  G4VPhysicalVolume * phys_transmonCapBar0Conductor = new G4PVPlacement(0,
								 bar0loc,
								 log_transmonCapBar0Conductor,
								 cb0NameConductor,
								 log_baseAirLayer,
								 false,
								 0,
								 true);
  log_transmonCapBar0Conductor->SetVisAttributes(niobium_vis);
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Niobium",cb0NameConductor,phys_transmonCapBar0Conductor));
  
  //------------------------------------------------------------------------------------------
  //Capacitor Bar 1 conductor.  No need for emppty space as field is already empty

  // Cap Bar 1
  G4String cb1NameConductor = pName + "_transmonBar1Conductor";
  G4String cb1NameConductorSolid = cb1NameConductor + "_solid";
  G4String cb1NameConductorLog = cb1NameConductor + "_log";
  G4Box * solid_transmonCapBar1Conductor = new G4Box(cb1NameConductorSolid,
					      0.5 * dp_transmonCapBar1DimX,
					      0.5 * dp_transmonCapBar1DimY,
					      0.5 * dp_transmonCapBar1DimZ);  
  G4LogicalVolume * log_transmonCapBar1Conductor = new G4LogicalVolume(solid_transmonCapBar1Conductor,
								niobium_mat,
								cb1NameConductorLog);

  G4ThreeVector bar1loc(-0.5*dp_transmonCapBar1DimX - 0.5*dp_transmonSpacing, 0.0, 0.0);

  G4VPhysicalVolume * phys_transmonCapBar1Conductor = new G4PVPlacement(0,
								 bar1loc,
								 log_transmonCapBar1Conductor,
								 cb1NameConductor,
								 log_baseAirLayer,
								 false,
								 0,
								 true);
  log_transmonCapBar1Conductor->SetVisAttributes(niobium_vis);
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Niobium",cb1NameConductor,phys_transmonCapBar1Conductor));
    
  //------------------------------------------------------------------------------------------
  //Coupler Bar 0 conductor.  No need for emppty space as field is already empty

  // Coupling Bar 0
  G4String co0NameConductor = pName + "_transmonCoupler0Conductor";
  G4String co0NameConductorSolid = co0NameConductor + "_solid";
  G4String co0NameConductorLog = co0NameConductor + "_log";
  G4Box * solid_transmonCouBar0Conductor = new G4Box(co0NameConductorSolid,
					      0.5 * dp_transmonCapCoupDimX,
					      0.5 * dp_transmonCapCoupDimY,
					      0.5 * dp_transmonCapCoupDimZ);  
  G4LogicalVolume * log_transmonCapCoup0Conductor = new G4LogicalVolume(solid_transmonCouBar0Conductor,
								niobium_mat,
								co0NameConductorLog);

  G4ThreeVector cou0loc(-0.5*dp_transmonCapCoupDimX - 0.5*dp_transmonSpacing - 1.0*dp_transmonCapBar1DimX, 
                      dp_transmonSpacing+0.5*dp_transmonResCoupDimY+0.5*dp_transmonCapCoupDimY, 
                      0.0);

  G4VPhysicalVolume * phys_transmonCapCoup0Conductor = new G4PVPlacement(0,
								 cou0loc,
								 log_transmonCapCoup0Conductor,
								 co0NameConductor,
								 log_baseAirLayer,
								 false,
								 0,
								 true);
  log_transmonCapCoup0Conductor->SetVisAttributes(niobium_vis);
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Niobium",co0NameConductor,phys_transmonCapCoup0Conductor));

  //------------------------------------------------------------------------------------------
  //Coupler Bar 1 conductor.  No need for emppty space as field is already empty

  // Coupling Bar 1
  G4String co1NameConductor = pName + "_transmonCoupler1Conductor";
  G4String co1NameConductorSolid = co1NameConductor + "_solid";
  G4String co1NameConductorLog = co1NameConductor + "_log";
  G4Box * solid_transmonCouBar1Conductor = new G4Box(co1NameConductorSolid,
					      0.5 * dp_transmonCapCoupDimX,
					      0.5 * dp_transmonCapCoupDimY,
					      0.5 * dp_transmonCapCoupDimZ);  
  G4LogicalVolume * log_transmonCapCoup1Conductor = new G4LogicalVolume(solid_transmonCouBar1Conductor,
								niobium_mat,
								co1NameConductorLog);

  G4ThreeVector cou1loc(-0.5*dp_transmonCapCoupDimX - 0.5*dp_transmonSpacing - 1.0*dp_transmonCapBar1DimX, 
                      -1.0*dp_transmonSpacing-0.5*dp_transmonResCoupDimY-0.5*dp_transmonCapCoupDimY, 
                      0.0);

  G4VPhysicalVolume * phys_transmonCapCoup1Conductor = new G4PVPlacement(0,
								 cou1loc,
								 log_transmonCapCoup1Conductor,
								 co1NameConductor,
								 log_baseAirLayer,
								 false,
								 0,
								 true);
  log_transmonCapCoup1Conductor->SetVisAttributes(niobium_vis);
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Niobium",co1NameConductor,phys_transmonCapCoup1Conductor));

  //------------------------------------------------------------------------------------------
  //Res Coupling Bar conductor.  No need for emppty space as field is already empty

  // Res Coupling
  G4String rcNameConductor = pName + "_transmonResCoupConductor";
  G4String rcNameConductorSolid = rcNameConductor + "_solid";
  G4String rcNameConductorLog = rcNameConductor + "_log";
  G4Box * solid_transmonResCoupConductor = new G4Box(rcNameConductorSolid,
					      0.5 * dp_transmonResCoupDimX,
					      0.5 * dp_transmonResCoupDimY,
					      0.5 * dp_transmonResCoupDimZ);  
  G4LogicalVolume * log_transmonResCoupConductor = new G4LogicalVolume(solid_transmonResCoupConductor,
								niobium_mat,
								rcNameConductorLog);

  G4ThreeVector rescouploc(-0.5*dp_transmonResCoupDimX - 1.0*dp_transmonSpacing - 1.0*dp_transmonCapBar1DimX, 
                      0.0, 
                      0.0);

  G4VPhysicalVolume * phys_transmonResCoupConductor = new G4PVPlacement(0,
								 rescouploc,
								 log_transmonResCoupConductor,
								 rcNameConductor,
								 log_baseAirLayer,
								 false,
								 0,
								 true);
  log_transmonResCoupConductor->SetVisAttributes(niobium_vis);
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Niobium",rcNameConductor,phys_transmonResCoupConductor));

  //------------------------------------------------------------------------------------------
  //Res line conductor.  No need for emppty space as field is already empty

  // Res Line
  G4String rlNameConductor = pName + "_transmonResLineConductor";
  G4String rlNameConductorSolid = rlNameConductor + "_solid";
  G4String rlNameConductorLog = rlNameConductor + "_log";
  G4Box * solid_transmonResLineConductor = new G4Box(rlNameConductorSolid,
					      0.5 * dp_transmonResLineDimX,
					      0.5 * dp_transmonResLineDimY,
					      0.5 * dp_transmonResLineDimZ);  
  G4LogicalVolume * log_transmonResLineConductor = new G4LogicalVolume(solid_transmonResLineConductor,
								niobium_mat,
								rlNameConductorLog);

  G4ThreeVector reslineloc(-1.0*dp_transmonResCoupDimX - 1.0*dp_transmonSpacing - 1.0*dp_transmonCapBar1DimX - 0.5*dp_transmonResLineDimX, 
                      0.0, 
                      0.0);

  G4VPhysicalVolume * phys_transmonResLineConductor = new G4PVPlacement(0,
								 reslineloc,
								 log_transmonResLineConductor,
								 rlNameConductor,
								 log_baseAirLayer,
								 false,
								 0,
								 true);
  log_transmonResLineConductor->SetVisAttributes(niobium_vis);
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Niobium",rlNameConductor,phys_transmonResLineConductor));

  ///////////////////////////////////////////
  // Output logical/physical volume selection
  //-----------------------------------------

  fLog_output = log_baseAirLayer;
  fPhys_output = phys_baseAirLayer;

  /*

  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,0.647,0.0,0.9));
  simpleBoxVisAtt->SetVisibility(true);
  log_QubitHousing->SetVisAttributes(simpleBoxVisAtt);
  */


  

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector<std::tuple<std::string,G4String,G4VPhysicalVolume*> > FourQubitTransmon::GetListOfAllFundamentalSubVolumes()
{
  return fFundamentalVolumeList;
}

