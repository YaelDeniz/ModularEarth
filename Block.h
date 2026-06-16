///////////////////////////////////////////////////////////////////////////////
/// \class OscProb::TesselatedEarth
///
/// \brief Generator of a tessellated 3D Earth model for neutrino propagation.
///
/// This class builds a modular three-dimensional Earth model composed of
/// tessellated spherical shells representing Earth’s internal layers.
/// The tessellation allows individual regions to be modified independently,
/// enabling the inclusion of lateral heterogeneities such as mantle plumes,
/// slabs, hydrous regions, Large Low-Velocity Provinces (LLVPs), Ultra-Low
/// Velocity Zones (ULVZs), and other localized structures.
///
/// The class relies on the tracking capabilities of CERN ROOT’s `TGeoManager`
/// to propagate neutrinos through the Earth geometry. For each traversed
/// volume, physical properties such as density, Z/A, and path length are
/// computed. The geometry and material properties of layers, sublayers, and
/// individual blocks can be modified dynamically at runtime.
///
/// ---------------------------------------------------------------------------
/// Coordinate system
/// ---------------------------------------------------------------------------
/// CERN ROOT uses a right-handed Cartesian coordinate system.
/// The convention adopted in this class is:
///   +z : upward direction
///   +x : Greenwich meridian
///   +y : eastward longitude
///
/// ---------------------------------------------------------------------------
/// Main components
/// ---------------------------------------------------------------------------
///
/// 1) Block
///    The smallest building unit of the tessellated Earth model.
///    A Block represents a spherical shell segment with user-defined size
///    and material properties (density, Z, A).
///    Material properties can be modified at any time after construction.
///
/// 2) SubLayer
///    An assemblage of Blocks forming a continuous radial shell.
///    All Blocks within a SubLayer share the same geometric dimensions and
///    default material properties.
///    Users may update the properties of the entire SubLayer or selectively
///    modify individual Blocks to introduce localized heterogeneities.
///
/// 3) Layer
///    An assemblage of SubLayers corresponding to major Earth regions or
///    phase transitions (e.g., the Dʺ layer, LLVPs, or the Mantle Transition
///    Zone).
///    Each Layer defines the number of SubLayers it contains, along with their
///    respective block sizes and material properties.
///    Users may modify properties at the Layer, SubLayer, or Block level.
///
/// 4) Earth
///    The top-level container assembling all Layers into a complete Earth model.
///    This is the primary volume used for neutrino propagation studies.
///    The Earth model may be initialized from a PREM profile or constructed
///    from scratch by the user.
///    Regardless of initialization method, a uniform block size must be
///    specified, and all material properties remain fully accessible and
///    modifiable.
///    Neutrino tracking and path integration are performed at this level.
///
/// ---------------------------------------------------------------------------
/// \author Yael Deniz (yaeldeniz0499@gmail.com)
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
///             3) Convetion labeling of bins, Maybe start filling to the right of local axis 
///             4) Map projection 
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
#include <TCanvas.h>
#include <TGraph.h>

//OscProb
#include "PremModel.h"



//#include <TGeoMarker.h>

class Block {
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// This class generates the "building blocks"  for the modular Earth
    /// Each block is a shell sergement defined by:
    ///     rmin-Inner Radius of the shell
    ///     rmax-outer radius of the shell
    ///     phi bins (longitude) and theta bins (colatitude)
    ///     (th_min-th_max) and (phi_min-phi_max) - Blocks angular limits
    /// The material (Z,A) of each block can be modified and, from a sublayer, user can access each block and modify it
    /// 
    ///
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    private: 
    TGeoVolume * block = nullptr; // Owns the shape
    TGeoMaterial * blockMaterial = nullptr; // Owns the Material
    TGeoMedium * blockMedium = nullptr; // Owns the Medium

