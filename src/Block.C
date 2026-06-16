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

    int Block::GetBlockID() const {
        return blockID;
    }

    void Block::SetBlockGeometry(double r1, double r2,
                                double theta1, double theta2,
                                double phi1, double phi2,
                                int id) {
        rmin = r1;
        rmax = r2;
        thetamin = theta1;
        thetamax = theta2;
        phimin = phi1;
        phimax = phi2;
        blockID = id;
    }

    void Block::DefineBlockMaterial(const std::string& name, double A, double Z, double rho) {
        materialName = name;
        atomicMass = A;
        atomicNumber = Z;
        density = rho;

        // Clean any previous material/medium if you re-define
        delete blockMedium;   blockMedium = nullptr;
        delete blockMaterial; blockMaterial = nullptr;

        blockMaterial = new TGeoMaterial(name.c_str(), A, Z, rho);
        blockMedium   = new TGeoMedium(name.c_str(), blockID, blockMaterial);
    }

    void Block::BuildBlock(int /*colorid*/) {
        if (!blockMedium) {
            std::cerr << "ERROR: Medium not defined before building a block!\n";
            return;
        }

        if (!gGeoManager) {
            std::cerr << "ERROR: gGeoManager is nullptr. Initialize ROOT geometry before building blocks.\n";
            return;
        }

        // If rebuilding, delete previous volume first
        delete block;
        block = nullptr;

        TGeoShape* shape = new TGeoSphere(Form("BlockShape_%d", blockID),
                                        rmin, rmax, thetamin, thetamax, phimin, phimax);

        block = new TGeoVolume(Form("BlockVolume_%d", blockID), shape, blockMedium);
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
                blockMaterial != nullptr &&
                blockMedium != nullptr &&
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

        if (blockMaterial == nullptr)
            os << "  blockMaterial (TGeoMaterial*) is null\n";

        if (blockMedium == nullptr)
            os << "  blockMedium (TGeoMedium*) is null\n";

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
        std::cout << "  Block ID:        " << blockID << "\n";
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

        if (blockMedium && blockMedium->GetMaterial()) {
            TGeoMaterial* mat = blockMedium->GetMaterial();
            std::cout << std::boolalpha
                    << "  Specific Block Modified: " << BlockModified << "\n"
                    << "  Sub Layer Modified:      " << SubLayerModified << "\n";
            std::cout << "  Material:        " << mat->GetName() << "\n";
            std::cout << "  Atomic Num  Z:   " << mat->GetZ() << "\n";
            std::cout << "  Atomic Mass A:   " << mat->GetA() << "\n";
            std::cout << "  Density:         " << mat->GetDensity() << " g/cm^3\n";
        } else {
            std::cout << "  Material:        Undefined\n";
        }

        std::cout << "  Pointers:\n";
        std::cout << "    Volume:        " << block << "\n";
        std::cout << "    Material:      " << blockMaterial << "\n";
        std::cout << "    Medium:        " << blockMedium << "\n";
        std::cout << "=====================\n";
    }

    TGeoVolume* Block::GetVolume() const {
        return block;
    }
}