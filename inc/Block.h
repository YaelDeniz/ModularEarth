#ifndef MODULAREARTH_BLOCK_H
#define MODULAREARTH_BLOCK_H
///////////////////////////////////////////////////////////////////////////////
/// \class ModularEarth::Block
///
///    The smallest building unit of the tessellated Earth model.
///    A Block represents a spherical shell segment with user-defined size
///    and material properties (density, Z, A).
///    Material properties can be modified at any time after construction.
/// \author Yael Deniz (yaeldeniz0499@gmail.com)
///////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>

#include "TGeoManager.h"
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


namespace  ModularEarth {

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

            // Block Geometry Parameters
            double rmin = 0, rmax = 1; // Inner (rmin) and outer (rmax) radius of the block
            double thetamin = 0, thetamax = 45; // Block is constructed for \theta \in [thetamin, thetamax]
            double phimin = 0, phimax = 25; // Block is constructed for \phi \in [phimin, phimax]
            
            // Material Parameters
            double atomicMass = 0; // Atomic Mass (A) of volume material
            double atomicNumber = 0; // Atomic Number (Z) of volume material
            double density = 0; // Density (ρ) of volume medium
            std::string materialName = "Vacuum"; // Name of block material, by defualt it is set as vacuum.

            // Hierarchical indices (1-based, set by SetBlockGeometry).
            // Copy numbers passed to TGeoManager::AddNode equal these values,
            // so ROOT navigation recovers them directly via GetCopyNumber().
            int i_blk = 1; // Block index within its sublayer
            int i_sly = 0; // Sublayer index within its layer
            int i_ly  = 0; // Layer index within the Earth model

            // Modification flags
            bool BlockModified = false; // Flag that indicates if material of a specific set of blocks was modified
            bool SubLayerModified = false; //Flag that indicates if material of all the blocks in a sublayer was modified
            int colorIndex = 1; // Index used to highlight modified blocks with different colors

        public: 
        
            // ------------------------------------------------------------------
            // Construction
            // ------------------------------------------------------------------

            /// Default constructor.
            /// Initializes the block with default geometry and material parameters
            /// defined in the private section. Geometry and material must be set
            /// explicitly before building the Block.
            Block() = default;

            // Move constructor and assignment are explicitly noexcept so that
            // std::vector<Block> always moves (not copies) during reallocation.
            // Without the explicit noexcept, the compiler-generated move inherits
            // the noexcept status of std::string (not guaranteed on all platforms),
            // causing std::vector to fall back to the copy constructor — which
            // shallow-copies TGeoVolume* and crashes inside ROOT's object tracking.
            Block(Block&&) noexcept = default;
            Block& operator=(Block&&) noexcept = default;

            // The copy constructor is kept (not deleted) because ROOT's dictionary
            // generator (rootcling) requires copy-constructible types for vector I/O.
            // Do not call it in application code: use std::move() instead.
            Block(const Block&) = default;
            Block& operator=(const Block&) = default;


            // ------------------------------------------------------------------
            // Modification Flags
            // ------------------------------------------------------------------

            /// Mark this specific block as modified.
            /// Used to track and visually highlight blocks whose material
            /// properties differ from the default configuration.
            void BlockWasModified(bool status);

            /// Mark all blocks within the parent sublayer as modified.
            /// Typically used when applying a uniform material update to
            /// an entire sublayer.
            void SubLayerWasModified(bool status);


            // ------------------------------------------------------------------
            // Geometry and Material Definition
            // ------------------------------------------------------------------

            /// Define the geometric boundaries of the block and assign its hierarchical indices.
            ///
            /// Parameters:
            ///   r, R           : Inner and outer radii of the spherical shell segment
            ///   theta1,theta2  : Colatitude range [degrees]
            ///   phi1,phi2      : Longitude range [degrees]
            ///   i_blk_         : 1-based block index within its sublayer
            ///   i_ly_          : 1-based layer index (passed down from Layer via Sublayer)
            ///   i_sly_         : 1-based sublayer index within that layer
            ///
            /// Must be called before DefineBlockMaterial() so the systematic ROOT
            /// object names (BlockMat/BlockMed/BlockVol/BlockShape) are correct.

            void SetBlockGeometry(double r, double R,
                                double theta1, double theta2,
                                double phi1, double phi2,
                                int i_blk_, int i_ly_, int i_sly_);

            /// Define the material properties of the block.
            ///
            /// Parameters:
            ///   name : Material name
            ///   A    : Atomic mass
            ///   Z    : Atomic number
            ///   rho  : Density (g/cm^3)
            ///
            /// This method must be called before BuildBlock().
            void DefineBlockMaterial(const std::string& name,
                                    double A,
                                    double Z,
                                    double rho);


            // ------------------------------------------------------------------
            // Geometry Construction
            // ------------------------------------------------------------------

            /// Build the ROOT geometry volume associated with this block.
            ///
            /// This creates the TGeoShape, TGeoVolume, and assigns visualization
            /// properties based on modification flags.
            /// A valid TGeoManager must exist prior to calling this method.
            void BuildBlock(TGeoMedium* medium);


            // ------------------------------------------------------------------
            // Validation and Accessors
            // ------------------------------------------------------------------

            /// Check whether the block is fully and consistently defined.
            ///
            /// Returns true if geometry, material, and volume have been
            /// successfully created and all parameters are physically valid.
            bool IsValid() const;
            void InvalidBlockSummary(std::ostream& os) const;

            /// Retrieve the ROOT volume associated with this block.
            ///
            /// Used when attaching the block to higher-level structures
            /// such as Sublayers or Layers.
            TGeoVolume* GetVolume() const;

            /// Return the 1-based block index within its sublayer.
            int GetBlockIndex() const;

            /// Return the 1-based sublayer index (set by parent Sublayer).
            int GetSublayerIndex() const;

            /// Return the 1-based layer index (set by parent Layer via Sublayer).
            int GetLayerIndex() const;

            /// Deprecated alias for GetBlockIndex() — kept for Sublayer.C compatibility.
            int GetBlockID() const;

            bool IsBlockModified() const;
            bool IsSubLayerModified() const ;

            double      GetDensity()      const { return density; }
            double      GetAtomicNumber() const { return atomicNumber; }
            std::string GetMaterialName() const { return materialName; }
            double      GetEYield()       const { return atomicNumber / atomicMass; }

            // ------------------------------------------------------------------
            // Diagnostics and Debugging
            // ------------------------------------------------------------------

            /// Print a detailed summary of the block geometry, material,
            /// modification flags, and internal ROOT pointers.
            void BlockSummary() const;

    };

};

#endif