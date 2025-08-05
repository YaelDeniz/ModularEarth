///////////////////////////////////////////////////////////////////////////////
/// \struct OscProb::TesselatedEarth
///
/// \brief A class that generates a tesselated Earth model
///
/// This class generates a modular 3D Earth model, consistend in
/// Tesellated Speherical Shells representing Earths layers, so that
/// Each layer can be modifiend to include heterogeneities (e.g., Mantle Plumes, 
/// Slabs, Hydrous regions, LLVPs,ULVZ, etc.). This class uses the tracking 
/// Capabilities of TGeoManager to generates neutrino paths through the Earth.
/// The Density, Z/A and size of each layer's building volume can be adjusted anytime.
///
/// CERN ROOT uses the rigth handed cartesian coordinate system
/// Convetion used for coordinate system:  +z = up, +x= greenwhich, +y = east lon
///
/// \author Yael Deniz: deni5876@vandals.uidaho.edu
///////////////////////////////////////////////////////////////////////////////
///
///
///
/// Should the class has a constructer in which a volume, and medium has a default value or user must define those first?
/// Destructors and deletes of TGeoManagers between custoim classes?
/// Using not even divisible width->Error
/// Definition based on widths and number of binnings
/// Check for each input
/// Function-> modify a bins within a range in azimuth, zenith, and depth : Feed back about new bounds 
/// Different binnning in different sublayer
/// Developers notes:
///             1) Materials and media are tightly coupled with TGeoVolume at creation time and are not meant to be changed afterward
///             2) ROOT doesn't allow you to "edit" nodes inside TGeoVolumeAssembly directly.
///
///
///
///
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>

#include "TGeoManager.h"
//#include <TGeoVolumeAssembly.h>
#include <TGeoMaterial.h>
#include <TGeoMedium.h>
#include <TGeoVolume.h>
#include <TGeoShape.h>
#include <TGeoMatrix.h>
#include <TH1F.h>
#include <TPolyLine3D.h>
#include <TPolyMarker3D.h>
#include <TPolyLine3D.h>
#include <TView.h>
//#include <TGeoMarker.h>

class Block {
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// This class generates the "building blocks" in for the modular Earth
    /// Each block is a shell sergement defined by:
    ///     rmin-Inner Radius of the shell
    ///     rmax-outer radius of the shell
    ///     th1/th2 (phi1/phi2) - Blocks angular limits
    /// The material of each block can be modified, from a sublayer, user can access each block and modify it
    /// Additional sub volumens can be added into each block
    ///
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    private: TGeoVolume * block = nullptr; // Owns the shape
    TGeoMaterial * blockMaterial = nullptr;
    TGeoMedium * blockMedium = nullptr;

    double rmin = 0,
    rmax = 1; // Inner (rmin) and outer (rmax) radius of the block
    double thetamin = 0,
    thetamax = 45; // Block is constructed for \theta \in [thetamin, thetamax]
    double phimin = 0,
    phimax = 25; // Block is constructed for \theta \in [phimin, phimax]
    int blockID = 1; //Each block has a unique index
    double atomicMass = 0; // Atomic Mass (A) of volume material
    double atomicNumber = 0; // Atomic Number (Z) of volume material
    double density = 0; // Density (ρ) of volume medium
    bool modified = false;
    bool FullSubLayerModified = false;
    std::string materialName = "Vacuum"; // Name of block material
    int colorIndex = 1;

    public: void WasModified(bool status) {
        modified = status;
    }
    void WasSubLayerModified(bool status) {
        FullSubLayerModified = status;
    }
    // Default Constructor (Uses the default variable values defined in private)
    Block() =
    default;

    // Return Block ID
    int GetBlockID() const {
        return blockID;
    }

    // Set the block 
    void SetBlockGeometry(double r1, double r2,
        double theta1, double theta2,
        double phi1, double phi2,
        int id) {

        // Error for angular units -> Must be in degrees, non negative and for zenith from 0-180
        rmin = r1;
        rmax = r2;
        thetamin = theta1;
        thetamax = theta2;
        phimin = phi1;
        phimax = phi2;
        blockID = id;
    }

    // Define material and medium
    void DefineBlockMaterial(const std::string & name, double A, double Z, double rho) {
        materialName = name;
        atomicMass = A;
        atomicNumber = Z;
        density = rho;

        blockMaterial = new TGeoMaterial(name.c_str(), A, Z, rho);
        blockMedium = new TGeoMedium(name.c_str(), blockID, blockMaterial);
    }

    // Build geometry volume
    void BuildBlock(int colorid = 1) {
        if (!blockMedium) {
            std::cerr << "ERROR: Medium not defined before building a block!" << std::endl;
            return;
        }

        if (!gGeoManager) {
            std::cerr << "ERROR: gGeoManager is nullptr. Initialize ROOT geometry before building blocks." << std::endl;
            return;
        }

        TGeoShape * shape = new TGeoSphere(Form("BlockShape_%d", blockID),
            rmin, rmax, thetamin, thetamax, phimin, phimax);
        block = new TGeoVolume(Form("BlockVolume_%d", blockID), shape, blockMedium);
        //block->SetLineColor(kBlue);     // edge color
        block -> SetVisibility(kFALSE);
        //block->SetFillColor(kYellow);  // face color 
        if (modified) {
            block -> SetLineColor(kRed);
            block -> SetVisibility(kFALSE);
            block -> SetFillColor(kRed);
        } // Distinguishable colors
        //else{block->SetLineColor(kGreen + (blockID % 5));}
        if (FullSubLayerModified) {
            block -> SetLineColor(kGreen);
        }

    }

    //Check if block is build properly
    bool IsValid() const {
        return (block != nullptr &&
            blockMaterial != nullptr &&
            blockMedium != nullptr &&
            rmax > rmin &&
            thetamax > thetamin &&
            phimax > phimin &&
            density > 0 &&
            atomicMass > 0 &&
            atomicNumber > 0);
    }

    //Get Summary of Block -> Switch to extract information from TGeoManager Itself
    void BlockSummary() const {
        std::cout << "=== Block Summary ===" << std::endl;
        std::cout << "  Block ID:        " << blockID << std::endl;
        std::cout << "  Block Name:      " << (block ? block -> GetName() : "N/A") << std::endl;

        if (block && block -> GetShape() -> IsA() == TGeoSphere::Class()) {
            TGeoSphere * sphere = dynamic_cast < TGeoSphere * > (block -> GetShape());
            std::cout << "  Shape:           TGeoSphere" << std::endl;
            std::cout << "    rmin:          " << sphere -> GetRmin() << std::endl;
            std::cout << "    rmax:          " << sphere -> GetRmax() << std::endl;
            std::cout << "    thetamin:      " << sphere -> GetTheta1() << std::endl;
            std::cout << "    thetamax:      " << sphere -> GetTheta2() << std::endl;
            std::cout << "    phimin:        " << sphere -> GetPhi1() << std::endl;
            std::cout << "    phimax:        " << sphere -> GetPhi2() << std::endl;
        } else {
            std::cout << "  Shape:           Not a TGeoSphere or invalid" << std::endl;
        }

        if (blockMedium && blockMedium -> GetMaterial()) {
            TGeoMaterial * mat = blockMedium -> GetMaterial();
            std::cout << std::boolalpha << "  Modified: " << modified << std::endl;
            std::cout << "  Material:        " << mat -> GetName() << std::endl;
            std::cout << "  Atomic Mass A: " << mat -> GetA() << std::endl;
            std::cout << "  Atomic Num  Z: " << mat -> GetZ() << std::endl;
            std::cout << "  Density      :  " << mat -> GetDensity() << " g/cm^3" << std::endl;
        } else {
            std::cout << "  Material:        Undefined" << std::endl;
        }

        std::cout << "  Pointers:" << std::endl;
        std::cout << "    Volume:        " << block << std::endl;
        std::cout << "    Material:      " << blockMaterial << std::endl;
        std::cout << "    Medium:        " << blockMedium << std::endl;
        std::cout << "=====================" << std::endl;
    }

