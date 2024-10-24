//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//

//Includes (basic)
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

//Includes (specific to this project)
#include "FourQubitPad.hh"
#include "FourQubitDetectorParameters.hh"

using namespace FourQubitDetectorParameters;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Primary Constructor
FourQubitPad::FourQubitPad(G4RotationMatrix * pRot,
								   const G4ThreeVector & tLate,
								   const G4String & pName,
								   G4LogicalVolume * pMotherLogical,
								   G4bool pMany,
								   G4int pCopyNo,
								   G4bool pSurfChk)
{
  //Here, use the inputs to this to set up the geometry and fill out the PVPlacement data member,
  //which is the real output from this class (and which we'll access in our main detector construction
  //file.)

  ConstructPad(pRot,
			tLate,
			pName,
			pMotherLogical,
			pMany,
			pCopyNo,
			pSurfChk);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Default Constructor
FourQubitPad::FourQubitPad()
{  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Destructor
FourQubitPad::~FourQubitPad()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//Moving implementation down here so it's not in the constructor
void FourQubitPad::ConstructPad(G4RotationMatrix * pRot,
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

  //Set up the niobium visualization
  G4VisAttributes* niobium_vis= new G4VisAttributes(G4Colour(0.0,1.0,1.0,0.5));
  niobium_vis->SetVisibility(true);
  G4VisAttributes* air_vis= new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.5));
  air_vis->SetVisibility(true);
  

  //------------------------------------------------------------------------------------------
  //For the pad, we start with a volume of air with the same dimensions as the niobium substrate.
  //Then we add the niobium pad within.
  G4String padEmptyPart1Name = pName + "_PadEmptyPart1";
  G4String padEmptyPart1NameLog = pName + "_PadEmptyPart1_log";
  G4String padEmptyPart1NameSolid = pName + "_PadEmptyPart1_solid";
  G4String padEmptyPart2Name = pName + "_PadEmptyPart2";
  G4String padEmptyPart2NameLog = pName + "_PadEmptyPart2_log";
  G4String padEmptyPart2NameSolid = pName + "_PadEmptyPart2_solid";
  G4Box * solid_padEmptyPart1 = new G4Box(padEmptyPart1NameSolid,
					  0.5 * dp_padEmptyPart1DimX,
					  0.5 * dp_padEmptyPart1DimY,
					  0.5 * dp_padEmptyPart1DimZ);
  G4Trd * solid_padEmptyPart2 = new G4Trd(padEmptyPart2NameSolid,
					  0.5 * dp_padEmptyPart2TrdX1,
					  0.5 * dp_padEmptyPart2TrdX2,
					  0.5 * dp_padEmptyPart2TrdY1,
					  0.5 * dp_padEmptyPart2TrdY2,
					  0.5 * dp_padEmptyPart2TrdZ);

  //We need to rotate the part 2 so that it can be aligned and placed next to part 1
  G4RotationMatrix * rotEmptyPart2 = new G4RotationMatrix();
  rotEmptyPart2->rotateX(90.*deg);
  rotEmptyPart2->rotateY(-90.*deg);

  G4ThreeVector transPart2EmptyWrtPart1Empty(dp_padEmptyPart1DimX/2.0 + dp_padEmptyPart2TrdZ/2.0,0,0);

  G4String padEmptyName = pName + "_PadEmpty";
  G4String padEmptyNameLog = pName + "_PadEmpty_log";
  G4String padEmptyNameSolid = pName + "_PadEmpty_solid";
  G4UnionSolid * solid_padEmpty = new G4UnionSolid(padEmptyNameSolid,
						   solid_padEmptyPart1,
						   solid_padEmptyPart2,
						   rotEmptyPart2,
						   transPart2EmptyWrtPart1Empty);







  //Now we make a smaller pad that is internal to the larger one. This is actually niobium
  //and is where the electrical contact is actually made
  G4String padConductorPart1Name = pName + "_PadConductorPart1";
  G4String padConductorPart1NameLog = pName + "_PadConductorPart1_log";
  G4String padConductorPart1NameSolid = pName + "_PadConductorPart1_solid";
  G4String padConductorPart2Name = pName + "_PadConductorPart2";
  G4String padConductorPart2NameLog = pName + "_PadConductorPart2_log";
  G4String padConductorPart2NameSolid = pName + "_PadConductorPart2_solid";
  G4Box * solid_padConductorPart1 = new G4Box(padConductorPart1NameSolid,
					      0.5 * dp_padPart1DimX,
					      0.5 * dp_padPart1DimY,
					      0.5 * dp_padPart1DimZ);
  G4Trd * solid_padConductorPart2 = new G4Trd(padConductorPart2NameSolid,
					      0.5 * dp_padPart2TrdX1,
					      0.5 * dp_padPart2TrdX2,
					      0.5 * dp_padPart2TrdY1,
					      0.5 * dp_padPart2TrdY2,
					      0.5 * dp_padPart2TrdZ);

  
  //We need to rotate the part 2 so that it can be aligned and placed next to part 1
  G4RotationMatrix * rotPart2 = new G4RotationMatrix();
  rotPart2->rotateX(90.*deg);
  rotPart2->rotateY(-90.*deg);

  G4ThreeVector transPart2WrtPart1(dp_padPart1DimX/2.0 + dp_padPart2TrdZ/2.0,0,0);

  G4String padConductorName = pName + "_PadConductor";
  G4String padConductorNameLog = pName + "_PadConductor_log";
  G4String padConductorNameSolid = pName + "_PadConductor_solid";
  G4UnionSolid * solid_padConductor = new G4UnionSolid(padConductorNameSolid,
						       solid_padConductorPart1,
						       solid_padConductorPart2,
						       rotPart2,
						       transPart2WrtPart1);


  //We now create a shift the non-empty pad and place it inside:
  G4ThreeVector transPadWrtEmptyPad(dp_padPart2InternalShiftX,0,0);

  

 
  ///////////////////////////////////////////
  // Logical and Physical Volume Creation
  //-----------------------------------------
  // Done after one solid object subsuming all of the geometry has been made.

  
  //Now attribute a physical material to the housing
  G4LogicalVolume * log_padEmpty = new G4LogicalVolume(solid_padEmpty,
						       air_mat, //Test for now
						       padEmptyNameLog);
  log_padEmpty->SetVisAttributes(air_vis);//G4VisAttributes::Invisible);
  

  G4LogicalVolume * log_padConductor = new G4LogicalVolume(solid_padConductor,
							   niobium_mat,
							   padConductorNameLog);
  log_padConductor->SetVisAttributes(niobium_vis);
  
  
  //Now, create a physical volume and G4PVPlacement for storing as the final output
  G4VPhysicalVolume* phys_padConductor = new G4PVPlacement(0,
							   transPadWrtEmptyPad,
							   log_padConductor,
							   padConductorName,
							   log_padEmpty,
							   false,
							   0,
							   true);
  
  G4VPhysicalVolume* phys_padEmpty = new G4PVPlacement(pRot,
						       tLate,
						       log_padEmpty,
						       padEmptyName, 
						       pMotherLogical,
						       pMany,
						       pCopyNo,
						       pSurfChk);

  //Push these back into the fundamental volume list
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Vacuum",padEmptyName,phys_padEmpty));
  fFundamentalVolumeList.push_back(std::tuple<std::string,G4String,G4VPhysicalVolume*>("Niobium",padConductorName,phys_padConductor));

  
  
  
  /*  
  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.9));
  simpleBoxVisAtt->SetVisibility(true);
  log_pad->SetVisAttributes(simpleBoxVisAtt);

  G4VisAttributes* simpleBoxVisAtt2= new G4VisAttributes(G4Colour(0.0,1.0,0.0,0.9));
  simpleBoxVisAtt2->SetVisibility(true);
  log_padEmpty->SetVisAttributes(simpleBoxVisAtt2);
  */

  


  
  // Lastly, make the logical volume and physical volume of the top/mother object accessible data members
  fLog_output = log_padEmpty;
  fPhys_output = phys_padEmpty;


  

}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector<std::tuple<std::string,G4String,G4VPhysicalVolume*> > FourQubitPad::GetListOfAllFundamentalSubVolumes()
{
  return fFundamentalVolumeList;
}
