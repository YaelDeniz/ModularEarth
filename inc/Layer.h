#ifndef MODULAREARTH_LAYER_H
#define MODULAREARTH_LAYER_H
///////////////////////////////////////////////////////////////////////////////
/// \author Yael Deniz (yaeldeniz0499@gmail.com)
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>

#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoVolume.h"
#include "TGeoShape.h"
#include "TGeoMatrix.h"
#include "TH1F.h"
#include "TPolyLine3D.h"
#include "TPolyMarker3D.h"
#include "TPolyLine3D.h"
#include "TView.h"
#include "TCanvas.h"
#include "TGraph.h"
//OscProb
#include "Sublayer.h"



namespace ModularEarth {

    // The SublayerConfig struct stores all parameters required to define a
    // single SubLayer within a ModularEarth::Layer.
    //
    // The structure has the following form:
    //
    //   SublayerConfig = { int id, double r1, double r2, double rho,
    //                      double A, double Z, std::string name }
    //
    // Each SublayerConfig entry defines one radial sublayer of the current Layer.
    //
    // Parameters:
    //   id    - Unique identifier of the SubLayer (eg., 1-N).
    //   r1    - Inner radius of the SubLayer.
    //   r2    - Outer radius of the SubLayer.
    //   rho   - Density of the SubLayer material (g/cm³).
    //   A     - Atomic mass of the SubLayer material.
    //   Z     - Atomic number of the SubLayer material.
    //   name  - Material name of the SubLayer.
    //
    // Multiple SubLayers can be added to a Layer using Layer::SetSubLayers().
    // The input is a vector of SublayerConfig objects.
    //
    // Example:
    //
    //   Layer ToyLayer("ToyLayer", LayerID, dth, dphi);
    //
    //   std::vector<SublayerConfig> sublayersConfig = {
    //     {1, r1, r2, rho, A, Z, "InnerCore_SubLayer"},   // InnerCore SubLayer #1
    //     {2, r1, r2, rho, A, Z, "InnerCore_SubLayer"},   // InnerCore SubLayer #2
    //     {3, r1, r2, rho, A, Z, "InnerCore_SubLayer"},      // InnerCore SubLayer #3
    //     ...
    //     {N, r1, r2, rho, A, Z, "InnerCore_SubLayer"}        // InnerCore SubLayer #N
    //   };
    //
    //   ToyLayer.SetSubLayers(sublayersConfig);

    struct SublayerConfig {
        double rmin; // Inner radius of sublayer
        double rmax; // Outer radius of sublayer
        double density; // g/cm³
        double atomicMass; // A
        double atomicNumber; // Z
        int layerID;
        std::string materialName; // Optional, default = "LayerMaterial"

        SublayerConfig() = default;

        SublayerConfig(int id, double r1, double r2, double rho, double A, double Z, std::string name = "SubLayerMaterial"): layerID(id), rmin(r1), rmax(r2), density(rho), atomicMass(A), atomicNumber(Z), materialName(name) {}
    };

