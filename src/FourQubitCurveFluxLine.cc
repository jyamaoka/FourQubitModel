//
/// \file src/FourQubitCurveFluxLine.cc
/// \brief Definition of FourQubitCurveFluxLine class
///  

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4Orb.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"

#include "FourQubitCurveFluxLine.hh"
#include "FourQubitPad.hh"
#include "FourQubitDetectorParameters.hh"

using namespace FourQubitDetectorParameters;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Primary Constructor
FourQubitCurveFluxLine::FourQubitCurveFluxLine(G4RotationMatrix * pRot,
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

  ConstructCurveFluxLine(pRot,
			    tLate,
			    pName,
			    pMotherLogical,
			    pMany,
			    pCopyNo,
			    pSurfChk);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Default Constructor
FourQubitCurveFluxLine::FourQubitCurveFluxLine()
{  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Destructor
FourQubitCurveFluxLine::~FourQubitCurveFluxLine()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//Moving implementation down here so it's not in the constructor
void FourQubitCurveFluxLine::ConstructCurveFluxLine(G4RotationMatrix * pRot,
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

  //Set up the niobium visualization
  G4VisAttributes* niobium_vis= new G4VisAttributes(G4Colour(0.0,1.0,1.0,0.5));
  niobium_vis->SetVisibility(true);
  G4VisAttributes* air_vis= new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.5));
  air_vis->SetVisibility(true);
  G4VisAttributes* attention_vis= new G4VisAttributes(G4Colour(1.0,0.0,0.0,1.0));
  attention_vis->SetVisibility(true);
  


  //------------------------------------------------------------------------------------------
  //Start with a base layer of niobium into which our objects will fit. We'll return this in the end.
  G4String baseNbLayerName = pName;
  G4String baseNbLayerNameSolid = pName + "_solid";
  G4String baseNbLayerNameLog = pName + "_log";

  //Here, we can just make our baseNbLayer a simple box, since it won't interfere/overlap with any other
  //elements at this layer in the heirarchy
  G4Box * solid_baseNbLayer = new G4Box(baseNbLayerNameSolid,0.5*dp_cfluxLineBaseNbLayerDimX,0.5*dp_cfluxLineBaseNbLayerDimY,0.5*dp_cfluxLineBaseNbLayerDimZ);

  //Now attribute a physical material to the housing
  G4LogicalVolume * log_baseNbLayer = new G4LogicalVolume(solid_baseNbLayer,
							  niobium_mat,
							  baseNbLayerNameLog);
  //log_baseNbLayer->SetVisAttributes(attention_vis);
  log_baseNbLayer->SetVisAttributes(G4VisAttributes::Invisible);//niobium_vis);

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
  //Now make the pad of the flux line. The logical mother volume of these is the base
  //Ni layer. Pads come with their own visualization attributes already set.
  
  //Pad 1:
  G4String pad1Name = pName + "_FluxLinePad1";
  G4RotationMatrix * pad1Rot = new G4RotationMatrix();
  pad1Rot->rotateZ(90.*deg);
  FourQubitPad * pad1 = new FourQubitPad(pad1Rot,
							       G4ThreeVector(dp_cfluxLinePadOffsetX,dp_cfluxLinePadOffsetY,0),
							       pad1Name,
							       log_baseNbLayer,
							       false,
							       0,
							       checkOverlaps);
  G4LogicalVolume * log_pad1 = pad1->GetLogicalVolume();
  G4VPhysicalVolume * phys_pad1 = pad1->GetPhysicalVolume();
  AddComplexGeometryPadSubVolumesToThisList(pad1);
  
  
  // straight bit 
  // curve bit
  // straight bit 

  //------------------------------------------------------------------------------------------
  //Now make the flux line itself, in two parts: empty and conductor. The logical
  //mother volume of this is the base Ni layer.

  // straight 1
  G4String fl1NameEmpty = pName + "_FluxLine1Empty";
  G4String fl1NameEmptySolid = fl1NameEmpty + "_solid";
  G4String fl1NameEmptyLog = fl1NameEmpty + "_log";
  G4Box * solid_fluxLine1Empty = new G4Box(fl1NameEmptySolid,
					  0.5 * dp_fluxLine1EmptyDimX,
					  0.5 * dp_fluxLine1EmptyDimY,
					  0.5 * dp_fluxLine1EmptyDimZ);
  
  G4LogicalVolume * log_fluxLine1Empty = new G4LogicalVolume(solid_fluxLine1Empty,
							    air_mat,
							    fl1NameEmptyLog);
  G4VPhysicalVolume * phys_fluxLine1Empty = new G4PVPlacement(0,
							     G4ThreeVector(dp_fluxLine1LineOffsetX,dp_fluxLine1LineOffsetY,0),
							     log_fluxLine1Empty,
							     fl1NameEmpty,
							     log_baseNbLayer,
							     false,
							     0,
							     true);
  log_fluxLine1Empty->SetVisAttributes(air_vis);
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Vacuum",fl1NameEmpty,phys_fluxLine1Empty));
  
  
  G4String fl1NameConductor = pName + "_FluxLine1Conductor";
  G4String fl1NameConductorSolid = fl1NameConductor + "_solid";
  G4String fl1NameConductorLog = fl1NameConductor + "_log";
  G4Box * solid_fluxLine1Conductor = new G4Box(fl1NameConductorSolid,
					      0.5 * dp_fluxLine1ConductorDimX,
					      0.5 * dp_fluxLine1ConductorDimY,
					      0.5 * dp_fluxLine1ConductorDimZ);  
  G4LogicalVolume * log_fluxLine1Conductor = new G4LogicalVolume(solid_fluxLine1Conductor,
								niobium_mat,
								fl1NameConductorLog);
  G4VPhysicalVolume * phys_fluxLine1Conductor = new G4PVPlacement(0,
								 G4ThreeVector(0,0,0),
								 log_fluxLine1Conductor,
								 fl1NameConductor,
								 log_fluxLine1Empty,
								 false,
								 0,
								 true);
  log_fluxLine1Conductor->SetVisAttributes(niobium_vis);
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Niobium",fl1NameConductor,phys_fluxLine1Conductor));
  
  // Curve  
  //Some mathematical things to help (can't put these in the dp file because they can't be executed as constexpr)
  //double cornerFluxLineCurve1WrtBaseLayerX = dp_cornerFluxLineCurve1WrtPadCenterX; // + cornerFluxLinePadOffsetX;
  //double cornerFluxLineCurve1WrtBaseLayerY = dp_cornerFluxLineCurve1WrtPadCenterY; // + cornerFluxLinePadOffsetY;

  
  //Curve, empty part
  G4String curve1NameEmpty = pName + "_FluxCurveEmpty";
  G4String curve1NameEmptySolid = curve1NameEmpty + "_solid";
  G4String curve1NameEmptyLog = curve1NameEmpty + "_log";
  G4Tubs * solid_curve1Empty = new G4Tubs(curve1NameEmptySolid,dp_curveFluxLineCurveRadius - dp_fluxLine1EmptyDimX/2.0
                                          , dp_curveFluxLineCurveRadius + dp_fluxLine1EmptyDimX/2.0,dp_curveEmptyDimZ/2.0,180.0*deg,90.0*deg);

  G4LogicalVolume * log_curve1Empty = new G4LogicalVolume(solid_curve1Empty,air_mat,curve1NameEmptyLog);  
  G4VPhysicalVolume * phys_curve1Empty = new G4PVPlacement(0,
							      G4ThreeVector(dp_fluxLineCurveOffsetX,dp_fluxLineCurveOffsetY,0),
                    log_curve1Empty,
                    curve1NameEmpty,
                    log_baseNbLayer,
                    false,
                    0,
                    true);
  log_curve1Empty->SetVisAttributes(air_vis);
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Vacuum",curve1NameEmpty,phys_curve1Empty));


  //Curve, conductor part
  G4String curve1NameConductor = pName + "_FluxCurveConductor";
  G4String curve1NameConductorSolid = curve1NameConductor + "_solid";
  G4String curve1NameConductorLog = curve1NameConductor + "_log";
  G4Tubs * solid_curve1Conductor = new G4Tubs(curve1NameConductorSolid,dp_curveFluxLineCurveRadius - dp_fluxLine1ConductorDimX/2.0
                                              , dp_curveFluxLineCurveRadius + dp_fluxLine1ConductorDimX/2.0,dp_curveEmptyDimZ/2.0,180*deg,90.0*deg);
  G4LogicalVolume * log_curve1Conductor = new G4LogicalVolume(solid_curve1Conductor,niobium_mat,curve1NameConductorLog);  
  G4VPhysicalVolume * phys_curve1Conductor = new G4PVPlacement(0,
                    G4ThreeVector(0,0,0),
                    log_curve1Conductor,
                    curve1NameConductor,
                    log_curve1Empty,
                    false,
                    0,
                    true);
  log_curve1Conductor->SetVisAttributes(niobium_vis);
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Niobium",curve1NameConductor,phys_curve1Conductor));

  // straight 2
  G4String fl2NameEmpty = pName + "_FluxLine2Empty";
  G4String fl2NameEmptySolid = fl2NameEmpty + "_solid";
  G4String fl2NameEmptyLog = fl2NameEmpty + "_log";
  G4Box * solid_fluxLine2Empty = new G4Box(fl2NameEmptySolid,
					  0.5 * dp_fluxLine2EmptyDimX,
					  0.5 * dp_fluxLine2EmptyDimY,
					  0.5 * dp_fluxLine2EmptyDimZ);
  
  G4LogicalVolume * log_fluxLine2Empty = new G4LogicalVolume(solid_fluxLine2Empty,
							    air_mat,
							    fl2NameEmptyLog);
  G4VPhysicalVolume * phys_fluxLine2Empty = new G4PVPlacement(0,
							     G4ThreeVector(dp_fluxLine2LineOffsetX,dp_fluxLine2LineOffsetY,0),
							     log_fluxLine2Empty,
							     fl2NameEmpty,
							     log_baseNbLayer,
							     false,
							     0,
							     true);
  log_fluxLine2Empty->SetVisAttributes(air_vis);
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Vacuum",fl2NameEmpty,phys_fluxLine2Empty));
  
  
  G4String fl2NameConductor = pName + "_FluxLine2Conductor";
  G4String fl2NameConductorSolid = fl2NameConductor + "_solid";
  G4String fl2NameConductorLog = fl2NameConductor + "_log";
  G4Box * solid_fluxLine2Conductor = new G4Box(fl2NameConductorSolid,
					      0.5 * dp_fluxLine2ConductorDimX,
					      0.5 * dp_fluxLine2ConductorDimY,
					      0.5 * dp_fluxLine2ConductorDimZ);  
  G4LogicalVolume * log_fluxLine2Conductor = new G4LogicalVolume(solid_fluxLine2Conductor,
								niobium_mat,
								fl2NameConductorLog);
  G4VPhysicalVolume * phys_fluxLine2Conductor = new G4PVPlacement(0,
								 G4ThreeVector(0,0,0),
								 log_fluxLine2Conductor,
								 fl2NameConductor,
								 log_fluxLine2Empty,
								 false,
								 0,
								 true);
  log_fluxLine2Conductor->SetVisAttributes(niobium_vis);
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Niobium",fl2NameConductor,phys_fluxLine2Conductor));
  

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
std::vector<std::tuple<std::string,G4String,G4VPhysicalVolume*> > FourQubitCurveFluxLine::GetListOfAllFundamentalSubVolumes()
{
  return fFundamentalVolumeList;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FourQubitCurveFluxLine::AddComplexGeometryPadSubVolumesToThisList(FourQubitPad * pad)
{
  for( int iSubVol = 0; iSubVol < pad->GetListOfAllFundamentalSubVolumes().size(); ++iSubVol){
    std::tuple<std::string,G4String,G4VPhysicalVolume*> theTuple(std::get<0>(pad->GetListOfAllFundamentalSubVolumes()[iSubVol]),
								 std::get<1>(pad->GetListOfAllFundamentalSubVolumes()[iSubVol]),
								 std::get<2>(pad->GetListOfAllFundamentalSubVolumes()[iSubVol]));
    fFundamentalVolumeList.push_back(theTuple);
  }
}

