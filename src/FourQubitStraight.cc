//
/// \file src/FourQubitStraight.cc
/// \brief Definition of FourQubitStraight class
///  

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4Orb.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4RunManager.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"

#include "FourQubitStraight.hh"
#include "FourQubitDetectorParameters.hh"

using namespace FourQubitDetectorParameters;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Primary Constructor
FourQubitStraight::FourQubitStraight(G4RotationMatrix *pRot,
                                       const G4ThreeVector &tLate,
                                       const G4String &pName,
                                       G4LogicalVolume *pMotherLogical,
                                       G4bool pMany,
                                       G4int pCopyNo,
                                       G4bool pSurfChk,
                                       G4float pLength)
{
  // Here, use the inputs to this to set up the geometry and fill out the PVPlacement data member,
  // which is the real output from this class (and which we'll access in our detector construction file
  // file.)

  ConstructStraight(pRot,
                     tLate,
                     pName,
                     pMotherLogical,
                     pMany,
                     pCopyNo,
                     pSurfChk,
                     pLength);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Default Constructor
FourQubitStraight::FourQubitStraight()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Destructor
FourQubitStraight::~FourQubitStraight()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Moving implementation down here so it's not in the constructor
void FourQubitStraight::ConstructStraight(G4RotationMatrix *pRot,
                                            const G4ThreeVector &tLate,
                                            const G4String &pName,
                                            G4LogicalVolume *pMotherLogical,
                                            G4bool pMany,
                                            G4int pCopyNo,
                                            G4bool pSurfChk,
                                            G4float pLength)
{

  // Start with some preliminaries - NIST manager
  G4NistManager *nist = G4NistManager::Instance();
  G4Material *niobium_mat = nist->FindOrBuildMaterial("G4_Nb");
  G4Material *air_mat = nist->FindOrBuildMaterial("G4_AIR");
  bool checkOverlaps = true;

  // Set up the visualization
  G4VisAttributes *niobium_vis = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0, 0.5));
  niobium_vis->SetVisibility(true);
  G4VisAttributes *air_vis = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.5));
  air_vis->SetVisibility(true);
  G4VisAttributes *attention_vis = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 1.0)); // used for debuging
  attention_vis->SetVisibility(true);

  //------------------------------------------------------------------------------------------
  // Start with a base layer of niobium into which our objects will fit. We'll return this in the end.
  G4String baseNbLayerName = pName;
  G4String baseNbLayerNameSolid = pName + "_solid";
  G4String baseNbLayerNameLog = pName + "_log";

  // Here, we can just make our baseNbLayer a simple box, since it won't interfere/overlap with any other
  // elements at this layer in the heirarchy
  G4Box *solid_baseNbLayer = new G4Box(baseNbLayerNameSolid,
                                       0.5 * pLength,
                                       0.55 * dp_tlCouplingEmptyDimY,
                                       0.55 * dp_groundPlaneDimZ);

  // Now attribute a physical material to the housing
  G4LogicalVolume *log_baseNbLayer = new G4LogicalVolume(solid_baseNbLayer,
                                                         niobium_mat,
                                                         baseNbLayerNameLog);
  //log_baseNbLayer->SetVisAttributes(attention_vis); //
  log_baseNbLayer->SetVisAttributes(G4VisAttributes::Invisible);//niobium_vis);

  // Now, create a physical volume and G4PVPlacement for storing as the final output. This is the
  // top volume.
  G4VPhysicalVolume *phys_baseNbLayer = new G4PVPlacement(pRot,
                                                          tLate,
                                                          log_baseNbLayer,
                                                          baseNbLayerName,
                                                          pMotherLogical,
                                                          pMany,
                                                          pCopyNo,
                                                          pSurfChk);
  fFundamentalVolumeList.push_back(std::tuple<std::string, G4String, G4VPhysicalVolume *>("Niobium", baseNbLayerName, phys_baseNbLayer));

  //------------------------------------------------------------------------------------------
  // Straight

  // 1. straight

    // // Straight horizontal line (SHL), (empty/cavity)
    G4String shlEmptyName = pName + "_sEmpty";
    G4String shlEmptyNameSolid = shlEmptyName + "_solid";
    G4String shlEmptyNameLog = shlEmptyName + "_log";
    G4Box *solid_shlEmpty = new G4Box(shlEmptyNameSolid, 0.5 * pLength, 0.5 * dp_tlCouplingEmptyDimY, 0.5 * dp_groundPlaneDimZ);
    G4LogicalVolume *log_shlEmpty = new G4LogicalVolume(solid_shlEmpty, air_mat, shlEmptyNameLog);


    G4VPhysicalVolume *shlEmpty = new G4PVPlacement(0, G4ThreeVector(0,0,0), log_shlEmpty, shlEmptyName, log_baseNbLayer, false, 0, true);
    log_shlEmpty->SetVisAttributes(air_vis);
    fFundamentalVolumeList.push_back(std::tuple<std::string, G4String, G4VPhysicalVolume *>("Vacuum", shlEmptyName, shlEmpty));

    // //------------------------------------------------------
    // // Straight horizontal line (SHL) (conductor)
    G4String shlConductorName = pName + "_shlConductor";
    G4String shlConductorNameSolid = shlConductorName + "_solid";
    G4String shlConductorNameLog = shlConductorName + "_log";
    G4Box *solid_shlConductor = new G4Box(shlConductorNameSolid, 0.5 * pLength, 0.5 * dp_tlCouplingConductorDimY, 0.5 * dp_shlConductorDimZ);
    G4LogicalVolume *log_shlConductor = new G4LogicalVolume(solid_shlConductor, niobium_mat, shlConductorNameLog);
    G4VPhysicalVolume *shlConductor = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), log_shlConductor, shlConductorName, log_shlEmpty, false, 0, true);
    log_shlConductor->SetVisAttributes(niobium_vis);
    fFundamentalVolumeList.push_back(std::tuple<std::string, G4String, G4VPhysicalVolume *>("Niobium", shlConductorName, shlConductor));

    

  ///////////////////////////////////////////
  // Output logical/physical volume selection
  //-----------------------------------------

  fLog_output = log_baseNbLayer;
  fPhys_output = phys_baseNbLayer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector<std::tuple<std::string, G4String, G4VPhysicalVolume *>> FourQubitStraight::GetListOfAllFundamentalSubVolumes()
{
  return fFundamentalVolumeList;
}