    // Accessor for attaching to a layer
    TGeoVolume * GetVolume() const {
        return block;
    }
};

class Sublayer {
    /////////////////////////////////////////////////////////////////////////////////////
    /// class Sublayer
    /// Represents a spherical shell segment composed of multiple Blocks (TGeoVolumes)
    /// - Can be initialized homogeneously with a default material
    /// - Allows updating material properties for specific blocks
    /// - Supports printing detailed information and geometry inspection
    /// - Rebuilds ROOT geometry assemblies only when requested
    /////////////////////////////////////////////////////////////////////////////////////

    private: TGeoVolumeAssembly * SubLayer = nullptr;
    std::vector < Block > blocks;
    int SubLayerID = 1;
    std::string SubLayerName;
    std::string SubLayerMaterialName = "Iron";
    double SubLayerA = 55.85;
    double SubLayerZ = 26;
    double SubLayerDensity = 7.87;
    bool BlocksUpdate = false;
    bool FullUpdate = false;

    double Nphi = 4;
    double Nth = 3;
    double dphi = 90;
    double dth = 60;

    // Internal function to build(rebuild) the geometry assembly
    void BuildAssembly() {
        if (SubLayer) delete SubLayer;
        SubLayer = new TGeoVolumeAssembly(SubLayerName.c_str());

        static TGeoTranslation * sharedTranslation = new TGeoTranslation(0, 0, 0);
        for (const auto & b: blocks) {
            if (b.GetVolume() == nullptr) {
                std::cerr << "Block is empty volume!\n";
            } else if (!b.IsValid()) {
                std::cerr << "Error: Invalid block detected. Blocks were not built properly\n";
                b.BlockSummary();
            }
            SubLayer -> AddNode(b.GetVolume(), b.GetBlockID(), sharedTranslation);
        }
    }

    public: Sublayer() =
        default;

    // Set ID and update name
    void SetSubLayerID(int id) {
        SubLayerID = id;
        SubLayerName = Form("SubLayer_%d", SubLayerID);
    }

    void SetSubLayerMaterial(const std::string & name, double A, double Z, double density) {
        SubLayerMaterialName = name;
        SubLayerA = A;
        SubLayerZ = Z;
        SubLayerDensity = density;
    }

    void CreateSubLayerBlocks(double r1, double r2, double dphi, double dth) {
        int Nphi = 360 / dphi;
        int Nth = 180 / dth;
        int blockID = 1;

        for (int i = 0; i < Nth; ++i) {
            double th1 = i * dth;
            double th2 = (i + 1) * dth;

            for (int j = 0; j < Nphi; ++j) {
                double phi1 = j * dphi;
                double phi2 = (j + 1) * dphi;

                Block block;
                block.SetBlockGeometry(r1, r2, th1, th2, phi1, phi2, blockID);
                block.DefineBlockMaterial(SubLayerMaterialName, SubLayerA, SubLayerZ, SubLayerDensity);
                block.BuildBlock(SubLayerID); // color hint from ID
                blocks.push_back(block);
                blockID++;
            }
        }
    }

    void UpdateBlockMaterial(int id,
        const std::string & NewName, double NewA, double NewZ, double NewDensity) {
        if (id < 1 || id > static_cast < int > (blocks.size())) {
            std::cerr << "Block index out of bounds. Index must be between 1 and " << blocks.size() << std::endl;
            return;
        }

        Block & modifiedBlock = blocks[id - 1];
        modifiedBlock.DefineBlockMaterial(NewName, NewA, NewZ, NewDensity);
        modifiedBlock.WasModified(true);
        modifiedBlock.BuildBlock();
        BlocksUpdate = true;
    }

    void UpdateMultipleBlocksMaterial(const std::vector < int > & ids,
        const std::string & NewName,
            double NewA, double NewZ, double NewDensity) {
        for (int id: ids) {
            if (id < 1 || id > static_cast < int > (blocks.size())) {
                std::cerr << "Block ID " << id << " is out of bounds! Skipping...\n";
                continue;
            }

            Block & modifiedBlock = blocks[id - 1];
            modifiedBlock.DefineBlockMaterial(NewName, NewA, NewZ, NewDensity);
            modifiedBlock.WasModified(true);
            modifiedBlock.BuildBlock();
        }
        BlocksUpdate = true;
    }

    void UpdateAllBlocksMaterial(const std::string & NewName, double NewA, double NewZ, double NewDensity) {
        for (Block & modifiedBlock: blocks) {
            modifiedBlock.DefineBlockMaterial(NewName, NewA, NewZ, NewDensity);
            modifiedBlock.WasSubLayerModified(true);
            modifiedBlock.BuildBlock();
        }
        FullUpdate = true;
    }

    void BuildSubLayer() {
        BuildAssembly();
    }

    TGeoVolumeAssembly * GetSubLayerVolume() const {
        return SubLayer;
    }

    void GetBlockSummary(int id) {
        if (id >= 1 && id <= static_cast < int > (blocks.size()))
            blocks[id - 1].BlockSummary();
        else
            std::cerr << "Invalid block ID: " << id << std::endl;
    }

    void GetAllBlockSummary() {
        for (const auto & b: blocks) {
            if (!b.IsValid()) {
                std::cerr << "Error: Invalid block\n";
            }
            b.BlockSummary();
        }
    }

    void GetSubLayerSummary() {
    std::cout << "SubLayer Index: " << SubLayerID << "\n"
              << "SubLayer Name: " << SubLayerName << "\n"
              << "Number of Blocks: " << blocks.size() << "\n"
              << "Sublayer Updates: Full(" << std::boolalpha << FullUpdate
              << ") Blocks(" << BlocksUpdate << ")" << std::endl;
    }

};

struct SublayerConfig {
    double rmin; // Inner radius of sublayer
    double rmax; // Outer radius of sublayer
    double density; // g/cm³
    double atomicMass; // A
    double atomicNumber; // Z
    int layerID;
    std::string materialName; // Optional, default = "LayerMaterial"

    SublayerConfig(int id, double r1, double r2, double rho, double A, double Z, std::string name = "SubLayerMaterial"): layerID(id), rmin(r1), rmax(r2), density(rho), atomicMass(A), atomicNumber(Z), materialName(name) {}
};

class Layer {
    /////////////////////////////////////////////////////////////////////////////////////
    ///
    /// class Layer
    ///
    /// Represents a modular spherical shell composed of multiple Sublayers (TGeoVolumeAssemblies),
    /// each made of multiple Blocks. This class:
    /// - Initializes from input configuration (SublayerConfig)
    /// - Supports block-by-block or whole-sublayer material modification
    /// - Manages geometry rebuilding after updates
    /// - Interfaces with ROOT’s TGeoManager
    ///
    /////////////////////////////////////////////////////////////////////////////////////

    private: 
    TGeoVolumeAssembly * layerVolume = nullptr;
    std::vector < Sublayer > sublayers;
    int layerID = 1;

    std::string layerTitle = "EarthLayer";
    std::string layerName;

    double dphi = 90.0;
    double dth = 60.0;

    public:

    // === Constructor ===
     Layer(std::string name = "DefaultName", int id = 1, double dthsize = 60.0, double dphisize = 90.0): layerTitle(name),
    layerID(id),
    dphi(dphisize),
    dth(dthsize) {
        layerName = Form("%s_%d", layerTitle.c_str(), layerID);
    }

    // === Geometry Build ===
    void SetSubLayers(const std::vector < SublayerConfig > & configs) {
        layerVolume = new TGeoVolumeAssembly(layerName.c_str());

        for (const auto & cfg: configs) {
            Sublayer sub;
            sub.SetSubLayerID(cfg.layerID);
            sub.SetSubLayerMaterial(cfg.materialName, cfg.atomicMass, cfg.atomicNumber, cfg.density);
            sub.CreateSubLayerBlocks(cfg.rmin, cfg.rmax, dphi, dth);
            sublayers.push_back(sub);
        }
    }


