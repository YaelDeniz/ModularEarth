///////////////////////////////////////////////////////////////////////////////
/// \author Yael Deniz (yaeldeniz0499@gmail.com)
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

#include "TGeoManager.h"
#include "TGeoVolume.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoShape.h"
#include "TGeoSphere.h"
#include "TGeoMatrix.h"
#include "TPolyLine3D.h"
#include "TPolyMarker3D.h"
#include "TVector3.h"
#include "TCanvas.h"
#include "TMath.h"

#include "EarthPaths.h"

namespace ModularEarth {

    EarthPaths::EarthPaths(TGeoManager* mgr) : geoManager(mgr) {
        if (!geoManager) {
            std::cerr << "[EarthPaths] ERROR: null TGeoManager pointer.\n";
        }
    }

    // -------------------------------------------------------------------------

    void EarthPaths::SetDetectorLocation(double lon, double lat, double R_det) {
        double lon_rad = lon * TMath::Pi() / 180.0;
        double lat_rad = lat * TMath::Pi() / 180.0;

        double x = R_det * std::cos(lat_rad) * std::cos(lon_rad);
        double y = R_det * std::cos(lat_rad) * std::sin(lon_rad);
        double z = R_det * std::sin(lat_rad);

        r_det.SetXYZ(x, y, z);
        locationSet = true;

        // Local +z = radial outward
        z_hat = r_det.Unit();

        // Local +x = East; avoid degeneracy at poles
        if (std::fabs(z_hat.Z()) > 0.999) {
            x_hat = TVector3(1, 0, 0).Cross(z_hat).Unit();
        } else {
            x_hat = TVector3(0, 0, 1).Cross(z_hat).Unit();
        }

        // Local +y = North
        y_hat = z_hat.Cross(x_hat).Unit();
    }

    // -------------------------------------------------------------------------

    void EarthPaths::LocateNuSource() {
        // Solve |r_det - t*n_dir|^2 = R_ATM^2 for t
        double a   = 1.0;
        double b   = -2.0 * r_det.Dot(n_dir);
        double c   = r_det.Mag2() - R_ATM * R_ATM;
        double disc = b * b - 4.0 * a * c;

        if (disc < 0.0) {
            std::cerr << "[EarthPaths] No intersection with atmospheric sphere (R="
                      << R_ATM << " km). Check detector location and direction.\n";
            return;
        }

        double sqrt_disc = std::sqrt(disc);
        double t1 = (-b + sqrt_disc) / 2.0;
        double t2 = (-b - sqrt_disc) / 2.0;

        // The source is the farther intersection (neutrino enters from outside)
        double t_entry = std::max(t1, t2);
        r_src = r_det - t_entry * n_dir;
    }

    // -------------------------------------------------------------------------