    // Block Geometry Parameters
    double rmin = 0, rmax = 1; // Inner (rmin) and outer (rmax) radius of the block
    double thetamin = 0, thetamax = 45; // Block is constructed for \theta \in [thetamin, thetamax]
    double phimin = 0, phimax = 25; // Block is constructed for \phi \in [phimin, phimax]
    // Material Parameters
    double atomicMass = 0; // Atomic Mass (A) of volume material
    double atomicNumber = 0; // Atomic Number (Z) of volume material
    double density = 0; // Density (ρ) of volume medium
    std::string materialName = "Vacuum"; // Name of block material, by defualt it is set as vacuum.

    // Indexing and flags for block info access and modificiations .
    int blockID = 1; //Each block has a unique index id
    bool BlockModified = false; // Flag that indicates if material of a specific set of blocks was modified
    bool SubLayerModified = false; //Flag that indicates if material of all the blocks in a sublayer was modified
    int colorIndex = 1; // Index used to highlight modified blocks with different colors

    public: 
    
    /// Update flag when specific block is modified
    void BlockWasModified(bool status) {
        BlockModified = status;
    }

    // Update flag when all material in sublayer is modified 
    void SubLayerWasModified(bool status) {
        SubLayerModified = status;
    }
    
    // Default Constructor (Uses the default variable values defined in private)
    Block() =
    default;

    // Return Block ID
    int GetBlockID() const {
        return blockID;
    }

    // Set the geometry defining parameters of a block and its indexing label

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

