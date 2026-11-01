#ifndef MODULAREARTH_EARTHPATHS_H
#define MODULAREARTH_EARTHPATHS_H
///////////////////////////////////////////////////////////////////////////////
/// \author Yael Deniz (yaeldeniz0499@gmail.com)
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>

#include "TGeoManager.h"
#include "TGeoVolume.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoShape.h"
#include "TGeoSphere.h"
#include "TPolyLine3D.h"
#include "TPolyMarker3D.h"
#include "TVector3.h"
#include "TCanvas.h"
#include "TMath.h"

namespace ModularEarth {

    // One segment of the neutrino geometric path through the Earth model.
    // Filled by EarthPaths::GetSegments().
    struct PathSegment {
        int    step;       // Step number (1-based)
        double distance;   // Distance traversed through this block [km]
        double cumulative; // Cumulative path length up to and including this block [km]
        int    i_blk;      // Block index within its sublayer (1-based)
        int    i_sly;      // Sublayer index within its layer (1-based)
        int    i_ly;       // Layer index within the Earth model (1-based)
        std::string volName;  // ROOT volume name
        std::string matName;  // Material name
        double density;       // Material density [g/cm³]
        double Z;             // Atomic number of material
        double A;             // Atomic mass of material [g/mol]
    };

    class EarthPaths {
    ///////////////////////////////////////////////////////////////////////////
    ///
    /// \class ModularEarth::EarthPaths
    ///
    /// Computes the geometric path of a neutrino through a ModularEarth
    /// geometry and stores per-block segment data (distance, block index,
    /// sublayer index, layer index).
    ///
    /// Requires an already-closed TGeoManager (call Earth::BuiltEarth()
    /// before constructing EarthPaths). The TGeoManager pointer is obtained
    /// from Earth::GetGeoManager().
    ///
    /// Typical usage:
    ///   EarthPaths paths(earth.GetGeoManager());
    ///   paths.SetDetectorLocation(lon, lat, R_det);
    ///   paths.GetSegments(zenith_deg, azimuth_deg);
    ///   paths.PrintSegments();
    ///   paths.DisplayVectors();
    ///
    ///////////////////////////////////////////////////////////////////////////

    private:
        TGeoManager* geoManager = nullptr;

        // Atmospheric sphere radius — neutrino source is placed here [km]
        static constexpr double R_ATM = 6386.0;

        // Model-frame position vectors (Earth center as origin) [km]
        TVector3 r_det; // Detector location
        TVector3 r_src; // Neutrino entry point at the atmospheric sphere

        // Neutrino propagation direction (unit vector, source → detector)
        TVector3 n_dir;

        // Detector local coordinate frame
        TVector3 x_hat; // Local East
        TVector3 y_hat; // Local North
        TVector3 z_hat; // Radial outward (= r_det.Unit())

        // Path segments accumulated by the last GetSegments() call
        std::vector<PathSegment> segments;

        // Visualization objects
        TPolyLine3D*   nuPath    = nullptr;
        TPolyMarker3D* markerDet = nullptr;
        TPolyMarker3D* markerSrc = nullptr;
        TPolyLine3D*   dirArrow  = nullptr;

        // Solve |r_det - t*n_dir|² = R_ATM² and set r_src to the farther root.
        // Must be called after n_dir is set.
        void LocateNuSource();

    public:

        // geoManager must point to a fully built geometry (CloseGeometry done).
        explicit EarthPaths(TGeoManager* mgr);

        // Set the detector geographic position.
        //   lon  : longitude [degrees]
        //   lat  : latitude  [degrees] (positive = North)
        //   R_det: distance from Earth center [km]
        // Also builds the detector local coordinate frame (x_hat, y_hat, z_hat).
        void SetDetectorLocation(double lon, double lat, double R_det);

        // Compute path segments for a neutrino arriving from direction (th, phi)
        // expressed in the detector local frame.
        //   th  : zenith angle measured from straight-up (th=0 → vertical down-going)
        //   phi : azimuth angle
        //   radians: if false (default) angles are in degrees
        void GetSegments(double th, double phi, bool radians = false);

        // Print a formatted table of all segments to stdout.
        // Columns: Step | Distance[km] | Block | Sublayer | Layer | Material | Density[g/cm³]
        void PrintSegments() const;

        // Draw the Earth geometry, atmosphere boundary sphere, neutrino path line,
        // source and detector markers, and propagation direction arrow.
        // Must be called after GetSegments().
        void DisplayVectors();

        const std::vector<PathSegment>& GetPathSegments() const { return segments; }
    };

} // namespace ModularEarth

#endif
