//
//

// Includes (G4)
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
#include "G4RotationMatrix.hh"

// Includes (specific to this project)
#include "FourQubitCurve.hh"
#include "FourQubitDetectorParameters.hh"

using namespace FourQubitDetectorParameters;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Primary Constructor
FourQubitCurve::FourQubitCurve(G4RotationMatrix *pRot,
                               const G4ThreeVector &tLate,
                               const G4String &pName,
                               G4LogicalVolume *pMotherLogical,
                               G4bool pMany,
                               G4int pCopyNo,
                               G4bool pSurfChk,
                               G4float pRadius,
                               G4float pStart,
                               G4float pAngle)
{
  // Here, use the inputs to this to set up the geometry and fill out the PVPlacement data member,
  // which is the real output from this class (and which we'll access in our detector construction file
  // file.)

  ConstructCurve(pRot,
                 tLate,
                 pName,
                 pMotherLogical,
                 pMany,
                 pCopyNo,
                 pSurfChk,
                 pRadius,
                 pStart,
                 pAngle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Default Constructor
FourQubitCurve::FourQubitCurve()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Destructor
FourQubitCurve::~FourQubitCurve()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Moving implementation down here so it's not in the constructor
void FourQubitCurve::ConstructCurve(G4RotationMatrix *pRot,
                                    const G4ThreeVector &tLate,
                                    const G4String &pName,
                                    G4LogicalVolume *pMotherLogical,
                                    G4bool pMany,
                                    G4int pCopyNo,
                                    G4bool pSurfChk,
                                    G4float pRadius,
                                    G4float pStart,
                                    G4float pAngle)
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
                                       1.0 * pRadius + dp_tlCouplingEmptyDimY,
                                       1.0 * pRadius + dp_tlCouplingEmptyDimY,
                                       0.5 * dp_groundPlaneDimZ);

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
  // Curve

  // 2. curve

  //------------------------------------------------------

    //------------------------------------------------------
    // HalfCircle (empty/cavity)
    G4String halfCircleEmptyName = pName + "_halfCircleEmpty";
    G4String halfCircleEmptyNameSolid = halfCircleEmptyName + "_solid";
    G4String halfCircleEmptyNameLog = halfCircleEmptyName + "_log";

    G4Tubs *solid_halfCircleEmpty = new G4Tubs(halfCircleEmptyNameSolid,
                                               pRadius,
                                               pRadius + dp_tlCouplingEmptyDimY,
                                               dp_curveEmptyDimZ / 2.0,
                                               pStart * deg, pAngle * deg);
    G4LogicalVolume *log_halfCircleEmpty = new G4LogicalVolume(solid_halfCircleEmpty, air_mat, halfCircleEmptyNameLog);



    G4VPhysicalVolume *halfCircleEmpty = new G4PVPlacement(0,
                                                           G4ThreeVector(0,0,0) ,
                                                           log_halfCircleEmpty,
                                                           halfCircleEmptyName,
                                                           log_baseNbLayer,
                                                           false,
                                                           0,
                                                           true);
    log_halfCircleEmpty->SetVisAttributes(air_vis);
    fFundamentalVolumeList.push_back(std::tuple<std::string, G4String, G4VPhysicalVolume *>("Vacuum", halfCircleEmptyName, halfCircleEmpty));

    //------------------------------------------------------
    // HalfCircle (conductor)
    G4float CurveCentralRadius = pRadius + dp_tlCouplingEmptyDimY / 2.0;

    G4String halfCircleConductorName = pName + "_halfCircleConductor";
    G4String halfCircleConductorNameSolid = halfCircleConductorName + "_solid";
    G4String halfCircleConductorNameLog = halfCircleConductorName + "_log";

    G4Tubs *solid_halfCircleConductor = new G4Tubs(halfCircleConductorNameSolid,
                                                   CurveCentralRadius - dp_tlCouplingConductorDimY / 2.0,
                                                   CurveCentralRadius + dp_tlCouplingConductorDimY / 2.0,
                                                   dp_curveEmptyDimZ / 2.0, pStart * deg,
                                                   pAngle * deg);
    G4LogicalVolume *log_halfCircleConductor = new G4LogicalVolume(solid_halfCircleConductor, niobium_mat, halfCircleConductorNameLog);
    G4VPhysicalVolume *halfCircleConductor = new G4PVPlacement(0,
                                                               G4ThreeVector(0, 0, 0),
                                                               log_halfCircleConductor,
                                                               halfCircleConductorName,
                                                               log_halfCircleEmpty,
                                                               false,
                                                               0,
                                                               true);
    log_halfCircleConductor->SetVisAttributes(niobium_vis);
    fFundamentalVolumeList.push_back(std::tuple<std::string, G4String, G4VPhysicalVolume *>("Niobium", halfCircleConductorName, halfCircleConductor));

  

  ///////////////////////////////////////////
  // Output logical/physical volume selection
  //-----------------------------------------

  fLog_output = log_baseNbLayer;
  fPhys_output = phys_baseNbLayer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector<std::tuple<std::string, G4String, G4VPhysicalVolume *>> FourQubitCurve::GetListOfAllFundamentalSubVolumes()
{
  return fFundamentalVolumeList;
}
