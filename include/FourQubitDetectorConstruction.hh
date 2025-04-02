//
/// \file FourQubitDetectorConstruction.hh
/// \brief Definition of the class 
///

#ifndef FourQubitDetectorConstruction_hh
#define FourQubitDetectorConstruction_hh 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Material;
class G4VPhysicalVolume;
class G4CMPSurfaceProperty;
class G4CMPElectrodeSensitivity;
class FourQubitComponentModel;

class FourQubitDetectorConstruction : public G4VUserDetectorConstruction {
public:
  FourQubitDetectorConstruction();
  virtual ~FourQubitDetectorConstruction();
  virtual G4VPhysicalVolume* Construct();

private:
  void DefineMaterials();
  void SetupGeometry();
  void AttachPhononSensor(G4CMPSurfaceProperty* surfProp);
  void LogicalBorderCreation(FourQubitComponentModel* componentModel, G4VPhysicalVolume* physicalSiVolume, 
                             G4CMPSurfaceProperty* siNbInterface, 
                             G4CMPSurfaceProperty* siVacuumInterface);

private:
  G4Material* fLiquidHelium;
  G4Material* fGermanium;
  G4Material* fAluminum;
  G4Material* fTungsten;
  G4Material* fSilicon;
  G4Material* fNiobium;
  G4VPhysicalVolume* fWorldPhys;

  G4CMPSurfaceProperty* fSiNbInterface;
  G4CMPSurfaceProperty* fSiCopperInterface;
  G4CMPSurfaceProperty* fSiVacuumInterface;

  
  G4CMPElectrodeSensitivity* fSuperconductorSensitivity;
  G4bool fConstructed;
};

#endif