    void BuildLayer() {
        int id = 1;
        for (auto & sl: sublayers) {
            sl.BuildSubLayer();
            layerVolume -> AddNode(sl.GetSubLayerVolume(), id++);
        }
    }

    // === Block Modification ===

    void UpdateBlocksInSubLayer(size_t sublayerIndex,
        const std::vector < int > & blockIDs,
            const std::string & newName,
                const std::vector < double > & newA,
                    const std::vector < double > & newZ,
                        const std::vector < double > & newDensity) {
        if (sublayerIndex < 1 || sublayerIndex > sublayers.size()) {
            std::cerr << "Sublayer index " << sublayerIndex << " is out of bounds! Skipping...\n";
            return;
        }

        if (blockIDs.size() != newA.size() ||
            blockIDs.size() != newZ.size() ||
            blockIDs.size() != newDensity.size()) {
            std::cerr << "Inconsistent sizes in block update vectors.\n";
            return;
        }

        Sublayer & sl = sublayers[sublayerIndex - 1];
        bool anyModified = false;

        for (size_t i = 0; i < blockIDs.size(); ++i) {
            sl.UpdateBlockMaterial(blockIDs[i], newName, newA[i], newZ[i], newDensity[i]);
            std::cout << "Block " << blockIDs[i] <<
                " in Sublayer " << sublayerIndex <<
                " updated with material: " << newName << "\n";
            anyModified = true;
        }

        if (anyModified) {
            sl.BuildSubLayer(); // Rebuild only if changes were made
        } else {
            std::cerr << "No valid blocks were modified.\n";
        }
    }

    // === Full Sublayer Modification ===
    void UpdateSublayer(size_t sublayerIndex,
        const std::string & newName,
            double newA,
            double newZ,
            double newDensity) {
        if (sublayerIndex < 1 || sublayerIndex > sublayers.size()) {
            std::cerr << "Invalid sublayer index: " << sublayerIndex << "\n";
            return;
        }

        Sublayer & sl = sublayers[sublayerIndex - 1];
        sl.UpdateAllBlocksMaterial(newName, newA, newZ, newDensity);
        sl.BuildSubLayer();
    }

    // === Summary Methods ===

    void SubLayerSummary(size_t sublayerIndex) const {
        if (sublayerIndex < 1 || sublayerIndex > sublayers.size()) {
            std::cerr << "Invalid sublayer index.\n";
            return;
        }
        
        Sublayer sl = sublayers[sublayerIndex - 1];
        sl.GetAllBlockSummary();
    }

    void BlockSummary(size_t sublayerIndex, int blockIndex) const {
        if (sublayerIndex < 1 || sublayerIndex > sublayers.size()) {
            std::cerr << "Invalid sublayer index.\n";
            return;
        }
        
        Sublayer  sl = sublayers[sublayerIndex - 1];
        sl.GetBlockSummary(blockIndex);
    }

    void LayerSummary() const {
        std::cout << "=== Layer Summary: " << layerName << " ===\n";
        std::cout << " Number of Sublayers:" << sublayers.size() << std::endl;
    }

    // === Return Volume ===
    TGeoVolumeAssembly * GetLayerVolume() const {
        return layerVolume;
    }
};

struct LayerConfig 
{
    std::string layername;
    int layerindex; 
    double zenithbinning, azimuthbinning;
    std::vector<SublayerConfig> sublayersconfig; // Sublayers composing this Earth layer
    //std::vector<SublayersUpdates> updates;

    LayerConfig() = default; // Useful for STL containers or deferred initialization

    LayerConfig(const std::string& name, int id, double dth, double dphi, const std::vector<SublayerConfig>& configs)
        : layername(name), layerindex(id), zenithbinning(dth), azimuthbinning(dphi), sublayersconfig(configs) {}
};

struct SublayersUpdates
{
    int LayerIndex;
    int SubLayerIndex;
    std::vector<int> blockids;
    std::string NewMaterialName;
    std::vector<double> blocks_As;
    std::vector<double> blocks_Zs;
    std::vector<double> blocks_densities;

    // Correct constructor name
    SublayersUpdates(int lidx, int slidx,
                     const std::vector<int>& ids,
                     const std::string& name,
                     const std::vector<double>& As,
                     const std::vector<double>& Zs,
                     const std::vector<double>& densities)
        : LayerIndex(lidx), SubLayerIndex(slidx),
          blockids(ids), NewMaterialName(name),
          blocks_As(As), blocks_Zs(Zs), blocks_densities(densities) {}
};


class ModularEarth {
/////////////////////////////////////////////////////////////////////////////////////////////
///
/// class ModularEarth
///
/// ModularEarth represents a full 3D, layered Earth model using ROOT's `TGeoManager`.
/// Each Layer consists of spherical shell sublayers, each discretized into blocks.
/// This class:
///   - Reads a stratified Earth model from a .txt file
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
    std::vector<SublayersUpdates> updatelist;
    std::vector<Layer> EarthLayers;

    // Global Earth model defaults
    double zenithbinning = 10.0;
    double azimuthbinning = 10.0;
    double a_mass = 1.0;

    TPolyMarker3D *NuDetMarkers = nullptr;
     TPolyLine3D* nuLine = nullptr;

public:
    ModularEarth() {
        geoManager = new TGeoManager("EarthGeo", "Modular Earth geometry");
        matVac = new TGeoMaterial("Vacuum", 0, 0, 0);
        medVac = new TGeoMedium("Vacuum", 1, matVac);
        top = new TGeoVolume("TopBox", new TGeoBBox("Box", 5000, 5000, 5000), medVac);
        geoManager->SetTopVolume(top);
    }

    // Parse Earth model from file (rmax, ρ, Z/A, index)
    void SetEarthModel(const std::string& modelpath) {
        std::ifstream infile(modelpath);
        if (!infile) {
            std::cerr << "ERROR: Could not open model file: " << modelpath << std::endl;
            return;
        }

        std::string line;
        double radiusprev = 0.0;
        int indexprev = -1;
        int slindex = 1;

        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            double radius, density, z_a;
            int index;

            if (!(iss >> radius >> density >> z_a >> index)) {
                std::cerr << "WARNING: Invalid line skipped: " << line << std::endl;
                continue;
            }

            if (index != indexprev && indexprev != -1) {
                layersconfigs.push_back({"Layer", indexprev, zenithbinning, azimuthbinning, sublayersconfigs});
                sublayersconfigs.clear();
                slindex = 1;
            }

            sublayersconfigs.push_back({slindex, radiusprev, radius, density, a_mass, z_a, "SublayerMaterial"});
            radiusprev = radius;
            indexprev = index;
            slindex++;
        }

