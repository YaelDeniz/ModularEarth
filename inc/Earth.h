#ifndef MODULAREARTH_EARTH_H
#define MODULAREARTH_EARTH_H
///////////////////////////////////////////////////////////////////////////////
/// \author Yael Deniz (yaeldeniz0499@gmail.com)
///////////////////////////////////////////////////////////////////////////////

#include "Layer.h"
//#include "Sublayer.h"

#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<fstream>

#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoVolume.h"
#include "TGeoShape.h"
#include "TGeoMatrix.h"
#include "TH1F.h"
#include "TPolyMarker3D.h"
#include "TPolyLine3D.h"
#include "TView.h"
#include "TCanvas.h"
#include "TGraph.h"

namespace ModularEarth {

    class Earth {
    /////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// class ModularEarth
    ///
    /// ModularEarth generates a full 3D layered Earth model using ROOT's geometry and tracking toolkit
    /// "TGeoManager".
    /// In ModularEarth an "Earth" volume consist of a assembly (TGeoVolumeAssembly) of Layers(TGeoVolumeAssembly of Sublayers).
    /// Each Layer consists of spherical shell sublayers, each discretized into blocks.
    /// This class:
    ///   - Reads a layered Earth model (e.g., PREM-like models) from a .txt file
    ///   - Stores and updates configuration of Layers, Sublayers, and Block materials
    ///   - Supports visual inspection and summary reports
    ///   - Interfaces with ROOT geometry and enables particle tracking via `TGeoManager`
    ///
    /// Key Features:
    ///   - Set Earth binning resolution
    ///   - Apply material updates at block or sublayer level
    ///   - Visualize 3D geometry
    ///   - Inspect or summarize any layer, sublayer, or block
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////

    private:
        // Geometry core
        TGeoManager* geoManager = nullptr;
        TGeoMaterial* matVac = nullptr;
        TGeoMedium* medVac = nullptr;
        TGeoVolume* top = nullptr;

        // Internal configuration
        std::vector<LayerConfig> layersconfigs;
        std::vector<SublayerConfig> sublayersconfigs;
        std::vector<SubLayersUpdates> sublayerupdates;
        std::vector<Layer> earthlayers;

        // Global Earth model defaults
        TString earth_name = "NewEarthModel";
        double dth = 10.0;
        double dphi = 10.0;
        double a_mass = 1.0;

        
        TString PATH_modularearth = gSystem->pwd();
        TString PATH_premdir=PATH_modularearth + "/PremTables/"; // path to PREM table models
        TString premtable_default="PREM_DEFAULT"; // filename containing PREM table models
        TString premtable; // filename containing PREM table models
        
        TPolyMarker3D *NuDetMarkers = nullptr;
        TPolyLine3D* nuLine = nullptr;

        //void function for Intersection calculation

        //

    public:
     
    Earth(TString earth_name = "NewEarthModel",
            TString premtable = "PREM_DEFAULT",
            double dth  = 10.0,
            double dphi = 10.0)
            : earth_name(earth_name),
            premtable(premtable),
            dth(dth),
            dphi(dphi) {
        geoManager = new TGeoManager(this->earth_name, this->earth_name + " Earth Model");
        matVac     = new TGeoMaterial("Vacuum", 0, 0, 0);
        medVac     = new TGeoMedium("Vacuum", 1, matVac);
        top        = new TGeoVolume("EarthTopSphere", new TGeoSphere("Sphere", 0, 7000), medVac);
        geoManager->SetTopVolume(top);
    }

        // Parse Earth model from prem tables (rmax, ρ, Z/A, layer index)
        void SetEarthModel(TString premtable);

        void SetBlockSizes(double dth, double dphi);

        // Build the full ROOT geometry tree from the loaded PREM configuration
        void BuiltEarth();

        void GetModelSummary() const;

        TGeoManager* GetGeoManager() const { return geoManager; }
        TGeoVolume*  GetTopVolume()  const { return top; }


    };
};
/*

// Define the Earth's blocks size
        void SetBlockSizes(double dth, double dphi);

        void SetEarthBins(double nbins_th, double nbins_phi);

        // Load manually all the layers and sublayers for the Earth model
        void SetEarthLayers(std::vector<LayerConfig> layersconfigs);
        
        // Update material properties of a specific sublayer within a specific layer
        void UpdateSublayerMaterial(int layer_index, int sublayer_index, double new_density, double new_a_mass, double new_z_a);
        
        // Update material properties of a specific set of blocks within a specific sublayer
        void UpdateBlocksInSubLayer(size_t sublayerIndex,
                const std::vector < int > & blockIDs,
                    const std::string & newName,
                        const std::vector < double > & newA,
                            const std::vector < double > & newZ,
                                const std::vector < double > & newDensity) ; 


*/
#endif
