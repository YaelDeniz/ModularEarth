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
        TString EarthName;
        double theta_width = 10.0;
        double phi_width = 10.0;
        double a_mass = 1.0;

        TString TableName="PREM_REFERENCE"; // filename containing PREM table models

        TPolyMarker3D *NuDetMarkers = nullptr;
        TPolyLine3D* nuLine = nullptr;

        //void function for Intersection calculation

        //

    public:
     
    Earth(TString earth_name = "NewEarthModel", 
            TString table_name = "PREM_REFERENCE",
            double delta_theta  = 10.0,
            double delta_phi = 10.0)
            : EarthName(earth_name),
            TableName(table_name),
            theta_width(delta_theta),
            phi_width(delta_phi)
            {}

        // Parse Earth model from file (rmax, ρ, Z/A, index)
        void SetEarthModel(TString table_name); 

        void GetConfigsSummary() const;

        // Change angular resolution (before building Earth)
        void SetEarthBinning(double dth, double dphi);
    };
};

#endif
