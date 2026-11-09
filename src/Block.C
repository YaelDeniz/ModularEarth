///NOTEs :  Consider adding Move constructor + Move ASSIGMENT  + ~ (Delete opertatur)
#include "Block.h"

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
#include <TGeoSphere.h>
#include <TGeoMatrix.h>
#include <TH1F.h>
#include <TPolyLine3D.h>
#include <TPolyMarker3D.h>
#include <TPolyLine3D.h>
#include <TView.h>
#include <TCanvas.h>
#include <TGraph.h>



namespace ModularEarth {

    void Block::BlockWasModified(bool status) {
        BlockModified = status;
    }

    void Block::SubLayerWasModified(bool status) {
        SubLayerModified = status;
    }

    int Block::GetBlockIndex() const { return i_blk; }
    int Block::GetSublayerIndex() const { return i_sly; }
    int Block::GetLayerIndex() const { return i_ly; }
    int Block::GetBlockID() const { return i_blk; } // deprecated alias

    void Block::SetBlockGeometry(double r, double R,
                                double theta1, double theta2,
                                double phi1, double phi2,
                                int i_blk_, int i_ly_, int i_sly_) {
        rmin     = r;
        rmax     = R;
        thetamin = theta1;
        thetamax = theta2;
        phimin   = phi1;
        phimax   = phi2;
        i_blk    = i_blk_;
        i_ly     = i_ly_;
        i_sly    = i_sly_;
    }

    void Block::DefineBlockMaterial(const std::string& name, double A, double Z, double rho) {
        materialName = name;
        atomicMass   = A;
        atomicNumber = Z;
        density      = rho;
    }

    void Block::BuildBlock(TGeoMedium* medium) {
        if (!medium) {
            std::cerr << "ERROR: Medium not defined before building a block!\n";
            return;
        }

        if (!gGeoManager) {
            std::cerr << "ERROR: gGeoManager is nullptr. Initialize ROOT geometry before building blocks.\n";
            return;
        }

        // Do NOT delete the previous TGeoVolume here. ROOT's TGeoManager owns every volume it
        // registers via its internal TLists. Calling delete on a ROOT-managed object leaves
        // dangling pointers inside the manager, causing crashes the next time the geometry is
        // navigated or the volume is accessed. Leaked volumes are cleaned up when the
        // TGeoManager itself is destroyed.
        block = nullptr;

        TGeoShape* shape = new TGeoSphere(Form("BlockShape_%d_%d_%d", i_ly, i_sly, i_blk),
                                        rmin, rmax, thetamin, thetamax, phimin, phimax);

        block = new TGeoVolume(Form("BlockVol_%d_%d_%d", i_ly, i_sly, i_blk), shape, medium);
        block->SetVisibility(kTRUE);

        if (BlockModified) {
            block->SetLineColor(kRed);
            block->SetFillColor(kRed);
        }

        if (SubLayerModified) {
            block->SetLineColor(kGreen);
        }
    }

    bool Block::IsValid() const {
        return (block != nullptr &&
                rmax > rmin &&
                thetamax > thetamin &&
                phimax > phimin &&
                density > 0 &&
                atomicMass > 0 &&
                atomicNumber > 0);
    }

    void Block::InvalidBlockSummary(std::ostream& os) const
    {

        if (block == nullptr)
            os << "  block (TGeoVolume*) is null\n";

        if (!(rmax > rmin))
            os << "  invalid radii: rmin=" << rmin << ", rmax=" << rmax << "\n";

        if (!(thetamin < thetamax))
            os << "  invalid theta range: thetamin=" << thetamin
            << ", thetamax=" << thetamax << "\n";

        if (!(phimin < phimax))
            os << "  invalid phi range: phimin=" << phimin
            << ", phimax=" << phimax << "\n";

        if (!(density > 0))
            os << "  invalid density: " << density << "\n";

        if (!(atomicMass > 0))
            os << "  invalid atomic mass: " << atomicMass << "\n";

        if (!(atomicNumber > 0))
            os << "  invalid atomic number: " << atomicNumber << "\n";
    }


    void Block::BlockSummary() const {
        std::cout << "=== Block Summary ===\n";
        std::cout << "  Block Index:     (" << i_ly << ", " << i_sly << ", " << i_blk << ")  [i_ly, i_sly, i_blk]\n";
        std::cout << "  Block Name:      " << (block ? block->GetName() : "N/A") << "\n";

        if (block && block->GetShape() && block->GetShape()->IsA() == TGeoSphere::Class()) {
            auto* sphere = dynamic_cast<TGeoSphere*>(block->GetShape());
            std::cout << "  Shape:           TGeoSphere\n";
            std::cout << "    rmin:          " << sphere->GetRmin() << "\n";
            std::cout << "    rmax:          " << sphere->GetRmax() << "\n";
            std::cout << "    thetamin:      " << sphere->GetTheta1() << "\n";
            std::cout << "    thetamax:      " << sphere->GetTheta2() << "\n";
            std::cout << "    phimin:        " << sphere->GetPhi1() << "\n";
            std::cout << "    phimax:        " << sphere->GetPhi2() << "\n";
        } else {
            std::cout << "  Shape:           Not a TGeoSphere or invalid\n";
        }

        std::cout << std::boolalpha
                << "  Specific Block Modified: " << BlockModified << "\n"
                << "  Sub Layer Modified:      " << SubLayerModified << "\n";
        std::cout << "  Material:        " << materialName << "\n";
        std::cout << "  Atomic Num  Z:   " << atomicNumber << "\n";
        std::cout << "  Atomic Mass A:   " << atomicMass << "\n";
        std::cout << "  Density:         " << density << " g/cm^3\n";
        std::cout << "  Pointers:\n";
        std::cout << "    Volume:        " << block << "\n";
        std::cout << "=====================\n";
    }

    bool Block::IsBlockModified() const { return BlockModified; };
    bool Block::IsSubLayerModified() const { return SubLayerModified; };

    TGeoVolume* Block::GetVolume() const {
        return block;
    }
}