    class Layer {
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// \class ModularEarth::Layer
    ///
    /// \brief Represents an assembly of Sublayers within specific layer of an Earth model.
    ///
    /// The `Layer` class is a `TGeoVolumeAssembly` representing the main geological layers
    /// of an Earth model within the ModularEarth framework. ModularEarth divides the Earth into
    /// large-scale layers (e.g., Crust, Upper Mantle, Lower Mantle, Outer Core, Inner Core), which
    /// are further subdivided into Sublayers.
    ///
    /// In other words, In ModularEarth, each `Layer` is composed of multiple `Sublayer` volumes.
    /// Each Sublayer has its own material properties and radial extent, while all Sublayers
    /// within a given Earth model share a common angular binning scheme.
    ///
    ///ModularEarth::Layer  allows users to define
    /// the main Earth regions of a geophysical model and to localize heterogeneities within
    /// their sublayers, supporting use cases such as:
    ///   - Classification of large-scale Earth regions (e.g., Crust, Upper Mantle, Lower Mantle,
    ///     Outer Core, Inner Core)
    ///   - Localized structures within Earth regions via Sublayer modifications
    ///     (e.g., slabs, plumes, ULVZs, LLVPs)
    ///
    ///
    /// ### Key Features:
    /// - All  updates for a given sublayer of Layer are registered and applied from:
    ///     - `UpdateBlocksInSubLayer` stored in the input configuration (`SublayerConfig`)
    ///       where:
    ///         SublayerConfig(SubLayerIndex, r1, r2, newRho, newA, newZ, "NewSubLayerMaterial")
    ///
    ///   - For multiple updates, the user provides a vector of `SublayerConfig` entries,
    ///     each defining a block-level material modification.
    ///
    /// - For full Sublayer modification, the user must use:
    ///     `UpdateSublayer(size_t sublayerIndex,
    ///                    const std::string& newName,
    ///                    double newA,
    ///                    double newZ,
    ///                    double newDensity)`
    ///
    /// - All Sublayers in a given Layer:
    ///     - Are initialized from the input configuration (`SublayerConfig`)
    ///     - Share a common angular binning scheme (number of bins and bin size)
    ///
    /// - Each Sublayer in a given Layer:
    ///     - Has its own material properties (density, atomic composition, electron yield)
    ///
    /// - The Layer class supports targeted material updates:
    ///     - Single block material modification for a specific Sublayer
    ///     - Material modification of a user-defined set of blocks across different Sublayers
    ///     - Modification of entire Sublayers
    ///
    /// - Rebuilds its ROOT geometry assembly only upon explicit request (via `BuildLayer()`),
    ///   reducing unnecessary geometry updates
    ///
    ///
    /// ### Coordinate Conventions:
    /// - `theta` : colatitude (in degrees), range [0°, 180°] (North → South)
    /// - `phi`   : longitude (in degrees), range [0°, 360°) (East → West)
    ///
    ///
    /// ### Typical Usage:
    /// ```cpp
    /// ModularEarth::Layer layer;
    /// layer.SetLayerID(...);
    /// layer.SetLayerMaterial(...);
    /// layer.CreateLayer(rmin, rmax, dphi, dtheta);
    /// layer.BuildLayer();
    /// ```
    ///
    ///
    /// ### Integration with ModularEarth:
    /// As of January 2026, the typical usage of `ModularEarth::Layer` is through the
    /// `ModularEarth::Earth` class, where the Earth model is constructed from
    /// `ModularEarth::Layer` objects, each composed of multiple Sublayers. During construction:
    ///
    ///   - All Layers within the Earth model use the same angular binning (theta–phi) resolution.
    ///   - Users specify the bin size (e.g., 30° × 30°) or number of bins (e.g., Nθ × Nφ) globally.
    ///   - Mixed-binning Earth models (i.e., different binning per Layer) are not currently supported.
    ///
    ///
    /// ### Notes:
    /// - A valid `TGeoManager` must be initialized before building geometry.
    /// - This class does **not** own the `TGeoManager`; it only registers volumes into the
    ///   existing geometry hierarchy.
    ///
    /// /////////////////////////////////////////////////////////////////////////////////////////////////////////


        private: 

            /// ROOT assembly containing all SubLayers in a Layer.
            /// Owned by this class.
            TGeoVolumeAssembly * layerVolume = nullptr;

            /// Collection of Sublayers forming the Layer.
            std::vector < Sublayer > sublayers;

            // ------------------------------------------------------------------
            // SubLayer Identification
            // ------------------------------------------------------------------

            /// Numeric Index (ID) identifier for the SubLayer (used for debugging, material summary, naming and visualization).
            int layerID = 999;

            /// ROOT volume name derived from layerID (e.g. "layer_ID").
            ///std::string layerTitle = "DefaultEarthLayer";
            std::string layerName_string;
            std::string layerName;

            // ------------------------------------------------------------------
            // Update state flags
            // ------------------------------------------------------------------

            //    ...

            // ------------------------------------------------------------------
            // Angular binning configuration
            // ------------------------------------------------------------------

            /// Bin width in longitude (degrees).
            double dphi;

            /// Bin width in latitud (degrees).
            double dth;

            // ------------------------------------------------------------------
            // Internal helpers
            // ------------------------------------------------------------------

        public:

            // ------------------------------------------------------------------
            // Layer Construction
            // ------------------------------------------------------------------
            //
            //  \brief Default constructor.
            // 
            //  Creates an empty Layer object with default identifiers 
            //  parameters. No geometry is created at construction time.
            //  Geometry is created only after calling:
            //    - BuildLayer()
        
            Layer(std::string name = "DefaultLayer",
            int id = 99,
            double dthsize = 10.0,
            double dphisize = 10.0)
            : layerID(id),
            layerName_string(std::move(name)),
            layerName(layerName_string + "_" + std::to_string(layerID)),
            dphi(dphisize),
            dth(dthsize)
            {}


            // -----------------------------------------------------------------------------
            // Geometry and Material Definition
            // -----------------------------------------------------------------------------