    // Define material and medium of a block
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
        block -> SetVisibility(kTRUE);   //Set Block Visibility
        //block->SetFillColor(kYellow);   // face color 
        if (BlockModified) {
            block -> SetLineColor(kRed);
            block -> SetVisibility(kTRUE);
            block -> SetFillColor(kRed);
        } // Distinguishable colors
        //else{block->SetLineColor(kGreen + (blockID % 5));}
        if (SubLayerModified) {
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
            std::cout << std::boolalpha << "  Specific Block Modified: " << BlockModified << std::endl;
            std::cout << std::boolalpha << "  Sub Layer Modified: " << SubLayerModified << std::endl;
            std::cout << "  Material:        " << mat -> GetName() << std::endl;
            std::cout << "  Atomic Num  Z: " << mat -> GetZ() << std::endl;
            std::cout << "  Atomic Mass A: " << mat -> GetA() << std::endl;
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
    /// Represents a Earth sublayer by a spherical shell segment composed of multiple Blocks (TGeoVolumes)
    /// - Can be initialized homogeneously with a default material
    /// - Allows updating material properties for specific blocks
    /// - Supports printing detailed information and geometry inspection
    /// - Rebuilds ROOT geometry assemblies only when requested
    /////////////////////////////////////////////////////////////////////////////////////

    private: TGeoVolumeAssembly * SubLayer = nullptr;
    std::vector < Block > blocks; // Vector containing all the Sublayer-forming blocks

    //SubLayer Labels
    int SubLayerID = 1; // Label to identify a specific sublayer
    std::string SubLayerName="ClassDefaultSubLayerName";

    //SubLayer Material- Set the properties to be the same for all blocks in a sublayers
    std::string SubLayerMaterialName = "ClassDefaultMaterial";
    double SubLayerZ = 26;
    double SubLayerA = 55.85;
    double SubLayerDensity = 7.87;

    //Flags for tracking changes in sublayers' blocks
    bool BlocksUpdate = false; //Flags if only a set of blocks was modified (updated)
    bool FullUpdate = false;   //Flags if whole blocks in sublayer were modified (updated)

    // Binning of SubLayer
    double Nphi = 4; //West-East binning: Number of bins in longitude
    double Nth = 3; //North-South binning: Number of bins in latitude
    double dphi = 90;//West-East binning size
    double dth = 60;//North-South binning size

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

    public:
    //Default constructor of a sublayer
    Sublayer() =
        default;

    // Set SubLayer ID. SubLayers are name based on their ID (e.g., "SubLayer_#ID")
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
        //Error: size of bin does not correpond to integer number of bins
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
        modifiedBlock.BlockWasModified(true);
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
            modifiedBlock.BlockWasModified(true);
            modifiedBlock.BuildBlock();
        }
        BlocksUpdate = true;
    }

    void UpdateAllBlocksMaterial(const std::string & NewName, double NewA, double NewZ, double NewDensity) {
        for (Block & modifiedBlock: blocks) {
            modifiedBlock.DefineBlockMaterial(NewName, NewA, NewZ, NewDensity);
            modifiedBlock.SubLayerWasModified(true);
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

    //void function for Intersection calculation

    //

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
        TVector3 rL = r0-r_src;    // Detector location in RHCC


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
        std::cout << "Total Propagation distance" <<  sqrt(rL.Mag2()) << std::endl;


        std::cout << "Test number: " << sqrt(r_src.X()*r_src.X() + r_src.Y()*r_src.Y() + r_src.Z()*r_src.Z())  << std::endl;
        
        //Tracking
        double start_point[3] = {r_src.X(), r_src.Y(), r_src.Z()};
        double direction[3] = {n_dir.X(), n_dir.Y(), n_dir.Z()};
        //TGeoNavigator* navigator = gGeoManager->GetCurrentNavigator();
        //navigator->SetCurrentPoint(start_point);    // double[3]
        //navigator->SetCurrentDirection(direction);  // double[3]
        //navigator->FindNode();                      // Enter volume

        std::cout << "Initializing Tracking" << std::endl;
        gGeoManager->InitTrack(r_src.X(), r_src.Y(), r_src.Z(), n_dir.X(), n_dir.Y(), n_dir.Z());
        gGeoManager->FindNextBoundaryAndStep(); //First Step
        //////////////////////////////////////////////////////
        double TotalLegth = 0;
        while (!gGeoManager->IsOutside()) 
        {
                gGeoManager->FindNextBoundary(); // Get next step
                double step = gGeoManager->GetStep();
                if (step <= 0) break;

                gGeoManager->Step(); // Move to next boundary
                TotalLegth +=step;
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
                        << "Total Length: " << TotalLegth << " cm, "
                        << "ρ: " << density << " g/cm³, "
                        << "Z/A: " << ZoverA << ", "
                        << "Material: " << matName << ", "
                        << "Volume: " << volName << std::endl;
                      
        }
        //////////////////////////////////////////////////////

    }
    /////WIP
    void GetEarthPathWIP(double zenith, double azimuth) {
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
        TVector3 rL = r0-r_src;    // Detector location in RHCC


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
 
        ///////////////////////////////////////////////////////Tracking
            
        auto& geo = *gGeoManager;
        geo.InitTrack(r_src.X(), r_src.Y(), r_src.Z(), n_dir.X(), n_dir.Y(), n_dir.Z());
        double SumL = 0; 
        if (!geo.FindNode()) return;

        //NUDGING
        auto nudge_into_next = [&]{

        const double epsnudge = 1e-5; // cm
        std::cout << "Inside of nudging function. Tolerance: "<< epsnudge << std::endl;
        double X[3] = { geo.GetCurrentPoint()[0], geo.GetCurrentPoint()[1], geo.GetCurrentPoint()[2] };
        double D[3] = { geo.GetCurrentDirection()[0], geo.GetCurrentDirection()[1], geo.GetCurrentDirection()[2] };
        geo.SetCurrentPoint(X[0]+epsnudge*D[0], X[1]+epsnudge*D[1], X[2]+epsnudge*D[2]);
        geo.FindNode();
        };

        if (geo.IsOnBoundary()) nudge_into_next();

        const double BIG  = 1e30;
        const double ZEPS = 1e-10;

        while (!geo.IsOutside()) {
        
            if (abs(cradius - radius) <= 1E-3 && SumL > 1000.0)
        {
            std::cout << "Neutrino Reach detector, tracking stopped"  << cradius << " " << radius << " " << abs(cradius - radius)  << std::endl;
            break; // Stopping at detector location
        }
        // volume BEFORE stepping (material you’re in)
        TGeoVolume* volBefore = geo.GetCurrentVolume();
        TGeoVolume* vol = geo.GetCurrentVolume();
        TGeoMaterial* mat = (volBefore && volBefore->GetMedium())
                                ? volBefore->GetMedium()->GetMaterial() : nullptr;

        // step to next boundary
        geo.FindNextBoundaryAndStep(BIG, kTRUE);
        double s = geo.GetStep();
        double cradius = 0;
        // guard: if we got a zero step on a boundary, nudge and try next iteration
        if (s <= ZEPS && geo.IsOnBoundary()) { nudge_into_next(); continue; }

        // log segment (after step)
        if (mat && s > ZEPS) {
            double X[3] = { geo.GetCurrentPoint()[0], geo.GetCurrentPoint()[1], geo.GetCurrentPoint()[2] };
            double r_bnd = std::sqrt(X[0]*X[0] + X[1]*X[1] + X[2]*X[2]);
            cradius=r_bnd;


            
            std::cout << std::fixed << std::setprecision(3)
                << "Boundary radius " << r_bnd
                << " Coordinates " << X[0] << " " << X[1] << " " << X[2]
                << " | Step Length: " << s << " Path Length" << SumL << " cm\n";
                std::cout << "ρ: " << mat->GetDensity()
                << " g/cm³, Z/A: " << (mat->GetA() ? mat->GetZ()/mat->GetA() : 0.0)
                << ", Material: " << mat->GetName()
                << ", Volume: " << (vol ? vol->GetName() : "NULL") << "\n\n"
                << std::boolalpha << "  Entering Volume: " << gGeoManager->IsEntering() 
                << std::boolalpha << "  Exiting Volume: " << gGeoManager->IsExiting() << "\n\n" << std::endl;
            

                SumL += s;
                ModularEarthProfile->AddPoint(SumL,mat->GetDensity());
                ModularPathL.push_back(SumL);
                ModularPathR.push_back(r_bnd);
                ModularPathDensity.push_back(mat->GetDensity());

        }

        if (geo.IsOutside()) break;

        // volume AFTER stepping (useful if you want from→to info)
        TGeoVolume* volAfter = geo.GetCurrentVolume();

        // if we’re exactly on a boundary, push into the next volume
        if (geo.IsOnBoundary()){
            std::cout << "NUDGIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIING into next boundary" << std::endl;
            nudge_into_next();

        }

        // (optional) explicit cross-check without flags:
        // if (volAfter != volBefore) { /* crossed a boundary */ }
        }
        //////////////////////////////////////////////////////

    }
    /////
};

