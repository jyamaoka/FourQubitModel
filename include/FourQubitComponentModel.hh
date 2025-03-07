//
/// \file FourQubitComponentModel.hh
/// \brief Definition of the class defining Curve
///        

#ifndef FourQubitComponentModel_h
#define FourQubitComponentModel_h 1

#include "G4UnionSolid.hh"
#include "globals.hh"
#include "FourQubitPad.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4PVPlacement;
class FourQubitPad;

// Base class: FourQubitComponent
class FourQubitComponentModel {
    public:
        // Default constructor
        FourQubitComponentModel() : fLog_output(nullptr), fPhys_output(nullptr) {}
    
        // Virtual destructor to ensure proper cleanup of derived classes
        virtual ~FourQubitComponentModel() {}
    
        // Access functions to retrieve the logical and physical volumes
        virtual G4VPhysicalVolume* GetPhysicalVolume() { return fPhys_output; }
        virtual G4LogicalVolume* GetLogicalVolume() { return fLog_output; }
    
        // A method to get the list of sub-volumes (can be overridden by derived class if needed)
        virtual std::vector<std::tuple<std::string, G4String, G4VPhysicalVolume*>> GetListOfAllFundamentalSubVolumes() {
            return fFundamentalVolumeList;
        }
    
        // A method to add sub-volumes (can be overridden by derived class if needed)
        virtual void AddComplexGeometryPadSubVolumesToThisList(FourQubitPad* pad) {
            // Default implementation (if any)
        }
    
    protected:
        // Member variables common across components
        G4LogicalVolume* fLog_output;
        G4VPhysicalVolume* fPhys_output;
        std::vector<std::tuple<std::string, G4String, G4VPhysicalVolume*>> fFundamentalVolumeList;
};
#endif