        void EarthPaths::GetSegments(double th, double phi, bool radians) {
        if (!geoManager) {
            std::cerr << "[EarthPaths] GetSegments: no geometry loaded.\n";
            return;
        }
        if (!locationSet) {
            std::cerr << "[EarthPaths] GetSegments: detector location not set — "
                      << "call SetDetectorLocation() first.\n";
            return;
        }

        // Clamp th away from exactly 0 and 180 (degrees) or 0 and pi (radians).
        // A perfectly vertical direction places r_src at a pole of the angular
        // tiling, which is a degenerate vertex shared by all phi-bin boundaries
        // and guaranteed to trigger the boundary-straddling failure.
        const double TH_EPS = 1e-4; // degrees
        if (!radians) {
            if (th >= 180.0 - TH_EPS) th = 180.0 - TH_EPS;
            if (th <=         TH_EPS) th =         TH_EPS;
        } else {
            const double erad = TH_EPS * TMath::Pi() / 180.0;
            if (th >= TMath::Pi() - erad) th = TMath::Pi() - erad;
            if (th <=              erad)  th =               erad;
        }

        double th_rad, phi_rad;
        if (!radians) {
            th_rad  = (180.0 - th)  * TMath::Pi() / 180.0;
            phi_rad = phi * TMath::Pi() / 180.0;
        } else {
            th_rad  = TMath::Pi() - th;
            phi_rad = phi;
        }

        // Build global direction from detector local frame
        TVector3 ndir_local = x_hat * (std::sin(th_rad) * std::cos(phi_rad))
                            + y_hat * (std::sin(th_rad) * std::sin(phi_rad))
                            + z_hat * (std::cos(th_rad));
        n_dir = ndir_local.Unit();

        LocateNuSource();

        // Total straight-line distance from atmospheric entry to detector
        const double total_dist = (r_det - r_src).Mag();
        

        segments.clear();

        // Initialize ROOT track from r_src toward the detector
        geoManager->InitTrack(r_src.X(), r_src.Y(), r_src.Z(),
                              n_dir.X(), n_dir.Y(), n_dir.Z());

        // Localize the track in the geometry hierarchy (required after InitTrack)
        geoManager->FindNode();

        // --- DEBUG: confirm which volume ROOT assigned the starting point to ---
        {
            TGeoVolume* startVol = geoManager->GetCurrentVolume();
            const double* sp = geoManager->GetCurrentPoint();
            double sR = std::sqrt(sp[0]*sp[0] + sp[1]*sp[1] + sp[2]*sp[2]);
            std::cout << "[DEBUG] After FindNode(): volume = \""
                      << (startVol ? startVol->GetName() : "NULL")
                      << "\"  |r_src| = " << sR
                      << "  IsOutside=" << geoManager->IsOutside()
                      << "  IsOnBoundary=" << geoManager->IsOnBoundary() << "\n";
        }
        // -----------------------------------------------------------------------

        // Returns true iff the navigator is currently inside a named BlockVol.
        // Catches (a) IsOnBoundary()=true, (b) silent vacuum assignment to
        // EarthTopSphere, and (c) phi/theta/vertex boundary — all three failure
        // modes that can cause the track to skip layers at initialization.
        auto is_in_block_vol = [&]() -> bool {
            if (geoManager->IsOutside()) return false;
            TGeoVolume* v = geoManager->GetCurrentVolume();
            if (!v) return false;
            int il = 0, isl = 0, ibl = 0;
            return std::sscanf(v->GetName(),
                               "BlockVol_%d_%d_%d", &il, &isl, &ibl) == 3;
        };

        const double EPSNUDGE          = 1e-5; // cm — above ROOT's ~1e-9 cm boundary tolerance
        const int    MAX_NUDGE_RETRIES = 20;

        // Nudge the track forward along the current direction and re-locate.
        // With maxRetries > 1 keeps nudging until inside a valid BlockVol or
        // until the limit is reached (needed for degenerate boundary vertices).
        // Default maxRetries=1 preserves the original single-step behaviour used
        // by the post-step boundary call at the bottom of the loop.
        auto nudge_into_next = [&](int maxRetries = 1) {
            for (int r = 0; r < maxRetries; ++r) {
                const double* X = geoManager->GetCurrentPoint();
                const double* D = geoManager->GetCurrentDirection();
                geoManager->SetCurrentPoint(X[0] + EPSNUDGE * D[0],
                                            X[1] + EPSNUDGE * D[1],
                                            X[2] + EPSNUDGE * D[2]);
                geoManager->FindNode();
                if (geoManager->IsOutside() || is_in_block_vol()) break;
            }
        };

        // Robust initialization: nudge until inside a BlockVol.
        // When r_src is already cleanly placed (the common case), is_in_block_vol()
        // is true immediately and zero nudges are applied.
        if (!is_in_block_vol()) {
            nudge_into_next(MAX_NUDGE_RETRIES);
        }
        if (!is_in_block_vol() && !geoManager->IsOutside()) {
            TGeoVolume* cv = geoManager->GetCurrentVolume();
            std::cerr << "[EarthPaths] WARNING: after " << MAX_NUDGE_RETRIES
                      << " init retries, track not in a BlockVol. Volume: \""
                      << (cv ? cv->GetName() : "NULL") << "\""
                      << "  IsOutside="    << geoManager->IsOutside()
                      << "  IsOnBoundary=" << geoManager->IsOnBoundary() << "\n";
        }

        const double ZEPS = 1e-10;
        int    stepN = 0;
        double sumL  = 0.0;

        while (!geoManager->IsOutside()) {
            // Capture volume and material BEFORE stepping — this is the volume
            // currently being traversed, whose density belongs to the next segment
            TGeoVolume* vol = geoManager->GetCurrentVolume();
            if (!vol) break;

            TGeoMedium*   med  = vol->GetMedium();
            TGeoMaterial* mat  = med ? med->GetMaterial() : nullptr;
            std::string   vName = vol->GetName();

            //Debug-----------------------------------------------------------------
            const double* raw = geoManager->GetCurrentPoint();
            double Xi[3] = { raw[0], raw[1], raw[2] };   // safe copy
            double Ri = std::sqrt(Xi[0]*Xi[0] + Xi[1]*Xi[1] + Xi[2]*Xi[2]);
            //----------------------------------------------------------------------
            geoManager->FindNextBoundaryAndStep();
            double step = geoManager->GetStep();

            // Zero step on a boundary — push into the next volume and retry
            if (step <= ZEPS && geoManager->IsOnBoundary()) {
                nudge_into_next(MAX_NUDGE_RETRIES);
                if (!is_in_block_vol() && !geoManager->IsOutside()) {
                    TGeoVolume* sv = geoManager->GetCurrentVolume();
                    std::cerr << "[EarthPaths] WARNING: stall at cumL=" << sumL
                              << " km — after " << MAX_NUDGE_RETRIES
                              << " retries still not in a BlockVol. Volume: \""
                              << (sv ? sv->GetName() : "NULL") << "\". Continuing.\n";
                }
                continue;
            }

            // Stepping has broken down: ROOT returned a non-positive step outside
            // of a boundary stall. This signals a geometry error, an infinite-loop
            // detected internally by TGeoManager, or the track already being outside
            // the geometry. Break out cleanly to avoid an infinite loop.
            if (step <= 0.0) {
                std::cerr << "[EarthPaths] WARNING: non-positive step (" << step
                          << " km) at sumL=" << sumL << " km — geometry error or "
                          << "track already outside. Stopping track.\n";
                break;
            }

            sumL += step;

            int  il = 0, isl = 0, ibl = 0;
            bool isBlock = (std::sscanf(vName.c_str(),
                                        "BlockVol_%d_%d_%d", &il, &isl, &ibl) == 3);

            if (isBlock && mat) {
                segments.push_back({ ++stepN,
                                     step,
                                     sumL,
                                     ibl, isl, il,
                                     vName,
                                     std::string(mat->GetName()),
                                     mat->GetDensity(),
                                     mat->GetZ(),
                                     mat->GetA() });
            }

            //Debug-----------------------------------------------------------------
            std::cout << "  [Step " << stepN << "] Start |r| = " << Ri
                    << " km | dL = " << step
                    << " km | cumL = " << sumL << " km\n";
            //----------------------------------------------------------------------

            double tol = 1e-2;

            if (std::abs(sumL - total_dist) <= tol) break;

            

            if (geoManager->IsOutside()) break;

            // Push past a boundary surface into the next volume
            if (geoManager->IsOnBoundary()) nudge_into_next();
        }

        // --- DEBUG: confirm last recorded segment and path closure ---
        if (segments.empty()) {
            std::cout << "[DEBUG] Last segment: none recorded.\n";
        } else {
            const PathSegment& last = segments.back();
            std::cout << "[DEBUG] Last segment appended:"
                      << "  step="  << last.step
                      << "  vol="   << last.volName
                      << "  (ly="   << last.i_ly
                      << " sly="    << last.i_sly
                      << " blk="    << last.i_blk << ")"
                      << "  dL="    << last.distance   << " km"
                      << "  cumL="  << last.cumulative << " km"
                      << "  |  expected total_dist=" << total_dist << " km\n";
        }
        // -------------------------------------------------------------
    }