            // For SubLayers within Layer in Modular Earth, 
            // Geometry and material properties of SubLayers are defined using the
            // ModularEarth::SublayerConfig structure.
            //
            // The SublayerConfig struct stores all parameters required to define a
            // single SubLayer within a ModularEarth::Layer.
            //
            // The structure has the following form:
            //
            //   SublayerConfig = { int id, double r1, double r2, double rho,
            //                      double A, double Z, std::string name }
            //
            // Each SublayerConfig entry defines one radial sublayer of the current Layer.
            //
            // Parameters:
            //   id    - Unique identifier of the SubLayer (eg., 1-N).
            //   r1    - Inner radius of the SubLayer.
            //   r2    - Outer radius of the SubLayer.
            //   rho   - Density of the SubLayer material (g/cm³).
            //   A     - Atomic mass of the SubLayer material.
            //   Z     - Atomic number of the SubLayer material.
            //   name  - Material name of the SubLayer.
            //
            // Multiple SubLayers can be added to a Layer using Layer::SetSubLayers().
            // The input is a vector of SublayerConfig objects.
            //
            // Example:
            //
            //   Layer ToyLayer("ToyLayer", LayerID, dth, dphi);
            //
            //   std::vector<SublayerConfig> sublayersConfig = {
            //     {1, r1, r2, rho, A, Z, "InnerCore_SubLayer"},   // InnerCore SubLayer #1
            //     {2, r1, r2, rho, A, Z, "InnerCore_SubLayer"},   // InnerCore SubLayer #2
            //     {3, r1, r2, rho, A, Z, "InnerCore_SubLayer"},      // InnerCore SubLayer #3
            //     ...
            //     {N, r1, r2, rho, A, Z, "InnerCore_SubLayer"}        // InnerCore SubLayer #N
            //   };
            //
            //   ToyLayer.SetSubLayers(sublayersConfig);
            //
            // NOTE: The geometry volumes are not created until BuildLayer() is called.

            
            void SetSubLayers(const std::vector < SublayerConfig > & configs) ;

            // ------------------------------------------------------------------
            // Material updates 
            // ------------------------------------------------------------------
            //
            // 1.- Localized heterogeneities
            //
            // Brief: Update material properties of selected Blocks within a given SubLayer.
            //
            // A SubLayer is discretized into theta–phi Blocks covering the full spherical shell.
            // As described in the Block and SubLayer documentation, each Block is identified by a
            // unique ID. This ID allows the user to access and modify material properties of
            // specific Blocks within a given SubLayer.
            //
            // The combination of SubLayer index and Block IDs enables localized material
            // modifications within an Earth model. At the Layer level, similar modifications
            // can be applied via Layer::UpdateBlocksInSubLayer(...).
            //
            // Parameters:
            //   sublayerIndex   Index of the target SubLayer.
            //   blockIDs       Vector of Block IDs specifying which Blocks will be modified.
            //   newName        New material name assigned to the selected Blocks.
            //   newA           New atomic mass assigned to the selected Blocks.
            //   newZ           New atomic number assigned to the selected Blocks.
            //   newDensity     New density assigned to the selected Blocks.
            //
            // Note:
            //
            //   If different materials must be assigned to different Blocks within the same
            //   SubLayer, the user must group Blocks by material and apply updates separately.
            //
            // Note:
            //   The volume of each modified Block is rebuilt immediately after the update.

            void UpdateBlocksInSubLayer(size_t sublayerIndex,
                const std::vector < int > & blockIDs,
                    const std::string & newName,
                        const std::vector < double > & newA,
                            const std::vector < double > & newZ,
                                const std::vector < double > & newDensity) ;

            //
            // 2.- SubLayer-scale modifications
            //
            // Brief: Update material properties of an entire SubLayer.
            //
            // Using the SubLayer index enables full-scale material modifications of a SubLayer.
            // At the Layer level, similar modifications can be applied via Layer::UpdateSubLayer(...).
            //
            // Parameters:
            //   sublayerIndex   Index of the target SubLayer.
            //   newName         New material name assigned to the SubLayer.
            //   newA            New atomic mass assigned to the SubLayer.
            //   newZ            New atomic number assigned to the SubLayer.
            //   newDensity      New density assigned to the SubLayer.
            //
            // Note:
            //   The volume of each modified SubLayer is rebuilt immediately after the update.
            
            void UpdateSublayer(size_t sublayerIndex,
                const std::string & newName,
                    double newA,
                    double newZ,
                    double newDensity) ;

            // ------------------------------------------------------------------
            // Geometry construction
            // ------------------------------------------------------------------

            
            //  \brief Build or rebuild the ROOT geometry assembly for this SubLayer.
       
            //  This assembles all Block volumes into a TGeoVolumeAssembly.
            //  A valid TGeoManager must exist prior to calling this method.

            void BuildLayer() ;

            // ------------------------------------------------------------------
            // Accessors
            // ------------------------------------------------------------------
            TGeoVolumeAssembly * GetLayerVolume() const ;

            // ------------------------------------------------------------------
            // Diagnostics and debugging
            // ------------------------------------------------------------------

            void SubLayerSummary(size_t sublayerIndex) const ;

            void BlockSummary(size_t sublayerIndex, int blockIndex) const ;

            void LayerSummary() const ;
    };

}


#endif



 