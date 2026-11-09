#ifndef MODULAREARTH_EARTH_H
#define MODULAREARTH_EARTH_H
///////////////////////////////////////////////////////////////////////////////
/// \author Yael Deniz (yaeldeniz0499@gmail.com)
///////////////////////////////////////////////////////////////////////////////

#include "Layer.h"
#include "EarthPaths.h"
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

    struct BlockMatInfo {
        double      density;
        double      eYield;
        std::string materialName;
        double      originalDensity;
        double      originalEYield;
        bool        isModified         = false;
        bool        isSubLayerModified = false;
    };

    struct BlockMatUpdate {
        int    i_ly;
        int    i_sly;
        int    i_blk;
        double value;
    };

    // One neutrino path through the Earth for a fixed direction.
    // eyield[i]   = electron yield Ye = Z/A per segment
    // density[i]  = mass density ρ [g/cm³]   per segment
    // ne[i]       = electron number density ρ × Ye per segment
    // baseline[i] = chord length [km]          per segment
    struct PropaPath { //PropagationPathSegemnt
        double              costh;    // cos(zenith angle)  — direction identifier
        double              azimuth;  // azimuth angle [same units as passed to BuildPropaPath]
        std::vector<double> eyield;   // Ye per segment
        std::vector<double> density;  // ρ [g/cm³] per segment
        std::vector<double> ne;       // ρ × Ye per segment
        std::vector<double> baseline; // segment length [km]
    };

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
        //std::vector<SubLayersUpdates> sublayerupdates;
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

        mutable std::vector<std::vector<std::vector<BlockMatInfo>>> BlockMaterialCache;
        std::vector<std::vector<std::vector<BlockMatInfo>>> BlockMaterialCacheOriginal;

        EarthPaths* earthPaths = nullptr;

        // Geometry-only path cache populated by GetEarthPath(th_grid, phi).
        // MultiGeomPathCache[i] holds the ROOT-tracked segments for th_grid[i].
        // Density and eyield are NOT stored here; they are read from BlockMaterialCache
        // at query time so material edits never invalidate the geometry cache.
        std::vector<std::vector<PathSegment>> MultiGeomPathCache;

        // Parallel angle cache: (costh, azimuth) for each slot in MultiGeomPathCache.
        // Required so UpdateEarthPath/OriginalEarthPath can restore PropaPath.costh/azimuth.
        std::vector<std::pair<double,double>> MultiGeomAngleCache;

        void PopulateBlockMaterialCache();

        // Run ROOT tracking for (th, phi) and return geometry-only segments.
        // Checks MultiGeomPathCache first; runs tracking only on a cache miss.
        std::vector<PathSegment> SetEarthPath(double th, double phi, bool radians = false);

        // Build a single PropaPath for direction (th, phi) using SetEarthPath.
        PropaPath BuildPropaPath(double th, double phi = 0.0, bool radians = false);

        //void function for Intersection calculation
        void BuildGeometry();
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
        
        std::cout << "Setting Earth model: " << this->premtable << std::endl;
        SetEarthModel(this->premtable);
        std::cout << "Earth model set, call BuildEarth() to build the Earth geometry." << std::endl;
    }

        // Parse Earth model from prem tables (rmax, ρ, Z/A, layer index)
        void SetEarthModel(TString premtable_);

        void SetBlockSizes(double dth, double dphi);

        void SetEarthLayers(std::vector<LayerConfig> layersconfigs); // Load manually all the layers and sublayers for the Earth model

        /*
        void SetSublayerUpdates(int i_ly, // Index of the target Major Layer
                int i_sly, // Index of the target Sublayer whithin the Major Layer
                    double rho_sly, // New density of the target Sublayer
                        double a_mass_sly, // new atomic mass of the target Sublayer
                            double z_sly); // new atomic number of the target Sublayer
        */
        
        void SetSublayerUpdates(int i_ly, // Index of the target Major Layer
                int i_sly, // Index of the target Sublayer whithin the Major Layer
                    double rho_sly, // New density of the target Sublayer
                            double eyield_sly,
                        const std::string& UpdateMaterialName = "UpdatedSublayerMaterial"); // new electron yield , Ye = \sum w_i x (Z/A)_i, of the target Sublayer
        
        
        // Update material properties of a specific set of blocks within a specific sublayer
        void SetBlocksUpdates(size_t sublayerIndex,
                const std::vector < int > & blockIDs,
                    const std::string & newName,
                        const std::vector < double > & newA,
                            const std::vector < double > & newZ,
                                const std::vector < double > & newDensity) ;


        // Build the full ROOT geometry tree from the loaded PREM configuration
        void BuildEarth();
        void GetModelSummary() const;

        // Single-block setters (marks isModified only)
        void SetBlockDensity(int i_ly, int i_sly, int i_blk, double density_);
        void SetBlockEYield (int i_ly, int i_sly, int i_blk, double eYield_);

        // Batch single-block setters
        void SetBlockDensity(const std::vector<BlockMatUpdate>& updates);
        void SetBlockEYield (const std::vector<BlockMatUpdate>& updates);

        // Sublayer-level setters (marks both isModified and isSubLayerModified)
        void SetSublayerDensity(int i_ly, int i_sly, double density_);
        void SetSublayerEYield (int i_ly, int i_sly, double eYield_);

        const BlockMatInfo& GetBlockMatInfo(int i_ly, int i_sly, int i_blk) const;

        const std::vector<std::vector<std::vector<BlockMatInfo>>>& GetBlockMaterialCache() const;
        const std::vector<std::vector<std::vector<BlockMatInfo>>>& GetBlockMaterialCacheOriginal() const;

        void StatusCacheOriginal() const;

        void ResetBlockMaterialCache() const;

        void ForcedMaterialCache(std::vector<std::vector<std::vector<BlockMatInfo>>> NewBlockMaterialCache);

        // Set the detector position used by GetEarthPath.
        // Must be called after BuildEarth().
        void SetDetectorLocation(double lon, double lat, double R_det);

        // Sweep a th_grid × phi (fixed azimuth) and return one PropaPath per angle.
        // Uses MultiGeomPathCache when available; falls back to live tracking otherwise.
        // Must be called after BuildEarth() and SetDetectorLocation().

        //Multiple definiion of 
        std::vector<PropaPath> GetEarthPath(double th_grid= 180,
                                              double phi     = 0.0,
                                              bool   radians = false);

        std::vector<PropaPath> GetEarthPath(const std::vector<double>& th_grid, //Grid centers
                                              double phi     = 0.0,
                                              bool   radians = false);

        const std::vector<std::vector<PathSegment>>& GetMultiGeomPathCache() const { return MultiGeomPathCache; }

        // Re-apply the current (possibly modified) BlockMaterialCache to the cached geometry.
        // Requires a prior GetEarthPath call to populate MultiGeomPathCache.
        std::vector<PropaPath> UpdateEarthPath();

        // Re-apply BlockMaterialCacheOriginal (unmodified PREM) to the cached geometry.
        // Requires a prior GetEarthPath call to populate MultiGeomPathCache.
        std::vector<PropaPath> OriginalEarthPath();

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