// top-level macro function

//Test for Tracking in SubLayers
void TesselatedEarth() {

    TMultiGraph * ThePaths = new TMultiGraph();
    TGraph * ModularEarthProfile = new TGraph();
    std::vector<double> ModularPathL;
    std::vector<double> ModularPathR;
    std::vector<double> ModularPathDensity;
    TGraph * OscProbProfile = new TGraph();

    ///////////////////////////////////////
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

    //ToyEarth.UpdateLayers(updatelist);
    ToyEarth.BuildEarth();

    ToyEarth.SetDetectorLocation(0.0,-90.0,6371); //lon lat rad:  Currently for SouthPole;
    double zenith = 179.99;
    ToyEarth.GetEarthPath(zenith,0.0); 
    ToyEarth.Visualize();

    /////////////////////////////////////// Comparison with OscProb

    std::cout << " " << std::endl;

    std::string PremTables = "/home/ydenizhernandez/OscProb/PremTables/";
    std::string PremFile = "prem_15layers.txt";
    std::string PremPath = PremTables+PremFile;
    OscProb::PremModel prem(PremPath);
    double cth = cos(zenith * M_PI/180.0);
    //std::cout << M_PI << " " << zen_rad << " " << zenith * M_PI/180.0 << std::endl;
    prem.FillPath(cth); // Fill paths from PREM model
    std::vector<OscProb::NuPath> paths = prem.GetNuPath();

    //std::cout << paths.size() << " " << ModularPathL.size() << " " << std::endl;

    //for (size_t i = 0; i < ModularPathL.size(); i++)
    //{
      //  std::cout<< i << " " << ModularPathR[i] <<  " " << ModularPathL[i] << " " << ModularPathDensity[i] << std::endl;
    //}
    
    std::cout << " " << std::endl;
    
    double sumL_OscProb = 0;

    for (int i = 0; i < paths.size() ; ++i)
    {

    sumL_OscProb += paths[i].length ; 

    OscProbProfile->SetPoint(i,sumL_OscProb,paths[i].density  );
    std::cout << i << " " << sumL_OscProb << " " << paths[i].density << std::endl;

    }

    std::cout << sumL_OscProb << " " << SumL << " " << std::endl;


    
    ///////

    TCanvas* c1 = new TCanvas("c1","Paths (TMultiGraph/TGraph)", 800, 600);
    c1->cd();

    ModularEarthProfile->SetLineColor(kRed);

    //ThePaths->Add(ModularEarthProfile);   // assuming ThePaths is a TMultiGraph
    ThePaths->Add(OscProbProfile);
    ThePaths->Draw("APL");                // axes, points, line
    // ThePaths->Add(OscProbProfile);     // if/when needed
    c1->Update();
    //ThePaths->Add(OscProbProfile);

    ///////
    TCanvas* c2 = new TCanvas("c2","Geometry (TGeo + 3D markers)", 900, 600);
    c2->cd();
    top->Draw("gl");
    //detMarker->Draw("same");
    NuDetMarkers->Draw("same");
    //trackingpoints->Draw("same");
    //srcDetMarkers->Draw("same");
    nuLine->Draw("same");
    TView *view = gPad->GetView();
    c2->Update();

}
/*
 //Comparion with OscProbe

    TMultiGraph * ThePaths = new TMultiGraph();

    TGraph * ModularEarthProfile = new TGraph();
    std::vector<double> ModularPathL;
    std::vector<double> ModularPathR;
    std::vector<double> ModularPathDensity;


    TGraph * OscProbProfile = new TGraph();

    //---------Create TOP volume
    TGeoManager *geoManager = new TGeoManager("TopVol", "Earth in Sphere demo");
    // Define vacuum for outer container
    TGeoMaterial *matVac = new TGeoMaterial("Vacuum", 0, 0, 0);
    TGeoMedium *medVac = new TGeoMedium("Vacuum", 1, matVac);
    // Create top-level box (10x10x10 km)
    //TGeoVolume *top = new TGeoVolume("TopBox", new TGeoBBox("Box", 5000, 5000, 5000), medVac);
    TGeoVolume* top = new TGeoVolume("EarthTopSphere", new TGeoSphere("Sphere", 0, 7000), medVac);
    geoManager->SetTopVolume(top);

    ////////////////////////////// Layer Test

    // SubLayer How to:
    // 1) To create a Layer do:
    //
    //    Layer LayerObject("Layer Name",LayerIndex,dlon,dlat)- Create a sublayer object with class constructor, dlon and dlat are lon lat bin size
    //
    // 2) Create a list containing information of the sublayers with sublayersconfigOC
    //
    //      std::vector<SublayerConfig> sublayersconfig = {
    //          {SubLayerIndex1, rmin, rmax, rho, z, A, "SubLayerMateialName"},
    //          {...},
    //          {...},
    //          ...
    //          {SubLayerIndexN, rmin, rmax, rho, z, A, "SubLayerMateialName"},
    //      };
    //
    // 3) from the configuration list, Create the sublayer and stack them in the assembly making a layer with
    //
    //   LayerObject.SetSubLayers(sublayersconfig);   
    //
    // 4) Build the Layer Volume Assembly and ad it to top
    //
    //    LayerObject.BuildLayer();
    //    top->AddNode(LayerObject.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    //




    //Layer InnerCore("InnerCore",0,30,60); // Layer constructor indicating Layer name, index, lon bin size and lat bin size
    std::vector<SublayerConfig> sublayersconfigIC = {{1,0, 1221.5, 12.9807, 0.4691, 1, "Iron"}}; //Struct containing the list of sublayers of a layer
    //syntax {sublayer index, rmin,rmax,Z,A,rho, material name}
    //InnerCore.SetSubLayers(sublayersconfigIC);
    //InnerCore.BuildLayer();
    //Make Sphere

    TGeoMaterial *matIC = new TGeoMaterial("SolidNiFe",  0.4691, 1, 12.9807);
    TGeoMedium *medIC = new TGeoMedium("SolidNiFe", 1, matIC);
    TGeoVolume* InnerCore = new TGeoVolume("InnerCore", new TGeoSphere("Sphere", 0, 1221.5), medIC);

    //
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


    //top->AddNode(InnerCore.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    top->AddNode(InnerCore, 1,new TGeoTranslation(0, 0, 0));
    top->AddNode(OuterCore.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    top->AddNode(LowerMantle.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    top->AddNode(UpperMantle.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    top->AddNode(Crust.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    top->AddNode(Ocean.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    top->AddNode(Atmosphere.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    

    /////////////////////////////
    
    geoManager->CloseGeometry(); //Here we finished the model creation. "Closed" means that no more modification can be made to the model.
    geoManager->SetTopVisible();     // Ensure top volume shows up

    //Tracking Example--------------------------------------------------------------------------------------------------------------------------
    double lat = -90.0;
    double lon = 0.0;
    double radius = 6368;
    
    double lat_rad = lat * M_PI / 180.0;
    double lon_rad = lon * M_PI / 180.0;

    TVector3 x_hat, y_hat, z_hat;

    // Compute detector location in right handed cartesian coordinate
    double x_det = radius * cos(lat_rad) * cos(lon_rad);
    double y_det = radius * cos(lat_rad) * sin(lon_rad);
    double z_det = radius * sin(lat_rad);

    TVector3 r_det(x_det, y_det, z_det);  // Vector from Earth's center to detector

    //Detector local coordinate system set up:

    // Detector local +z axis is outward radial direction
    z_hat = r_det.Unit();
    // Handle poles: cross with X instead of Z
    if (fabs(z_hat.Z()) > 0.999) {
        x_hat = TVector3(1, 0, 0).Cross(z_hat).Unit();
    } else {
        x_hat = TVector3(0, 0, 1).Cross(z_hat).Unit();  // Local East
    }
    y_hat = z_hat.Cross(x_hat).Unit();  // Local North
    
    //Neutrino Direction in detectors coordinate system 
    double epszenith = 1E-4; //Tolerance for epsilon path 
    double zenith = 180.0 - (epszenith);
    double azimuth = 0.0;

    // Convert to radians
    double zen_rad = (180-zenith) * M_PI / 180.0;
    double az_rad  = azimuth * M_PI / 180.0;
    // Neutrino direction in detector frame
    TVector3 dir_local(
        sin(zen_rad) * cos(az_rad),
        sin(zen_rad) * sin(az_rad),
        cos(zen_rad)
    );

    // Convert to global direction using unit vetors representing the detector's local coordinate system
    TVector3 dir_global =
        x_hat * dir_local.X() +
        y_hat * dir_local.Y() +
        z_hat * dir_local.Z();

    TVector3 n_dir = dir_global.Unit();  // Normalize
    TVector3 r0(x_det, y_det, z_det);    // Detector location in RHCC

    //Neutrino Source Location
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
    TVector3 rL = r0-r_src;    // Detector location in RHCC

    //Draw The Neutrino Path
    TPolyLine3D* nuLine = new TPolyLine3D(2);
    nuLine->SetPoint(0, r_src.X(), r_src.Y(), r_src.Z());
    nuLine->SetPoint(1, r0.X(),    r0.Y(),    r0.Z());
    nuLine->SetLineColor(kBlue);
    nuLine->SetLineWidth(3);
   
    // Detector marker
    //TPolyMarker3D* detMarker = new TPolyMarker3D(1,1);
    //detMarker->SetPoint(0, r0.X(), r0.Y(), r0.Z());
    //detMarker->SetMarkerColor(6);
    //detMarker->SetMarkerSize(3);

    // Neutrino source marker
    //TPolyMarker3D* srcMarker = new TPolyMarker3D(1,1);
    //srcMarker->SetPoint(0, r_src.X(), r_src.Y(), r_src.Z());
    //srcMarker->SetMarkerColor(9);
    //srcMarker->SetMarkerSize(3);

    TPolyMarker3D *NuDetMarkers= new TPolyMarker3D(100,2); //Markers indicating DetLoc and Inical Neutrino
    NuDetMarkers->SetPoint( 0 , r0.X(),    r0.Y(),    r0.Z()); //Detector
    NuDetMarkers->SetPoint( 1 ,r_src.X(),r_src.Y(),r_src.Z()); //Incoming Neutrino
    //NuDetMarkers->SetPoint( 2 , 0.0,0.0,-3680.000); //Incoming Neutrino
    
    NuDetMarkers->SetMarkerColor(kRed);
    NuDetMarkers->SetMarkerSize(5);

    //TPolyMarker3D *srcDetMarkers= new TPolyMarker3D(2,2); //Markers indicating DetLoc and Inical Neutrino
    //NuDetMarkers->SetPoint( 0 , r0.X(),    r0.Y(),    r0.Z()); //Detector
    //NuDetMarkers->SetPoint( 0 ,r_src.X(),r_src.Y(),r_src.Z()); //Incoming Neutrino
    //NuDetMarkers->SetMarkerColor(kRed);
    //NuDetMarkers->SetMarkerSize(3);

    std::cout << "Magnitude test in the atmosphere" << sqrt( r_src.Mag2() ) << std::endl;
    std::cout << "Magnitude test detector location" << sqrt( r0.Mag2() ) << std::endl;
    std::cout << "Magnitude test normal vector" << sqrt( n_dir.Mag2() ) << std::endl;
    std::cout << "Total Propagation distance" <<  sqrt(rL.Mag2()) << std::endl;

    std::cout << "Source point radius: " << sqrt(r_src.X()*r_src.X() + r_src.Y()*r_src.Y() + r_src.Z()*r_src.Z())  << std::endl;
        
    ////// WIP : Tracking Algorithm


    auto& geo = *gGeoManager;
    geo.InitTrack(r_src.X(), r_src.Y(), r_src.Z(), n_dir.X(), n_dir.Y(), n_dir.Z());
    double SumL = 0; 
    if (!geo.FindNode()) return;

    //NUDGING
    auto nudge_into_next = [&]{

    const double epsnudge = 1e-5; // cm
    std::cout << "Inside of nudging function. Tolerance: "<< epsnudge << std::endl;
    double X[3] = { geo.GetCurrentPoint()[0], geo.GetCurrentPoint()[1], geo.GetCurrentPoint()[2] };
    double D[3] = { geo.GetCurrentDirection()[0], geo.GetCurrentDirection()[1], geo.GetCurrentDirection()[2] };
    geo.SetCurrentPoint(X[0]+epsnudge*D[0], X[1]+epsnudge*D[1], X[2]+epsnudge*D[2]);
    geo.FindNode();
    };

    if (geo.IsOnBoundary()) nudge_into_next();

    const double BIG  = 1e30;
    const double ZEPS = 1e-10;

    while (!geo.IsOutside()) {
    
        if (abs(cradius - radius) <= 1E-3 && SumL > 1000.0)
    {
        std::cout << "Neutrino Reach detector, tracking stopped"  << cradius << " " << radius << " " << abs(cradius - radius)  << std::endl;
        break; // Stopping at detector location
    }
    // volume BEFORE stepping (material you’re in)
    TGeoVolume* volBefore = geo.GetCurrentVolume();
    TGeoVolume* vol = geo.GetCurrentVolume();
    TGeoMaterial* mat = (volBefore && volBefore->GetMedium())
                            ? volBefore->GetMedium()->GetMaterial() : nullptr;

    // step to next boundary
     geo.FindNextBoundaryAndStep(BIG, kTRUE);
     double s = geo.GetStep();
    double cradius = 0;
    // guard: if we got a zero step on a boundary, nudge and try next iteration
    if (s <= ZEPS && geo.IsOnBoundary()) { nudge_into_next(); continue; }

    // log segment (after step)
    if (mat && s > ZEPS) {
        double X[3] = { geo.GetCurrentPoint()[0], geo.GetCurrentPoint()[1], geo.GetCurrentPoint()[2] };
        double r_bnd = std::sqrt(X[0]*X[0] + X[1]*X[1] + X[2]*X[2]);
        cradius=r_bnd;


        
        std::cout << std::fixed << std::setprecision(3)
            << "Boundary radius " << r_bnd
            << " Coordinates " << X[0] << " " << X[1] << " " << X[2]
            << " | Step Length: " << s << " Path Length" << SumL << " cm\n";
            std::cout << "ρ: " << mat->GetDensity()
            << " g/cm³, Z/A: " << (mat->GetA() ? mat->GetZ()/mat->GetA() : 0.0)
            << ", Material: " << mat->GetName()
            << ", Volume: " << (vol ? vol->GetName() : "NULL") << "\n\n"
            << std::boolalpha << "  Entering Volume: " << gGeoManager->IsEntering() 
            << std::boolalpha << "  Exiting Volume: " << gGeoManager->IsExiting() << "\n\n" << std::endl;
        

            SumL += s;
            ModularEarthProfile->AddPoint(SumL,mat->GetDensity());
            ModularPathL.push_back(SumL);
            ModularPathR.push_back(r_bnd);
            ModularPathDensity.push_back(mat->GetDensity());

    }

    if (geo.IsOutside()) break;

    // volume AFTER stepping (useful if you want from→to info)
    TGeoVolume* volAfter = geo.GetCurrentVolume();

    // if we’re exactly on a boundary, push into the next volume
    if (geo.IsOnBoundary()){
        std::cout << "NUDGIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIING into next boundary" << std::endl;
        nudge_into_next();

    }


    

    // (optional) explicit cross-check without flags:
    }
    /////// Comparison with OscProb

    std::cout << " " << std::endl;

    std::string PremTables = "/home/ydenizhernandez/OscProb/PremTables/";
    std::string PremFile = "prem_15layers.txt";
    std::string PremPath = PremTables+PremFile;
    OscProb::PremModel prem(PremPath);
    double cth = cos(zenith * M_PI/180.0);
    std::cout << M_PI << " " << zen_rad << " " << zenith * M_PI/180.0 << std::endl;
    prem.FillPath(cth); // Fill paths from PREM model
    std::vector<OscProb::NuPath> paths = prem.GetNuPath();

    std::cout << paths.size() << " " << ModularPathL.size() << " " << std::endl;

    for (size_t i = 0; i < ModularPathL.size(); i++)
    {
        std::cout<< i << " " << ModularPathR[i] <<  " " << ModularPathL[i] << " " << ModularPathDensity[i] << std::endl;
    }
    
    std::cout << " " << std::endl;
    
    double sumL_OscProb = 0;

    for (int i = 0; i < paths.size() ; ++i)
    {

    sumL_OscProb += paths[i].length ; 

    OscProbProfile->SetPoint(i,sumL_OscProb,paths[i].density  );
    std::cout << i << " " << sumL_OscProb << " " << paths[i].density << std::endl;

    }

    std::cout << sumL_OscProb << " " << SumL << " " << std::endl;


    
    ///////

    TCanvas* c1 = new TCanvas("c1","Paths (TMultiGraph/TGraph)", 800, 600);
    c1->cd();

    ModularEarthProfile->SetLineColor(kRed);

    ThePaths->Add(ModularEarthProfile);   // assuming ThePaths is a TMultiGraph
    ThePaths->Add(OscProbProfile);
    ThePaths->Draw("APL");                // axes, points, line
    // ThePaths->Add(OscProbProfile);     // if/when needed
    c1->Update();
    //ThePaths->Add(OscProbProfile);

    ///////
    TCanvas* c2 = new TCanvas("c2","Geometry (TGeo + 3D markers)", 900, 600);
    c2->cd();
    top->Draw("gl");
    //detMarker->Draw("same");
    NuDetMarkers->Draw("same");
    //trackingpoints->Draw("same");
    //srcDetMarkers->Draw("same");
    nuLine->Draw("same");
    TView *view = gPad->GetView();
    c2->Update();


    //view->ShowAxis();
*/