    // -------------------------------------------------------------------------

    void EarthPaths::PrintSegments() const {
        if (segments.empty()) {
            std::cout << "[EarthPaths] No segments — call GetSegments() first.\n";
            return;
        }

        const int w1 = 6, w2 = 14, w3 = 16, w4 = 7, w5 = 9, w6 = 7, w7 = 26, w8 = 14, w9 = 8, w10 = 10;

        std::cout << "\n"
                  << std::setw(w1)  << "Step"
                  << std::setw(w2)  << "Distance[km]"
                  << std::setw(w3)  << "Cumulative[km]"
                  << std::setw(w4)  << "Block"
                  << std::setw(w5)  << "Sublayer"
                  << std::setw(w6)  << "Layer"
                  << std::setw(w7)  << "Material"
                  << std::setw(w8)  << "Density[g/cm3]"
                  << std::setw(w9)  << "Z"
                  << std::setw(w10) << "A[g/mol]"
                  << "\n"
                  << std::string(w1+w2+w3+w4+w5+w6+w7+w8+w9+w10, '-') << "\n";

        for (const auto& s : segments) {
            std::cout << std::fixed << std::setprecision(4)
                      << std::setw(w1)  << s.step
                      << std::setw(w2)  << s.distance
                      << std::setw(w3)  << s.cumulative
                      << std::setw(w4)  << s.i_blk
                      << std::setw(w5)  << s.i_sly
                      << std::setw(w6)  << s.i_ly
                      << std::setw(w7)  << s.matName
                      << std::setw(w8)  << s.density
                      << std::setw(w9)  << s.Z
                      << std::setw(w10) << s.A
                      << "\n";
        }

        std::cout << std::string(w1+w2+w3+w4+w5+w6+w7+w8+w9+w10, '-') << "\n"
                  << "  Total path length : " << std::fixed << std::setprecision(4)
                  << (segments.empty() ? 0.0 : segments.back().cumulative) << " km\n"
                  << "  Segments recorded : " << segments.size() << "\n\n";
    }