        if (!sublayersconfigs.empty()) {
            layersconfigs.push_back({"Layer", indexprev, zenithbinning, azimuthbinning, sublayersconfigs});
        }
    }

    // Change angular resolution (before building Earth)
    void SetEarthBinning(double dth, double dphi) {
        zenithbinning = dth;
        azimuthbinning = dphi;
    }

    // Provide block/sublayer material updates before building
    void UpdateLayers(const std::vector<SublayersUpdates>& sublayerupdates) {
        updatelist = sublayerupdates;
    }

    // Build full Earth volume with updates
    void BuildEarth() {
        EarthLayers.clear(); // Clear for rebuild

        for (size_t i = 0; i < layersconfigs.size(); ++i) {
            const auto& config = layersconfigs[i];
            Layer ithlayer(config.layername, config.layerindex, config.zenithbinning, config.azimuthbinning);
            ithlayer.SetSubLayers(config.sublayersconfig);

            for (const auto& update : updatelist) {
                if (update.LayerIndex == static_cast<int>(i)) {
                    ithlayer.UpdateBlocksInSubLayer(update.SubLayerIndex,
                                                    update.blockids,
                                                    update.NewMaterialName,
                                                    update.blocks_As,
                                                    update.blocks_Zs,
                                                    update.blocks_densities);
                }
            }

            ithlayer.BuildLayer();
            EarthLayers.push_back(ithlayer);
            top->AddNode(ithlayer.GetLayerVolume(), i, new TGeoTranslation(0, 0, 0));
        }

        geoManager->CloseGeometry();
    }

    // === Access & Inspection ===

    TGeoManager* GetGeoManager() const { return geoManager; }
    TGeoVolume* GetTopVolume() const { return top; }

    void Visualize() const {
        if (top) 
        {
            top->Draw("gl");
            NuDetMarkers->Draw("same");
            nuLine->Draw("same");

            TView *view = gPad->GetView();
            view->ShowAxis();
        }
    }

    void GetSubLayerInfo(int lindex, int slindex) {
        if (lindex < EarthLayers.size())
            EarthLayers[lindex].SubLayerSummary(slindex);
        else
            std::cerr << "Invalid Layer index.\n";
    }

    void GetBlocksInfo(int lindex, int slindex, int bindex) {
        if (lindex < EarthLayers.size())
            EarthLayers[lindex].BlockSummary(slindex, bindex);
        else
            std::cerr << "Invalid Layer index.\n";
    }

    void GetModelInfo() const {
        for (size_t i = 0; i < layersconfigs.size(); ++i) {
            std::cout << "\n--- Layer index: " << i << " ---\n";

            for (const auto& slcfg : layersconfigs[i].sublayersconfig) {
                std::cout << " Sublayer " << slcfg.layerID
                          << " | rmin: " << slcfg.rmin
                          << " | rmax: " << slcfg.rmax
                          << " | ρ: "   << slcfg.density
                          << " | Z/A: " << slcfg.atomicMass << "\n";
            }
        }
    }

    // TODO: Add methods like:
    // void TrackParticlePath(...)
    // void ExportToROOT(...)

    // Detector location in global coordinates
    double x_det, y_det, z_det;

    // Detector local coordinate system unit vectors
    TVector3 x_hat, y_hat, z_hat;

    // ROOT globals expected
    //extern TGeoManager* geoManager;
    //extern TGeoMedium* medVac;
    //extern TGeoVolume* top;

    void SetDetectorLocation(double lon, double lat, double radius) {
        double lat_rad = lat * M_PI / 180.0;
        double lon_rad = lon * M_PI / 180.0;

        // Compute detector location in RHCC
        x_det = radius * cos(lat_rad) * cos(lon_rad);
        y_det = radius * cos(lat_rad) * sin(lon_rad);
        z_det = radius * sin(lat_rad);

        TVector3 r_det(x_det, y_det, z_det);  // Vector from Earth's center to detector

        // Detector local +z axis is outward radial direction
        z_hat = r_det.Unit();

        // Handle poles: cross with X instead of Z
        if (fabs(z_hat.Z()) > 0.999) {
            x_hat = TVector3(1, 0, 0).Cross(z_hat).Unit();
        } else {
            x_hat = TVector3(0, 0, 1).Cross(z_hat).Unit();  // Local East
        }

        y_hat = z_hat.Cross(x_hat).Unit();  // Local North
    }

    void GetEarthPath(double zenith, double azimuth) {
        // Convert to radians
        double zen_rad = (180-zenith) * M_PI / 180.0;
        double az_rad  = azimuth * M_PI / 180.0;

        // Neutrino direction in detector frame
        TVector3 dir_local(
            sin(zen_rad) * cos(az_rad),
            sin(zen_rad) * sin(az_rad),
            cos(zen_rad)
        );

        // Convert to global direction
        TVector3 dir_global =
            x_hat * dir_local.X() +
            y_hat * dir_local.Y() +
            z_hat * dir_local.Z();

        TVector3 n_dir = dir_global.Unit();  // Normalize
        TVector3 r0(x_det, y_det, z_det);    // Detector location in RHCC

        // Find the entry point: intersection with R=6386 km (atmosphere height)
        double R = 6386.0;  // km
        double a = 1.0;
        double b = -2.0 * r0.Dot(n_dir);
        double c = r0.Mag2() - R * R;

        double discriminant = b * b - 4 * a * c;
        if (discriminant < 0) {
            std::cerr << "No intersection with atmospheric sphere!\n";
            return;
        }

        double sqrt_disc = sqrt(discriminant);
        double t1 = (-b + sqrt_disc) / (2.0 * a);
        double t2 = (-b - sqrt_disc) / (2.0 * a);

        // Choose the farther point from the detector (source position)
        double t_entry = std::max(t1, t2);
        TVector3 r_src = r0 - t_entry * n_dir;

        // Visualization
        nuLine = new TPolyLine3D(2);
        nuLine->SetPoint(0, r_src.X(), r_src.Y(), r_src.Z());
        nuLine->SetPoint(1, r0.X(),    r0.Y(),    r0.Z());
        nuLine->SetLineColor(kBlue);
        nuLine->SetLineWidth(2);
        //gGeoManager->GetListOfPrimitives()->Add(nuLine);

        //
        NuDetMarkers= new TPolyMarker3D(2,2); //Markers indicating DetLoc and Inical Neutrino
        NuDetMarkers->SetPoint( 0 , r0.X(),    r0.Y(),    r0.Z()); //Detector
        NuDetMarkers->SetPoint( 1 ,r_src.X(),r_src.Y(),r_src.Z()); //Incoming Neutrino
        NuDetMarkers->SetMarkerColor(6);
        NuDetMarkers->SetMarkerSize(3);

        std::cout << "Distance test in the atmosphere" << sqrt( r_src.Mag2() ) << std::endl;
        std::cout << "Distance test detector location" << sqrt( r0.Mag2() ) << std::endl;
        std::cout << "Distance test normal vector" << sqrt( n_dir.Mag2() ) << std::endl;

        std::cout << "Test number: " << sqrt(r_src.X()*r_src.X() + r_src.Y()*r_src.Y() + r_src.Z()*r_src.Z())  << std::endl;
        
        //Tracking
        double start_point[3] = {r_src.X(), r_src.Y(), r_src.Z()};
        double direction[3] = {n_dir.X(), n_dir.Y(), n_dir.Z()};
        //TGeoNavigator* navigator = gGeoManager->GetCurrentNavigator();
        //navigator->SetCurrentPoint(start_point);    // double[3]
        //navigator->SetCurrentDirection(direction);  // double[3]
        //navigator->FindNode();                      // Enter volume

        std::cout << "Initializing Tracking" << std::endl;

        int depth = 3;


    // InitTrack does NOT automatically find the node
    gGeoManager->InitTrack(r_src.X(), r_src.Y(), r_src.Z(), n_dir.X(), n_dir.Y(), n_dir.Z());
    /// Initial point 
    std::cout << "Initial point " << std::endl;
    const char* path = gGeoManager->GetPath();
    TGeoVolume* cvolume = gGeoManager->GetCurrentVolume();
    TGeoMaterial* cmaterial = cvolume->GetMedium()->GetMaterial();
    TGeoNode* cnode = gGeoManager->GetCurrentNode();


    std::string volName = cvolume->GetName();
    std::string matName = cmaterial->GetName();

        
        double density = cmaterial->GetDensity();
        double A = cmaterial->GetA();
        double Z = cmaterial->GetZ();
        double ZoverA = (A > 0) ? Z / A : 0.0;

        //int layerID = -1, sublayerID = -1;
        //sscanf(volName.c_str(), "Layer%d_Sublayer%d", &layerID, &sublayerID);

        std::cout << std::fixed << std::setprecision(3)
                << "Step Length: " << 0 << " cm, "
                << "ρ: " << density << " g/cm³, "
                << "Z/A: " << ZoverA << ", "
                << "Material: " << matName << ", "
                << "Volume: " << volName << ", "
                << "Layer: " << ", "
                << "Sublayer: "  << std::endl;
    /////////////////////////////////////////////////////////////
     path = gGeoManager->GetPath();  // Full path like /top/Layer1/Sublayer3/Block_7
    TGeoNode* node = gGeoManager->GetCurrentNode();

    // Get the current node’s name (usually the block)
    std::string blockName = node->GetName();

    // Get the hierarchy stack of nodes
    TGeoNode* stack[100];

    std::string layerName = "";
    std::string sublayerName = "";


        // Example: depth 3 → [0]=top, [1]=Layer, [2]=Sublayer, [3]=Block
        layerName    = stack[depth - 3]->GetName();  // grandparent: layer
        sublayerName = stack[depth - 2]->GetName();  // parent: sublayer
        blockName    = stack[depth - 1]->GetName();  // child: block
   

    // Now you have all names
    std::cout << "Layer: " << layerName
            << ", Sublayer: " << sublayerName
            << ", Block: " << blockName << std::endl;
    /////////////////////////////////////////////////////////////
    

    std::cout << "First step " << std::endl;

    /// First step
    gGeoManager->FindNextBoundaryAndStep();

    double step = gGeoManager->GetStep();


     path = gGeoManager->GetPath();
     cvolume = gGeoManager->GetCurrentVolume();
     cmaterial = cvolume->GetMedium()->GetMaterial();
     cnode = gGeoManager->GetCurrentNode();


     volName = cvolume->GetName();
     matName = cmaterial->GetName();

         double length = step;
         density = cmaterial->GetDensity();
         A = cmaterial->GetA();
         Z = cmaterial->GetZ();
         ZoverA = (A > 0) ? Z / A : 0.0;

        //int layerID = -1, sublayerID = -1;
        //sscanf(volName.c_str(), "Layer%d_Sublayer%d", &layerID, &sublayerID);

        std::cout << std::fixed << std::setprecision(3)
                << "Step Length: " << length << " cm, "
                << "ρ: " << density << " g/cm³, "
                << "Z/A: " << ZoverA << ", "
                << "Material: " << matName << ", "
                << "Volume: " << volName << ", "
                << "Layer: "  << ", "
                << "Sublayer: "  << std::endl;
/////////////////////////////////////////////////////////////
 path = gGeoManager->GetPath();  // Full path like /top/Layer1/Sublayer3/Block_7
 node = gGeoManager->GetCurrentNode();

// Get the current node’s name (usually the block)
 blockName = node->GetName();

// Get the hierarchy stack of nodes
TGeoNode* stack1[100];

 layerName = "";
 sublayerName = "";


    // Example: depth 3 → [0]=top, [1]=Layer, [2]=Sublayer, [3]=Block
    layerName    = stack1[depth - 3]->GetName();  // grandparent: layer
    sublayerName = stack1[depth - 2]->GetName();  // parent: sublayer
    blockName    = stack1[depth - 1]->GetName();  // child: block


// Now you have all names
std::cout << "Layer: " << layerName
          << ", Sublayer: " << sublayerName
          << ", Block: " << blockName << std::endl;
/////////////////////////////////////////////////////////////

gGeoManager->FindNextBoundaryAndStep();

     step = gGeoManager->GetStep();


     path = gGeoManager->GetPath();
     cvolume = gGeoManager->GetCurrentVolume();
     cmaterial = cvolume->GetMedium()->GetMaterial();
     cnode = gGeoManager->GetCurrentNode();


     volName = cvolume->GetName();
     matName = cmaterial->GetName();

         length = step;
         density = cmaterial->GetDensity();
         A = cmaterial->GetA();
         Z = cmaterial->GetZ();
         ZoverA = (A > 0) ? Z / A : 0.0;

        //int layerID = -1, sublayerID = -1;
        //sscanf(volName.c_str(), "Layer%d_Sublayer%d", &layerID, &sublayerID);

        std::cout << std::fixed << std::setprecision(3)
                << "Step Length: " << length << " cm, "
                << "ρ: " << density << " g/cm³, "
                << "Z/A: " << ZoverA << ", "
                << "Material: " << matName << ", "
                << "Volume: " << volName << ", "
                << "Layer: "  << ", "
                << "Sublayer: "  << std::endl;
/////////////////////////////////////////////////////////////
 path = gGeoManager->GetPath();  // Full path like /top/Layer1/Sublayer3/Block_7
 node = gGeoManager->GetCurrentNode();

// Get the current node’s name (usually the block)
 blockName = node->GetName();

// Get the hierarchy stack of nodes
TGeoNode* stack2[100];

 layerName = "";
 sublayerName = "";


    // Example: depth 3 → [0]=top, [1]=Layer, [2]=Sublayer, [3]=Block
    layerName    = stack2[depth - 3]->GetName();  // grandparent: layer
    sublayerName = stack2[depth - 2]->GetName();  // parent: sublayer
    blockName    = stack2[depth - 1]->GetName();  // child: block


// Now you have all names
std::cout << "Layer: " << layerName
          << ", Sublayer: " << sublayerName
          << ", Block: " << blockName << std::endl;
/////////////////////////////////////////////////////////////

gGeoManager->FindNextBoundaryAndStep();

     step = gGeoManager->GetStep();


     path = gGeoManager->GetPath();
     cvolume = gGeoManager->GetCurrentVolume();
     cmaterial = cvolume->GetMedium()->GetMaterial();
     cnode = gGeoManager->GetCurrentNode();


     volName = cvolume->GetName();
     matName = cmaterial->GetName();

         length = step;
         density = cmaterial->GetDensity();
         A = cmaterial->GetA();
         Z = cmaterial->GetZ();
         ZoverA = (A > 0) ? Z / A : 0.0;

        //int layerID = -1, sublayerID = -1;
        //sscanf(volName.c_str(), "Layer%d_Sublayer%d", &layerID, &sublayerID);

        std::cout << std::fixed << std::setprecision(3)
                << "Step Length: " << length << " cm, "
                << "ρ: " << density << " g/cm³, "
                << "Z/A: " << ZoverA << ", "
                << "Material: " << matName << ", "
                << "Volume: " << volName << ", "
                << "Layer: "  << ", "
                << "Sublayer: "  << std::endl;
/////////////////////////////////////////////////////////////
 path = gGeoManager->GetPath();  // Full path like /top/Layer1/Sublayer3/Block_7
 node = gGeoManager->GetCurrentNode();

// Get the current node’s name (usually the block)
 blockName = node->GetName();

// Get the hierarchy stack of nodes
TGeoNode* stack3[100];

 layerName = "";
 sublayerName = "";


    // Example: depth 3 → [0]=top, [1]=Layer, [2]=Sublayer, [3]=Block
    layerName    = stack3[depth - 3]->GetName();  // grandparent: layer
    sublayerName = stack3[depth - 2]->GetName();  // parent: sublayer
    blockName    = stack3[depth - 1]->GetName();  // child: block


// Now you have all names
std::cout << "Layer: " << layerName
          << ", Sublayer: " << sublayerName
          << ", Block: " << blockName << std::endl;
/////////////////////////////////////////////////////////////

        //TGeoMarker* detMarker = new TGeoMarker("Detector", x_det, y_det, z_det, kRed, 2);
        //gGeoManager->GetListOfPrimitives()->Add(detMarker);

        //TGeoMarker* srcMarker = new TGeoMarker("Source", r_src.X(), r_src.Y(), r_src.Z(), kGreen+2, 2);
        //gGeoManager->GetListOfPrimitives()->Add(srcMarker);

        // (Optional) Initialize tracking from source
        // double start[3] = { r_src.X(), r_src.Y(), r_src.Z() };
        // double dir[3] = { n_dir.X(), n_dir.Y(), n_dir.Z() };
        // geoManager->InitTrack(start, dir);
    }


};


