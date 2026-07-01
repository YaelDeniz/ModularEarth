#ifndef MODULAREARTH_SUBLAYER_H
#define MODULAREARTH_SUBLAYER_H
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
#include "TGeoSphere.h"
#include "TMath.h"

#include "TSystem.h"
#include "TROOT.h"

#include "Block.h"


namespace ModularEarth {

    class Sublayer {
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// \class ModularEarth::Sublayer
    ///
    /// \brief Represents a tessellated spherical shell segment within an Earth layer.
    ///
    /// The `Sublayer` class is a core component of the ModularEarth framework. ModularEarth devides 
    /// Earth layers (e.g., Crust,Upper Mantle, Lower Mantle, Outer Core, Inner Core) as into Sublayers , modeled as spherical shell.
    /// In ModularEarth, each Sublayer is subdivided into sphericall shell bins ("Blocks").
    /// Each Block corresponds to a (theta–phi) shell segment and is implemented as a ROOT `TGeoVolume`.
    /// All Blocks within a Sublayer share the same radial extent.
    ///
    /// A `Sublayer` is a `TGeoVolumeAssembly` composed of multiple Blocks. It allows users to
    /// define homogeneous or heterogeneous material properties at various levels of spatial granularity,
    /// supporting use cases such as:
    ///   - Create  regional anomalies (e.g., MTZ and Core hydration , D'')
    ///   - Localized structures (e.g., slabs, plumes, ULVZs, LLVPs)
    ///
    ///
    /// ### Key Features:
    /// - All Blocks in a given Sublayer:
    ///     - Share a common radial extent (rmin–rmax)
    ///     - Inherit a designated material at initialization
    /// - Supports targeted material updates:
    ///     - Single block material modification
    ///     - Material modification of user-defined set of Blocks 
    ///     - The entire Sublayer
    /// - Rebuilds its ROOT geometry assembly only upon explicit request (via `BuildSubLayer()`),
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
    /// Sublayer sl;
    /// sl.SetSubLayerID(...);
    /// sl.SetSubLayerMaterial(...);
    /// sl.CreateSubLayerBlocks(rmin, rmax, dphi, dtheta);
    /// sl.BuildSubLayer();
    /// ```
    ///
    ///
    /// ### Integration with ModularEarth:
    /// As of version Jan/2026, the typical usage of `ModularEarth::Sublayer` is through the `ModularEarth::Earth` class,
    /// where the Earth is constructed from `ModularEarth::Layer` objects, each composed of multiple
    /// `ModularEarth::Sublayer` instances. During construction:
    ///   - All Sublayers within the Earth model use the same angular binning (theta–phi) resolution.
    ///   - Users specify the bin size (e.g., 30° × 30°) or number of bins (e.g., Nθ × Nφ) globally.
    ///   - Mixed-binning scheme models (i.e., different binning per layer) are not currently supported.
    ///
    ///
    /// ### Notes:
    /// - A valid `TGeoManager` must be initialized before building geometry.
    /// - This class does **not** own the `TGeoManager`; it only registers volumes into the existing geometry.
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

        private:

            /// ROOT assembly containing all Blocks in this SubLayer.
            /// Owned by this class.
            TGeoVolumeAssembly* SubLayer = nullptr;

            /// Collection of Blocks forming the Sublayer.
            std::vector<Block> blocks;

            // ------------------------------------------------------------------
            // SubLayer Identification
            // ------------------------------------------------------------------

            /// Numeric Index (ID) identifier for the SubLayer (used for debugging, material summary, naming and visualization).
            int SubLayerID = 1;

            /// ROOT volume name derived from SubLayerID (e.g. "SubLayer_ID").
            std::string SubLayerName = "ClassDefaultSubLayerName";

            // ------------------------------------------------------------------
            // Default material properties (applied to all Blocks unless overridden)
            // ------------------------------------------------------------------

            std::string SubLayerMaterialName = "ClassDefaultMaterial";
            double SubLayerZ       = 0.9999;
            double SubLayerA       = 99.99;
            double SubLayerDensity = 99.99;   // g/cm^3

            // ------------------------------------------------------------------
            // Update state flags
            // ------------------------------------------------------------------

            /// True if a subset of Blocks has been modified.
            bool BlocksUpdate = false;

            /// True if all Blocks in the SubLayer have been modified.
            bool FullUpdate   = false;

            // ------------------------------------------------------------------
            // Angular binning configuration
            // ------------------------------------------------------------------