    // -------------------------------------------------------------------------

    void EarthPaths::DisplayVectors() {
        if (n_dir.Mag() == 0.0) {
            std::cerr << "[EarthPaths] DisplayVectors: call GetSegments() first.\n";
            return;
        }

        TCanvas* c = new TCanvas("NuPath", "Neutrino Path through Earth", 800, 800);

        // --- Earth geometry ---------------------------------------------------
        geoManager->GetTopVolume()->Draw("gl");

        // Highlight innermost layer (the core, Layer_1 by naming convention)
        TGeoVolume* coreVol = geoManager->GetVolume("Layer_1");
        if (coreVol) {
            coreVol->SetLineColor(kOrange);
            coreVol->SetTransparency(20);
        }

        // --- Atmosphere sphere ------------------------------------------------
        // Draw a thin transparent shell at R_ATM to mark the atmosphere boundary
        TGeoSphere* atmShape = new TGeoSphere("AtmSphere", R_ATM - 50.0, R_ATM);
        TGeoVolume* atmVol   = new TGeoVolume("Atmosphere", atmShape, nullptr);
        atmVol->SetLineColor(kAzure + 7);
        atmVol->SetTransparency(80);
        atmVol->Draw("same");

        // --- Neutrino path line -----------------------------------------------
        delete nuPath;
        nuPath = new TPolyLine3D(2);
        nuPath->SetPoint(0, r_src.X(), r_src.Y(), r_src.Z());
        nuPath->SetPoint(1, r_det.X(), r_det.Y(), r_det.Z());
        nuPath->SetLineColor(kBlue);
        nuPath->SetLineWidth(2);
        nuPath->Draw("same");

        // --- Propagation direction arrow (500 km from source) -----------------
        delete dirArrow;
        dirArrow = new TPolyLine3D(2);
        dirArrow->SetPoint(0, r_src.X(), r_src.Y(), r_src.Z());
        dirArrow->SetPoint(1, r_src.X() + n_dir.X() * 500.0,
                              r_src.Y() + n_dir.Y() * 500.0,
                              r_src.Z() + n_dir.Z() * 500.0);
        dirArrow->SetLineColor(kRed);
        dirArrow->SetLineWidth(2);
        dirArrow->Draw("same");

        // --- Detector marker --------------------------------------------------
        delete markerDet;
        markerDet = new TPolyMarker3D(1, 2);
        markerDet->SetPoint(0, r_det.X(), r_det.Y(), r_det.Z());
        markerDet->SetMarkerColor(kMagenta);
        markerDet->SetMarkerSize(3);
        markerDet->Draw("same");

        // --- Source marker ----------------------------------------------------
        delete markerSrc;
        markerSrc = new TPolyMarker3D(1, 2);
        markerSrc->SetPoint(0, r_src.X(), r_src.Y(), r_src.Z());
        markerSrc->SetMarkerColor(kCyan);
        markerSrc->SetMarkerSize(3);
        markerSrc->Draw("same");

        // --- Diagnostics ------------------------------------------------------
        double total = 0.0;
        for (const auto& s : segments) total += s.distance;

        std::cout << "\n[EarthPaths::DisplayVectors]\n"
                  << "  Detector location  |r_det| = " << r_det.Mag()     << " km\n"
                  << "  Source location    |r_src| = " << r_src.Mag()     << " km\n"
                  << "  Direction          |n_dir| = " << n_dir.Mag()     << " (unit)\n"
                  << "  Total path length           = " << total           << " km\n"
                  << "  Segments recorded           = " << segments.size() << "\n\n";

        c->Update();
    }

} // namespace ModularEarth