// top-level macro function

//Test for layers

void TesselatedEarth() {

    ModularEarth ToyEarth;
    ToyEarth.SetEarthBinning(30.0,60.0);
    ToyEarth.SetEarthModel("/home/ydenizhernandez/OscProb/PremTables/prem_15layers.txt");

    //== Target Sublayers for ubdates ==
    std::vector<int> blockIDs = {1,5,6,9};
    std::vector<int> blockIDs1 = {2,3,6,10};
    std::vector<int> blockIDs2 = {4,7,9,12};
    std::string newName = "UpdatedBlockConfirm";
    std::vector<double> newA = {1,1,1,1};
    std::vector<double> newZ = {1,1,1,1};
    std::vector<double> newDensity = {1,1,1,1};

    //Update list
    std::vector<SublayersUpdates> updatelist;
    
    updatelist.push_back(SublayersUpdates(
        1, 1,                        // Layer index, Sublayer index
        blockIDs,                   // Block IDs
        newName,          // New material name
        newA,         // A
        newZ,               // Z
        newDensity             // Densities
    ));

    updatelist.push_back(SublayersUpdates(
        2, 2,                        // Layer index, Sublayer index
        blockIDs1,                   // Block IDs
        newName,          // New material name
        newA,         // A
        newZ,               // Z
        newDensity             // Densities
    ));

    updatelist.push_back(SublayersUpdates(
        1, 3,                        // Layer index, Sublayer index
        blockIDs2,                   // Block IDs
        newName,          // New material name
        newA,         // A
        newZ,               // Z
        newDensity             // Densities
    ));

    ToyEarth.UpdateLayers(updatelist);
    ToyEarth.BuildEarth();

    ToyEarth.SetDetectorLocation(0.0,90.0,6371); //lon lat rad:  Currently for SouthPole;
    ToyEarth.GetEarthPath(180.0,60.0); 
    ToyEarth.Visualize();

    //ToyEarth.GetSubLayerInfo(1,3);
    //ToyEarth.GetBlocksInfo(1,1,6);
    //ToyEarth.GetModelInfo();

}
/*
//////////////////////////////////////////

gGeoManager->InitTrack(r_src.X(), r_src.Y(), r_src.Z(), n_dir.X(), n_dir.Y(), n_dir.Z());
    gGeoManager->FindNextBoundaryAndStep();
    
    TGeoNode* startNode = gGeoManager->FindNode();
    TGeoVolume* startVol = startNode->GetVolume();
    std::cout << "Checking Init volume name" << startVol->GetName() <<  std::endl;

    if (!startNode) {
         std::cerr << "Start point is outside geometry!" << std::endl;
        return;
    }

    

    while (!gGeoManager->IsOutside()) {
        gGeoManager->FindNextBoundary(); // Get next step
        double step = gGeoManager->GetStep();
        if (step <= 0) break;

        gGeoManager->Step(); // Move to next boundary

        const char* path = gGeoManager->GetPath();
        TGeoVolume* cvolume = gGeoManager->GetCurrentVolume();
        TGeoMaterial* cmaterial = cvolume->GetMedium()->GetMaterial();
        TGeoNode* cnode = gGeoManager->GetCurrentNode();

        if (!cvolume || !cmaterial) break;

        std::string volName = cvolume->GetName();
        std::string matName = cmaterial->GetName();

        double length = step;
        double density = cmaterial->GetDensity();
        double A = cmaterial->GetA();
        double Z = cmaterial->GetZ();
        double ZoverA = (A > 0) ? Z / A : 0.0;

        //int layerID = -1, sublayerID = -1;
        //sscanf(volName.c_str(), "Layer%d_Sublayer%d", &layerID, &sublayerID);

        std::cout << std::fixed << std::setprecision(3)
                << "Step Length: " << length << " cm, "
                << "ρ: " << density << " g/cm³, "
                << "Z/A: " << ZoverA << ", "
                << "Material: " << matName << ", "
                << "Volume: " << volName << ", "
                << "Layer: " << layerID << ", "
                << "Sublayer: " << sublayerID << std::endl;
    }

//////////////////////////////////////////
    /// === TGeoManager Initialization == 
    TGeoManager *geoManager = new TGeoManager("BoxGeo", "Block in Box demo");
    // Define vacuum for outer container
    TGeoMaterial *matVac = new TGeoMaterial("Vacuum", 0, 0, 0);
    TGeoMedium *medVac = new TGeoMedium("Vacuum", 1, matVac);
    // Create top-level box (10x10x10 km)
    TGeoVolume *top = new TGeoVolume("TopBox", new TGeoBBox("Box", 5000, 5000, 5000), medVac);
    geoManager->SetTopVolume(top);

    //== Target Sublayers for ubdates ==
    std::vector<int> blockIDs = {1,5,6,9};
    std::vector<int> blockIDs1 = {2,3,6,10};
    std::vector<int> blockIDs2 = {4,7,9,12};
    std::string newName = "UpdatedBlockConfirm";
    std::vector<double> newA = {1,1,1,1};
    std::vector<double> newZ = {1,1,1,1};
    std::vector<double> newDensity = {1,1,1,1};

    //Update list
    std::vector<SublayersUpdates> updatelist;

    updatelist.push_back(SublayersUpdates(
        1, 1,                        // Layer index, Sublayer index
        blockIDs,                   // Block IDs
        newName,          // New material name
        newA,         // A
        newZ,               // Z
        newDensity             // Densities
    ));

    updatelist.push_back(SublayersUpdates(
        1, 2,                        // Layer index, Sublayer index
        blockIDs1,                   // Block IDs
        newName,          // New material name
        newA,         // A
        newZ,               // Z
        newDensity             // Densities
    ));

    updatelist.push_back(SublayersUpdates(
        1, 3,                        // Layer index, Sublayer index
        blockIDs2,                   // Block IDs
        newName,          // New material name
        newA,         // A
        newZ,               // Z
        newDensity             // Densities
    ));


    //== Create Earth Canvas == 
    std::string filename = "prem_15layers.txt";
    std::ifstream infile("/home/ydenizhernandez/OscProb/PremTables/" + filename);
    std::string line;

std::vector<std::vector<std::vector<double>>> LayerConfigVector;
std::vector<std::vector<double>> SubLayerConfigV;

std::vector<std::vector<SublayerConfig>> layersconfig_i;
std::vector<SublayerConfig> sublayersconfig_i;

std::vector<LayerConfig> layerdata; // This vectors stores layer objetcs filled with sublayers

double radiusprev = 0.0; // Inner radius
int indexprev = -1;
int slindex = 1;

// == Create Earth from txt file ==
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        double radius, density, z_a;
        int index;

        if (!(iss >> radius >> density >> z_a >> index)) {
            std::cerr << "WARNING: Invalid line skipped: " << line << std::endl;
            continue;
        }

        if (index != indexprev && indexprev != -1) {

            layerdata.push_back({"Layer", indexprev, 30, 60, sublayersconfig_i}); // stores the different groups in sublayer

            LayerConfigVector.push_back(SubLayerConfigV);
            SubLayerConfigV.clear();

            layersconfig_i.push_back(sublayersconfig_i);
            sublayersconfig_i.clear();

            
            slindex = 1; //restart index of sublayers
        }

        std::cout << "Sublayer info: rmin=" << radiusprev
                << " rmax=" << radius
                << " ρ="    << density
                << " Z/A="  << z_a
                << " index="<< index << "\n";

        SubLayerConfigV.push_back({radiusprev, radius, density, z_a});
        sublayersconfig_i.push_back({slindex, radiusprev, radius, density, z_a, 1, "SublayerMaterial"});
        
        radiusprev = radius;
        indexprev = index;
        slindex++;
    }

    // Final push
    
    if (!SubLayerConfigV.empty()) {
        LayerConfigVector.push_back(SubLayerConfigV);
        
    }
    if (!sublayersconfig_i.empty()) {
        layersconfig_i.push_back(sublayersconfig_i);
        layerdata.push_back({"Layer", indexprev, 30, 60, sublayersconfig_i});
    }


// DEBUG PRINT
for (size_t i = 0; i < layerdata.size(); ++i) {
    std::cout << "\n--- Layer index : " << i << " ---\n";

    for (size_t j = 0; j < layerdata[i].sublayersconfig.size(); ++j) {
        std::cout << " Info Sublayer from struct " << layerdata[i].sublayersconfig[j].layerID
                  << " | rmin: " << layerdata[i].sublayersconfig[j].rmin
                  << " | rmax: " << layerdata[i].sublayersconfig[j].rmax
                  << " | rho: "  << layerdata[i].sublayersconfig[j].density
                  << " | Z/A: "  << layerdata[i].sublayersconfig[j].atomicMass << "\n";
    }
}

std::cout << "    " << std::endl;
std::cout << "    " << std::endl;
std::cout << "    " << std::endl;

std::vector<Layer> EarthLayers; 

//layername(name), layerindex(id), zenithbinning(dth), azimuthbinning(dphi), sublayersconfig(configs)

for (int i = 0; i < layerdata.size(); ++i)
{   
    std::string lname = layerdata[i].layername ;
    double dphi=layerdata[i].azimuthbinning;
    double dth = layerdata[i].zenithbinning;
    int lid = layerdata[i].layerindex;
    Layer ithlayer(lname, lid, dth, dphi);
    ithlayer.SetSubLayers(layerdata[i].sublayersconfig);

  for (const auto& update : updatelist) {
    if (update.LayerIndex == i) {
        ithlayer.UpdateBlocksInSubLayer(update.SubLayerIndex,
                                        update.blockids,
                                        update.NewMaterialName,
                                        update.blocks_As,
                                        update.blocks_Zs,
                                        update.blocks_densities);
    }
}
    ithlayer.BuildLayer();
    EarthLayers.push_back(ithlayer);
}

for (int i = 0; i < EarthLayers.size(); ++i)
{
    top->AddNode(EarthLayers[i].GetLayerVolume(), i, new TGeoTranslation(0, 0, 0));
}

    // Save for later reference if needed
    geoManager->CloseGeometry();
    top->Draw("gl");

//////////////////////////////////////////
    Layer InnerCore("InnerCore",0,30,60);

    std::vector<SublayerConfig> sublayersconfigIC = {{1,0, 1221.5, 12.9807, 0.4691, 1, "Iron"}};

    InnerCore.SetSubLayers(sublayersconfigIC);
    InnerCore.BuildLayer();

    Layer OuterCore("OuterCore",1,30,60);
    std::vector<SublayerConfig> sublayersconfigOC = {
        {1,1221.5, 1946.7, 11.9512, 0.4691, 1, "FeNi"},
        {2,1946.7, 2475.1, 11.4726, 0.4691, 1, "FeNi"},
        {3,2475.1, 2900.3, 10.9884, 0.4691, 1, "FeNi"},
        {4,2900.3, 3260.6, 10.5008	, 0.4691, 1, "FeNi"},
        {5,3260.6, 3480.0, 10.0861, 0.4691, 1, "FeNi"}
    };

    OuterCore.SetSubLayers(sublayersconfigOC);
    OuterCore.BuildLayer();

    Layer LowerMantle("LowerMantle",2,30,60);

    std::vector<SublayerConfig> sublayersconfigLM = {
        {1,3480.0, 4476.0, 5.3225, 0.4954, 1, "SilicateLM"},
        {2,4476.0, 5378.4, 4.8332, 0.4691, 1, "SilicateLM"},
        {3,5378.4, 5701.0, 4.4872, 0.4691, 1, "SilicateLM"}
    };

    LowerMantle.SetSubLayers(sublayersconfigLM);
    LowerMantle.UpdateBlocksInSubLayer(2,blockIDs,newName,newA,newZ,newDensity);
    LowerMantle.BuildLayer();

    Layer UpperMantle("UpperMantle",3,30,60);

    std::vector<SublayerConfig> sublayersconfigUM = {
        {1,5701.0, 5971.0, 3.8983	, 0.4954, 1, "SilicateUM"},
        {2,5971.0, 6346.6, 3.4306	, 0.4691, 1, "SilicateUM"}
    };

    UpperMantle.SetSubLayers(sublayersconfigUM);
    
    UpperMantle.BuildLayer();

    Layer Crust("Crust",4,30,60);

    std::vector<SublayerConfig> sublayersconfigC = {
        {1,6346.6, 6356.0, 2.9000	, 0.4956, 1, "SilicateC"},
        {2,6356.0, 6368.0, 2.6000	, 0.4956, 1, "SilicateC"}
    };

    Crust.SetSubLayers(sublayersconfigC);
    Crust.BuildLayer();

    Layer Ocean("Ocean",5,30,60);

    std::vector<SublayerConfig> sublayersconfigO = {
        {1,6368.0, 6371.0, 1.0200	, 0.5525, 1, "SilicateO"}
    };
    Ocean.SetSubLayers(sublayersconfigO);
    Ocean.BuildLayer();

    Layer Atmosphere("Atmosphere",6,30,60);

      std::vector<SublayerConfig> sublayersconfigAtm = {
        {1,6371.0, 6386.0	, 0.001	, 0.4991, 1, "Vac"}
    };

    Atmosphere.SetSubLayers(sublayersconfigAtm);
    Atmosphere.BuildLayer();

 // top->AddNode(InnerCore.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    //top->AddNode(OuterCore.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    //top->AddNode(LowerMantle.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    top->AddNode(UpperMantle.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    //top->AddNode(Crust.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    //top->AddNode(Ocean.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    //top->AddNode(Atmosphere.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));




void TesselatedEarth() {

    TGeoManager *geoManager = new TGeoManager("BoxGeo", "Block in Box demo");

    // Define vacuum for outer container
    TGeoMaterial *matVac = new TGeoMaterial("Vacuum", 0, 0, 0);
    TGeoMedium *medVac = new TGeoMedium("Vacuum", 1, matVac);

    // Create top-level box (10x10x10 km)
    TGeoVolume *top = new TGeoVolume("TopBox", new TGeoBBox("Box", 5000, 5000, 5000), medVac);
    geoManager->SetTopVolume(top);
    
    
    Layer InnerCore("InnerCore",0,30,60);

    std::vector<SublayerConfig> sublayersconfigIC = {{1,0, 1221.5, 12.9807, 0.4691, 1, "Iron"}};

    Layer OuterCore("OuterCore",1,30,60);
    std::vector<SublayerConfig> sublayersconfigOC = {
        {1,1221.5, 1946.7, 11.9512, 0.4691, 1, "FeNi"},
        {2,1946.7, 2475.1, 11.4726, 0.4691, 1, "FeNi"},
        {3,2475.1, 2900.3, 10.9884, 0.4691, 1, "FeNi"},
        {4,2900.3, 3260.6, 10.5008	, 0.4691, 1, "FeNi"},
        {5,3260.6, 3480.0, 10.0861, 0.4691, 1, "FeNi"}
    };

    OuterCore.SetSubLayers(sublayersconfigOC);

    Layer LowerMantle("LowerMantle",2,30,60);

    std::vector<SublayerConfig> sublayersconfigLM = {
        {1,3480.0, 4476.0, 5.3225, 0.4954, 1, "SilicateLM"},
        {2,4476.0, 5378.4, 4.8332, 0.4691, 1, "SilicateLM"},
        {3,5378.4, 5701.0, 4.4872, 0.4691, 1, "SilicateLM"}
    };

    LowerMantle.SetSubLayers(sublayersconfigLM);

    Layer UpperMantle("UpperMantle",3,30,60);

    std::vector<SublayerConfig> sublayersconfigUM = {
        {1,5701.0, 5971.0, 3.8983	, 0.4954, 1, "SilicateUM"},
        {2,5971.0, 6346.6, 3.4306	, 0.4691, 1, "SilicateUM"}
    };

    UpperMantle.SetSubLayers(sublayersconfigUM);

    Layer Crust("Crust",4,30,60);

    std::vector<SublayerConfig> sublayersconfigC = {
        {1,6346.6, 6356.0, 2.9000	, 0.4956, 1, "SilicateC"},
        {2,6356.0, 6368.0, 2.6000	, 0.4956, 1, "SilicateC"}
    };

    Crust.SetSubLayers(sublayersconfigC);

    Layer Ocean("Ocean",5,30,60);

    std::vector<SublayerConfig> sublayersconfigO = {
        {1,6368.0, 6371.0, 1.0200	, 0.5525, 1, "SilicateO"}
    };
    Ocean.SetSubLayers(sublayersconfigO);

    Layer Atmosphere("Atmosphere",6,30,60);

      std::vector<SublayerConfig> sublayersconfigAtm = {
        {1,6371.0, 6386.0	, 0.001	, 0.4991, 1, "Vac"}
    };

    Atmosphere.SetSubLayers(sublayersconfigAtm);
    



    //Set Some example sublayers
    std::vector<SublayerConfig> sublayersconfig = {
    {1,0, 200.0, 5.5, 55.85, 26, "Iron"},
    {2,400.0, 800, 4.5, 40.08, 20, "Calcium"},
    {3,1000.0, 2000.0, 3.6, 24.31, 12, "Magnesium"},
    };

  

    Layer ToyLayer("ToyLayer",1,30.0,60.0);
    ToyLayer.SetSubLayers(sublayersconfig);
    
     std::vector<int> blockIDs = {1,5,9,15};
     std::string newName = "UpdatedBlockConfirm";
     std::vector<double> newA = {1,1,1,1};
     std::vector<double> newZ = {1,1,1,1};
     std::vector<double> newDensity = {1,1,1,1};

    //ToyLayer.UpdateBlocksInSubLayer(1,blockIDs,newName,newA,newZ,newDensity);
    //ToyLayer.UpdateBlocksInSubLayer(2,blockIDs,newName,newA,newZ,newDensity);
    ToyLayer.UpdateBlocksInSubLayer(3,blockIDs,newName,newA,newZ,newDensity); // Update a set of bloks in sublayer
    ToyLayer.UpdateSublayer(1,"UpdatedLayerConfirm",6.6,6.6, 6.6); //Update whole sublayer
    ToyLayer.BuildLayer();

    //ToyLayer.SubLayerSummary(1); // Get info of specific sublayer
    ToyLayer.BlockSummary(3,10); //Get info of specific block
    ToyLayer.LayerSummary();


    
    //ToyLayer.UpdateBlocksInSubLayer(3,blockIDs2,newName2,newA2,newZ2,newDensity2);

    //ToyLayer.BuildLayer();

    top->AddNode(ToyLayer.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    // Save for later reference if needed
    geoManager->CloseGeometry();
    top->Draw("gl");

}





//TEST FOR SUBLAYERS
void TesselatedEarth() {

    TGeoManager *geoManager = new TGeoManager("BoxGeo", "Block in Box demo");

    // Define vacuum for outer container
    TGeoMaterial *matVac = new TGeoMaterial("Vacuum", 0, 0, 0);
    TGeoMedium *medVac = new TGeoMedium("Vacuum", 1, matVac);

    // Create top-level box (10x10x10 km)
    TGeoVolume *top = new TGeoVolume("TopBox", new TGeoBBox("Box", 5000, 5000, 5000), medVac);
    geoManager->SetTopVolume(top);
    
    Sublayer sublayer;
    //sublayer.SetSubLayerMaterial("example", 1, 1, 1);

    std::cout<< "Build blocks and Assemble the to create a sublayer"<< std::endl;
    sublayer.CreateSubLayerBlocks( 6171.0, 6371, 30.0, 20.0);
    std::cout<< "Create sublayer from blocks"<<std::endl;


    std::cout<< "Create sublayer from blocks"<<std::endl;
    int target = 5;
    //sublayer.ModifyBlockMaterial(target,"Silicate",66.6,66.6,66.6);

    std::vector<int> Multipletargets = {1, 15, 20};  // Any set of block IDs
    //sublayer.UpdateMultipleBlocksMaterial(Multipletargets,"Silicate",66.6,66.6,66.6);
    sublayer.BuildSubLayer();
    

    top->AddNode(sublayer.GetSubLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    // Save for later reference if needed
    geoManager->CloseGeometry();
    top->Draw("gl");
    std::cout<< "Summary of a single block"<<std::endl;
    sublayer.GetBlockSummary(target);
    std::cout << " " << std::endl;
    std::cout<< "Summary of all blocks"<<std::endl;
    sublayer.GetAllBlockSummary();

}
 

// top-level macro function
void TesselatedEarth() {

    TGeoManager *geoManager = new TGeoManager("BoxGeo", "Block in Box demo");

    // Define vacuum for outer container
    TGeoMaterial *matVac = new TGeoMaterial("Vacuum", 0, 0, 0);
    TGeoMedium *medVac = new TGeoMedium("Vacuum", 1, matVac);

    // Create top-level box (10x10x10 km)
    TGeoVolume *top = new TGeoVolume("TopBox", new TGeoBBox("Box", 5000, 5000, 5000), medVac);
    geoManager->SetTopVolume(top);

    // Create and configure block
    TGeoVolumeAssembly* layer = new TGeoVolumeAssembly("Layer") ; //Combine all blocks forming the layer
    
    /////
    std::ifstream infile("./earth_binned_default.txt");
    std::string line;
    int id = 1;

    //Create the blocks using a CSV file
    double dphi = 90.0;
    double dtheta = 60.0;
    double dr = 200.0;  // You might customize this per layer
    double rho_test = 0;

    while (std::getline(infile, line)) {

        std::istringstream iss(line);
        double phi_c, theta_c, radius, density, ZoA;
        int materialFlag;

        if (!(iss >> phi_c >> theta_c >> radius >> density >> ZoA >> materialFlag)) continue;

        // Only keep blocks with density == 13.088
        if (std::abs(density - 13.088) > 0.001) continue;
        // Define angular and radial bounds
        double r1 = radius - dr / 2.0;
        double r2 = radius + dr / 2.0;
        double th1 = theta_c - dtheta / 2.0 + 90.0;
        double th2 = theta_c + dtheta / 2.0 + 90.0;
        double phi1 = phi_c - dphi / 2.0;
        double phi2 = phi_c + dphi / 2.0;

        std::cout <<id << " : " <<  th1 << " " << theta_c << " " <<  th2 << " | " << phi1 << " " << phi_c << " " <<  phi2 << std::endl;

        Block b;

        b.SetGeometry(r1, r2, th1, th2, phi1, phi2, id);
        b.DefineMaterial("Iron", 55.85, 26, 7.87);
        b.BuildBlock( int colorid = 1);

        // Add to box
        layer->AddNode(b.GetVolume(), b.GetID());
        id ++;

        }
        top->AddNode(layer, 1);
        // Save for later reference if needed


        geoManager->CloseGeometry();
        top->Draw("gl");

}

*/

 /*
        //Modify Specific sublayer  block
        void UpdateSublayer(int id,std::string newName, double newZ, double newA, double newdensity){
            sublayers[id-1].SetSubLayerMaterial(newName.c_str,newZ,newA, newdensity);

        }
        void UpdateBlocksInSublayer(int idsublayer,int idblock,std::string newName, double newZ, double newA, double newdensity)
        {sublayers[idsublayer-1].UpdateBlockMaterial(idblock,newName.c_str,newZ,newA,newdensity);}

        void UpdateSubLayerBlocks(const int idsublayer,
                                  const std::vector<int>& blockids,
                                  const std::string& NewName,
                                  const std::vector<double>& NewA, const std::vector<double>& NewZ, const std::vector<double>& NewDensity) {
            
            for (const auto&  bids: blockids)
            {
                sublayers[idsublayer-1].UpdateMultipleBlocksMaterial(blockids,NewName, NewA,newZ,NewDensity)
            }
            
        }
        */
