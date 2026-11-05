#include "Earth.h"
#include "EarthPaths.h"
#include "Layer.h"
#include "Sublayer.h"

#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<fstream>
#include <sstream>

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

// SetEarthModel — reads a PREM-format table from ModularEarth/tables/<file_name>.txt and
// generates the corresponding LayerConfig and SublayerConfig objects for the Earth model.
//
// .txt file format (one row per radial shell boundary):
//
//   "Outer Boundary[km]"  "Massrho_sl[g/cm³]"  "Z/A"  "Major Layer Index"
//
// Each row defines the OUTER boundary of one sublayer. The inner boundary
// is carried forward from the previous row (starting at 0 for the first row).
// SublayerConfig objects are created for each row
// SublayerConfig objects are grouped into LayerConfig objects based on the Major Layer Index.
// When the index a major layer changes between rows, the accumulated sublayers are
// stored in a LayerConfig object and the sublayer list is reset for the sets of sublayers
// beloging to the new next major layer.

    void Earth::SetEarthModel(TString premtable) {

        std::string PATH_table;
        if (premtable.IsNull()){
            PATH_table = (PATH_premdir + premtable_default + ".txt").Data();
        }

        else
        {
            PATH_table = (PATH_premdir + premtable + ".txt").Data();
        }

        std::ifstream infile(PATH_table);
        if (!infile) {
            std::cerr << "ERROR: Could not open model file: " << PATH_table << std::endl;
            return;
        }

        std::string line;
        double r_sly = 0.0;
        int ly_prev = -1;
        int sly = 1;

        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            double R_sly, rho_sly, zoa_sly;
            int ly;

            if (!(iss >> R_sly >> rho_sly >> zoa_sly >> ly)) {
                std::cerr << "WARNING: Invalid line skipped: " << line << std::endl;
                continue;
            }

            //If the index changes, it indicates a new major geological layer. 
            // Store the previous layer's sublayer configurations and reset for the new layer. 
            if (ly != ly_prev && ly_prev != -1) {
                // Commit the  layer's sublayer configurations 
                layersconfigs.push_back({"Layer", ly_prev+1, dth, dphi, sublayersconfigs});
                sublayersconfigs.clear(); // Reset sublayer configurations for a new major layer
                sly = 1; // Reset sublayer index for a new major layer
            }

            // Add sublayer configuration defining the current major geological layer with index 
            sublayersconfigs.push_back({sly, r_sly, R_sly, rho_sly, a_mass, zoa_sly, "SublayerMaterial"});
            r_sly = R_sly;
            ly_prev = ly;
            sly++;
        }

        if (!sublayersconfigs.empty()) {
            layersconfigs.push_back({"Layer", ly_prev+1, dth, dphi, sublayersconfigs});
        }
    }

    void Earth::SetSublayerUpdates(int i_ly, // 1-based index of the target major layer
                int i_sly, // 1-based index of the target sublayer within that layer
                    double rho_sly, // New density of the target sublayer  [g/cm³]
                            double eyield_sly, // New electron yield  Ye = Σ wᵢ·(Z/A)ᵢ
                        const std::string& UpdateMaterialName)
    {
        if (!earthlayers.empty()) {
            std::cerr << "Warning: Earth already built. Call SetSublayerUpdates before BuiltEarth().\n";
            return;
        }

        if (i_ly < 1 || i_ly > static_cast<int>(layersconfigs.size())) {
            std::cerr << "Error: Layer index " << i_ly
                      << " is out of bounds (valid range: 1–" << layersconfigs.size() << ").\n";
            return;
        }

        if (i_sly < 1 || i_sly > static_cast<int>(layersconfigs[i_ly - 1].sublayersconfig.size())) {
            std::cerr << "Error: Sublayer index " << i_sly
                      << " is out of bounds for layer " << i_ly
                      << " (valid range: 1–" << layersconfigs[i_ly - 1].sublayersconfig.size() << ").\n";
            return;
        }

        layersconfigs[i_ly - 1].sublayersconfig[i_sly - 1].density      = rho_sly;
        layersconfigs[i_ly - 1].sublayersconfig[i_sly - 1].atomicMass   = 1.0;
        layersconfigs[i_ly - 1].sublayersconfig[i_sly - 1].atomicNumber = eyield_sly;
        layersconfigs[i_ly - 1].sublayersconfig[i_sly - 1].materialName = UpdateMaterialName;
    }

    



    void Earth::BuiltEarth() {

        if (!earthlayers.empty()) {
            std::cerr << "Warning: BuiltEarth() already called. Create a new Earth instance to rebuild.\n";
            return;
        }

        if (layersconfigs.empty()) {
            std::cout << "No layer configuration set for the current Earth model.\n"
                    << "Use Earth::SetEarthModel to load a predefined model, "
                    << "or add layers before attempting to build the Earth.\n";
            return;
        }

        // Reserve upfront so emplace_back never reallocates while we hold references.
        earthlayers.reserve(layersconfigs.size());

        for (size_t ly = 0; ly < layersconfigs.size(); ++ly) {
            const auto& layer_config = layersconfigs[ly];

            if (layer_config.sublayersconfig.empty()) {
                std::cout << "  Incomplete configuration: no sublayers in layer " << (ly + 1)
                        << ". Skipping.\n";
                continue;
            }

            int i_ly = static_cast<int>(ly) + 1; // 1-based layer index

            // Construct the Layer directly inside earthlayers (avoids copying raw pointers).
            earthlayers.emplace_back("Layer", i_ly, dth, dphi);
            Layer& MajorLayer = earthlayers.back();
            MajorLayer.SetSubLayers(layer_config.sublayersconfig);
            MajorLayer.BuildLayer();
            top->AddNode(MajorLayer.GetLayerVolume(), i_ly,
                         new TGeoTranslation(0, 0, 0));
        }

        this->BuildGeometry();
        this->PopulateBlockMaterialCache();
        earthPaths = new EarthPaths(geoManager);

        // Attempt 2: via CLING interpreter (bypasses cppyy dispatch)
        // ret = ROOT.gROOT.ProcessLine("gGeoManager->CloseGeometry();")
        // println("ProcessLine return: ", ret)
        // println("IsClosed after: ", pyconvert(Bool, geo.IsClosed()))
    }

        void Earth::GetModelSummary() const {
        if (layersconfigs.empty()) {
            std::cout << "No layer configuration set for the current Earth model.\n"
                    << "Use Earth::SetEarthModel to load a predefined model, "
                    << "or add layers before calling the layer summary.\n";
            return;
        }

        for (size_t ly = 0; ly < layersconfigs.size(); ++ly) {
            const auto& layer_config = layersconfigs[ly];

            std::cout << "\n--- Layer " << layer_config.layerindex << " (i_ly = " << layer_config.layerindex << ") ---\n";

            if (layer_config.sublayersconfig.empty()) {
                std::cout << "  Incomplete configuration: no sublayers in this layer. "
                        << "Please redo the configs.\n";
                continue;
            }

            for (const auto& slayer_config : layer_config.sublayersconfig) {
                std::cout << "  Sublayer " << slayer_config.sublayerID
                        << " | rmin: " << slayer_config.rmin
                        << " | rmax: " << slayer_config.rmax
                        << " | \u03c1: "   << slayer_config.density
                        << " | Z/A: " << slayer_config.atomicNumber << "\n";
            }
        }
    }

    void Earth::BuildGeometry() {
        if (geoManager == nullptr) {
            std::cerr << "Error: TGeoManager is not initialized. Cannot build geometry." << std::endl;
            return;
        }

        if (top == nullptr) {
            std::cerr << "Error: Top Volume is not constructed. Cannot build geometry." << std::endl;
            return;
        }

        geoManager->CloseGeometry();

    }


    // Change angular resolution (before building Earth)
    void Earth::SetBlockSizes(double dth, double dphi) {
        this->dth = dth;
        this->dphi = dphi;
    }

    void Earth::PopulateBlockMaterialCache() {
        BlockMaterialCache.resize(earthlayers.size());
        for (size_t i = 0; i < earthlayers.size(); ++i) {
            const auto& slys = earthlayers[i].GetSublayers();
            BlockMaterialCache[i].resize(slys.size());
            for (size_t j = 0; j < slys.size(); ++j) {
                const auto& blks = slys[j].GetBlocks();
                BlockMaterialCache[i][j].resize(blks.size());
                for (size_t k = 0; k < blks.size(); ++k) {
                    double rho = blks[k].GetDensity();
                    double ye  = blks[k].GetEYield();
                    BlockMaterialCache[i][j][k] = {
                        rho, ye, blks[k].GetMaterialName(),
                        rho, ye,
                        false, false
                    };
                }
            }
        }
        BlockMaterialCacheOriginal = BlockMaterialCache;
    }

    void Earth::ResetBlockMaterialCache() const {
        BlockMaterialCache = BlockMaterialCacheOriginal;
    }


    // Single-block setters
    void Earth::SetBlockDensity(int i_ly, int i_sly, int i_blk, double density_) {
        auto& b = BlockMaterialCache[i_ly-1][i_sly-1][i_blk-1];
        b.density    = density_;
        b.isModified = true;
    }

    void Earth::SetBlockEYield(int i_ly, int i_sly, int i_blk, double eYield_) {
        auto& b = BlockMaterialCache[i_ly-1][i_sly-1][i_blk-1];
        b.eYield     = eYield_;
        b.isModified = true;
    }

    // Batch single-block setters
    void Earth::SetBlockDensity(const std::vector<BlockMatUpdate>& updates) {
        for (const auto& u : updates) {
            if (u.i_ly  < 1 || u.i_ly  > (int)BlockMaterialCache.size()           ||
                u.i_sly < 1 || u.i_sly > (int)BlockMaterialCache[u.i_ly-1].size() ||
                u.i_blk < 1 || u.i_blk > (int)BlockMaterialCache[u.i_ly-1][u.i_sly-1].size()) {
                std::cerr << "[SetBlockDensity] Out-of-range index ("
                          << u.i_ly << "," << u.i_sly << "," << u.i_blk << ") — skipping.\n";
                continue;
            }
            SetBlockDensity(u.i_ly, u.i_sly, u.i_blk, u.value);
        }
    }

    void Earth::SetBlockEYield(const std::vector<BlockMatUpdate>& updates) {
        for (const auto& u : updates) {
            if (u.i_ly  < 1 || u.i_ly  > (int)BlockMaterialCache.size()           ||
                u.i_sly < 1 || u.i_sly > (int)BlockMaterialCache[u.i_ly-1].size() ||
                u.i_blk < 1 || u.i_blk > (int)BlockMaterialCache[u.i_ly-1][u.i_sly-1].size()) {
                std::cerr << "[SetBlockEYield] Out-of-range index ("
                          << u.i_ly << "," << u.i_sly << "," << u.i_blk << ") — skipping.\n";
                continue;
            }
            SetBlockEYield(u.i_ly, u.i_sly, u.i_blk, u.value);
        }
    }

    // Sublayer-level setters — update all blocks in the sublayer, mark both flags
    void Earth::SetSublayerDensity(int i_ly, int i_sly, double density_) {
        for (auto& b : BlockMaterialCache[i_ly-1][i_sly-1]) {
            b.density           = density_;
            b.isModified        = true;
            b.isSubLayerModified = true;
        }
    }

    void Earth::SetSublayerEYield(int i_ly, int i_sly, double eYield_) {
        for (auto& b : BlockMaterialCache[i_ly-1][i_sly-1]) {
            b.eYield            = eYield_;
            b.isModified        = true;
            b.isSubLayerModified = true;
        }
    }

    const BlockMatInfo& Earth::GetBlockMatInfo(int i_ly, int i_sly, int i_blk) const {
        return BlockMaterialCache[i_ly-1][i_sly-1][i_blk-1];
    }

    const std::vector<std::vector<std::vector<BlockMatInfo>>>& Earth::GetBlockMaterialCache() const {
        return BlockMaterialCache;
    }

    const std::vector<std::vector<std::vector<BlockMatInfo>>>& Earth::GetBlockMaterialCacheOriginal() const {
        return BlockMaterialCacheOriginal;
    }

    void Earth::StatusCacheOriginal() const {
        if (BlockMaterialCacheOriginal.empty()) {
            std::cout << "[StatusCacheOriginal] Cache is empty — call BuiltEarth() first.\n";
            return;
        }
        std::cout << "Total Major Layers in model = " << BlockMaterialCacheOriginal.size() << "\n";
        for (size_t i = 0; i < BlockMaterialCacheOriginal.size(); ++i) {
            int i_ly = static_cast<int>(i) + 1;
            std::cout << "  Layer " << i_ly
                      << ": Total number of Sublayers: " << BlockMaterialCacheOriginal[i].size() << "\n";
            for (size_t j = 0; j < BlockMaterialCacheOriginal[i].size(); ++j) {
                int i_sly = static_cast<int>(j) + 1;
                std::cout << "    Sublayer_" << i_ly << "_" << i_sly
                          << ": Total number of blocks: " << BlockMaterialCacheOriginal[i][j].size() << "\n";
            }
        }
    }

    void Earth::ForcedMaterialCache(std::vector<std::vector<std::vector<BlockMatInfo>>> NewBlockMaterialCache) {
        if (NewBlockMaterialCache.size() != BlockMaterialCacheOriginal.size()) {
            std::cerr << "[ForcedMaterialCache] Warning: layer count mismatch — "
                      << "provided " << NewBlockMaterialCache.size()
                      << " layers, model has " << BlockMaterialCacheOriginal.size() << ".\n";
        }
        BlockMaterialCache = std::move(NewBlockMaterialCache);
    }

    void Earth::SetDetectorLocation(double lon, double lat, double R_det) {
        if (!earthPaths) {
            std::cerr << "[Earth::SetDetectorLocation] Call BuiltEarth() before setting detector location.\n";
            return;
        }
        earthPaths->SetDetectorLocation(lon, lat, R_det);
    }

    std::vector<PathSegment> Earth::SetEarthPath(double th_, double phi_, bool radians) {
        if (BlockMaterialCache.empty()) {
            std::cerr << "[Earth::SetEarthPath] BlockMaterialCache is empty — call BuiltEarth() first.\n";
            return {};
        }
        if (!earthPaths) {
            std::cerr << "[Earth::SetEarthPath] EarthPaths not initialized — call BuiltEarth() first.\n";
            return {};
        }

        earthPaths->GetSegments(th_, phi_, radians);
        const std::vector<PathSegment>& geom_segs = earthPaths->GetPathSegments();

        std::vector<PathSegment> result;
        result.reserve(geom_segs.size());

        for (const PathSegment& geo_seg : geom_segs) {
            const BlockMatInfo& mat =
                BlockMaterialCache[geo_seg.i_ly  - 1]
                                  [geo_seg.i_sly - 1]
                                  [geo_seg.i_blk - 1];

            PathSegment seg = geo_seg;
            seg.density     = mat.density;
            seg.eyield      = mat.eYield;

            result.push_back(std::move(seg));
        }

        return result;
    }


    PropaPath Earth::BuildPropaPath(double th, double phi, bool radians) {
        const std::vector<PathSegment> segs = SetEarthPath(th, phi, radians);

        PropaPath out;
        out.costh   = std::cos(radians ? th : th * TMath::Pi() / 180.0);
        out.azimuth = phi;
        out.eyield.reserve(segs.size());
        out.density.reserve(segs.size());
        out.ne.reserve(segs.size());
        out.baseline.reserve(segs.size());

        for (const PathSegment& s : segs) {
            out.eyield.push_back(s.eyield);
            out.density.push_back(s.density);
            out.ne.push_back(s.density * s.eyield);
            out.baseline.push_back(s.distance);
        }
        return out;
    }
 
    // Populates MultiGeomPathCache with geometry-only segments; MultiGeomPathCache[i]
    // corresponds to th_grid[i] at the given phi.
    std::vector<PropaPath> Earth::GetEarthPath(
        const std::vector<double>& th_grid,
        double phi,
        bool   radians)
    {
        std::cout << "[Earth::GetEarthPath] Building Earth paths for " << th_grid.size() << " angles.\n";
        MultiGeomPathCache.clear();
        MultiGeomPathCache.resize(th_grid.size());
        MultiGeomAngleCache.clear();
        MultiGeomAngleCache.resize(th_grid.size());
        std::cout << "[Earth::GetEarthPath] Free cache  paths for " << th_grid.size() << " angles.\n";
        std::vector<PropaPath> result;
        result.reserve(th_grid.size());

        for (size_t nth = 0; nth < th_grid.size(); ++nth) {
            std::cout << "Building PropaPaths for " << nth << " angles.\n";
            result.push_back(BuildPropaPath(th_grid[nth], phi, radians));
            std::cout << "Finished building PropaPath for " << nth << " angles.\n";
            MultiGeomPathCache[nth] = earthPaths->GetPathSegments();
            double costh = std::cos(radians ? th_grid[nth] : th_grid[nth] * TMath::Pi() / 180.0);
            MultiGeomAngleCache[nth] = {costh, phi};
        }

         std::cout << "[Earth::GetEarthPath] Finished building paths for " << result.size() << " angles.\n";
        return result;
    }

    // Re-applies the current BlockMaterialCache to the cached geometry segments.
    // Returns one PropaPath per angle, identical in structure to GetEarthPath output.
    // Must be called after GetEarthPath has populated MultiGeomPathCache.
    std::vector<PropaPath> Earth::UpdateEarthPath()
    {
        if (MultiGeomPathCache.empty()) {
            std::cerr << "[UpdateEarthPath] GeomCache is empty — call GetEarthPath first.\n";
            return {};
        }
        if (BlockMaterialCache.empty()) {
            std::cerr << "[UpdateEarthPath] BlockMaterialCache is empty — call BuiltEarth() first.\n";
            return {};
        }

        std::vector<PropaPath> result;
        result.reserve(MultiGeomPathCache.size());

        for (size_t nth = 0; nth < MultiGeomPathCache.size(); ++nth) {
            PropaPath pp;
            pp.costh   = MultiGeomAngleCache[nth].first;
            pp.azimuth = MultiGeomAngleCache[nth].second;
            const auto& segs = MultiGeomPathCache[nth];
            pp.eyield.reserve(segs.size());
            pp.density.reserve(segs.size());
            pp.ne.reserve(segs.size());
            pp.baseline.reserve(segs.size());

            for (const PathSegment& seg : segs) {
                const BlockMatInfo& mat =
                    BlockMaterialCache[seg.i_ly  - 1]
                                      [seg.i_sly - 1]
                                      [seg.i_blk - 1];
                pp.eyield.push_back(mat.eYield);
                pp.density.push_back(mat.density);
                pp.ne.push_back(mat.density * mat.eYield);
                pp.baseline.push_back(seg.distance);
            }
            result.push_back(std::move(pp));
        }
        return result;
    }

    // Re-applies BlockMaterialCacheOriginal (unmodified PREM) to the cached geometry segments.
    // Returns one PropaPath per angle. Must be called after GetEarthPath.
    std::vector<PropaPath> Earth::OriginalEarthPath()
    {
        if (MultiGeomPathCache.empty()) {
            std::cerr << "[OriginalEarthPath] GeomCache is empty — call GetEarthPath first.\n";
            return {};
        }
        if (BlockMaterialCacheOriginal.empty()) {
            std::cerr << "[OriginalEarthPath] BlockMaterialCacheOriginal is empty — call BuiltEarth() first.\n";
            return {};
        }

        std::vector<PropaPath> result;
        result.reserve(MultiGeomPathCache.size());

        for (size_t nth = 0; nth < MultiGeomPathCache.size(); ++nth) {
            PropaPath pp;
            pp.costh   = MultiGeomAngleCache[nth].first;
            pp.azimuth = MultiGeomAngleCache[nth].second;
            const auto& segs = MultiGeomPathCache[nth];
            pp.eyield.reserve(segs.size());
            pp.density.reserve(segs.size());
            pp.ne.reserve(segs.size());
            pp.baseline.reserve(segs.size());

            for (const PathSegment& seg : segs) {
                const BlockMatInfo& mat =
                    BlockMaterialCacheOriginal[seg.i_ly  - 1]
                                              [seg.i_sly - 1]
                                              [seg.i_blk - 1];
                pp.eyield.push_back(mat.eYield);
                pp.density.push_back(mat.density);
                pp.ne.push_back(mat.density * mat.eYield);
                pp.baseline.push_back(seg.distance);
            }
            result.push_back(std::move(pp));
        }
        return result;
    }


};