            /// Number of bins in longitude (phi).
            int Nphi = 36;

            /// Number of bins in latitud (theta).
            int Nth  = 18;

            /// Bin width in longitude (degrees).
            double dphi = 10.0;

            /// Bin width in latitud (degrees).
            double dth  = 10.0;

            // ------------------------------------------------------------------
            // Internal helpers
            // ------------------------------------------------------------------

            /// Build or rebuild the ROOT TGeoVolumeAssembly from the current Blocks.
            /// Called internally by BuildSubLayer().
            void BuildAssembly();

        public:

            // ------------------------------------------------------------------
            // Sublayer Construction
            // ------------------------------------------------------------------
            //
            //  \brief Default constructor.
            // 
            //  Creates an empty Sublayer object with default identifiers and material
            //  parameters. No geometry is created at construction time.
            //  Geometry is created only after calling:
            //    - CreateSubLayerBlocks(...)
            //    - BuildSubLayer()
            //
            Sublayer() = default;

            
            //  \brief Assign a numeric ID to this SubLayer.
            //
            //  The ID is used to generate a unique ROOT volume name and can also be
            //  used for visualization, material summary or bookkeeping purposes.
            
            void SetSubLayerID(int id);

            // ------------------------------------------------------------------
            // Geometry and Material Definition
            // ------------------------------------------------------------------

      
            //  \brief Create all Blocks composing this SubLayer.
            
            //  The SubLayer is discretized into theta–phi bins covering the full sphere.
            //  Each bin becomes a Block with identical radial extent but distinct angular
            //  boundaries.
             
            //  @param r1   Inner radius of the SubLayer.
            //  @param r2   Outer radius of the SubLayer.
            //  @param dth  colatitud bin width (degrees). Must divide 180 exactly.
            //  @param dphi Longitude bin width (degrees). Must divide 360 exactly.
            
             
            //Notes:
            //- Blocks inherit the current SubLayer material.
            //- Block volumes are created, but the SubLayer assembly is not built until BuildSubLayer() is called.
             
            void CreateSubLayerBlocks(double r1, double r2, double dphi, double dth);

           
            //  \brief Set the default material for the entire SubLayer.
            
            //  This material will be applied to all Blocks created after this call.
            //  Existing Blocks are not updated unless UpdateAllBlocksMaterial(...) is used.
             
            void SetSubLayerMaterial(const std::string& name,
                                    double A, double Z, double density);

            // ------------------------------------------------------------------
            // Material updates (localized heterogeneities)
            // ------------------------------------------------------------------

            //  \brief Update material properties of a single Block (ID-based).
            
            //The corresponding Block volume is rebuilt immediately.
             
            void UpdateBlockMaterial(int id,
                                    const std::string& NewName,
                                    double NewA, double NewZ, double NewDensity);

     
            // \brief Update material properties of a user-defined set of Blocks.
             
            //  Invalid block IDs are skipped with a warning.
            
            void UpdateMultipleBlocksMaterial(const std::vector<int>& ids,
                                            const std::string& NewName,
                                            double NewA, double NewZ,
                                            double NewDensity);

         
            //  \brief Update material properties of all Blocks in the SubLayer.
       
            //  This operation homogenizes the SubLayer and marks it as fully updated.
    
            void UpdateAllBlocksMaterial(const std::string& NewName,
                                        double NewA, double NewZ,
                                        double NewDensity);

            // ------------------------------------------------------------------
            // Geometry construction
            // ------------------------------------------------------------------

            
            //  \brief Build or rebuild the ROOT geometry assembly for this SubLayer.
       
            //  This assembles all Block volumes into a TGeoVolumeAssembly.
            //  A valid TGeoManager must exist prior to calling this method.
           
            void BuildSubLayer();

            // ------------------------------------------------------------------
            // Accessors
            // ------------------------------------------------------------------

            /// Return the ROOT volume assembly representing this SubLayer.
            TGeoVolumeAssembly* GetSubLayerVolume() const;

            // ------------------------------------------------------------------
            // Diagnostics and debugging
            // ------------------------------------------------------------------

            /// Print detailed information for a specific Block (1-based ID).
            void GetBlockSummary(int id);

            /// Print summaries for all Blocks in the SubLayer.
            void GetAllBlockSummary();

            /// Print a concise summary of SubLayer properties and update state.
            void GetSubLayerSummary();

            void GetBlockVertices(TGeoManager* mgr, TString SubLayerNode_path) const;
    };




};

#endif