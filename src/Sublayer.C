#include "Sublayer.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>

#include <TGeoManager.h>
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
#include <TGeoSphere.h>
#include <TMath.h>



#include <TSystem.h>
#include <TROOT.h>


// Modular Earth classes
#include "Block.h"


namespace ModularEarth {
    // NOTES overlapping shapes ? Are blocks  tiling cleanly
    void Sublayer::BuildAssembly() {
        if (SubLayer) delete SubLayer;
        SubLayer = new TGeoVolumeAssembly(SubLayerName.c_str());

        static TGeoTranslation * sharedTranslation = new TGeoTranslation(0, 0, 0);
        // Alternative: TGeoTranslation * sharedTranslation = new TGeoTranslation(0, 0, 0);
        for (const auto & b: blocks) {
            if (b.GetVolume() == nullptr) {
                std::cerr << "Block is empty volume!\n";
            } else if (!b.IsValid()) {
                std::cerr << "Error: Invalid block detected. Blocks were not built properly\n";

                b.InvalidBlockSummary(std::cerr);

                b.BlockSummary();
            }
            SubLayer -> AddNode(b.GetVolume(), b.GetBlockID(), sharedTranslation);
        }
        }

    // Set SubLayer ID. SubLayers are name based on their ID (e.g., "SubLayer_#ID")
    void Sublayer::SetSubLayerID(int id) {
        SubLayerID = id;
        SubLayerName = Form("SubLayer_%d", SubLayerID);
    }

    // ------------------------------------------------------------------
    // Geometry and Material Definition
    // ------------------------------------------------------------------

    /// Define the geometric boundaries of the block and assign its ID.
    ///
    /// Parameters:
    ///   r1, r2       : Inner and outer radii of the sublayer
    ///   dth: Longitude range [degrees]
    ///   dph: Latitud range [degrees]
    void Sublayer::CreateSubLayerBlocks(double r1, double r2, double dphi, double dth) {
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


    /// Define the material properties of the Sublayer.
    ///
    /// Parameters:
    ///   name : Material name
    ///   A    : Atomic mass
    ///   Z    : Atomic number
    ///   rho  : Density (g/cm^3)
    ///
    /// This method must be called before BuildBlock().               
        void Sublayer::SetSubLayerMaterial(const std::string & name, double A, double Z, double density) {
            SubLayerMaterialName = name;
            SubLayerA = A;
            SubLayerZ = Z;
            SubLayerDensity = density;
        }

    // ------------------------------------------------------------------
    // Udate the materil of a block or several blocks whitin the sublayer
    // ------------------------------------------------------------------

    /// Allow the userto modify the material of specific blocks, set
    /// of blocks or an entire Sublayer.
    
    //Update individual material properties
    void Sublayer::UpdateBlockMaterial(int id,
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

    //Update the material properties of a set of blocks
    void Sublayer::UpdateMultipleBlocksMaterial(const std::vector < int > & ids,
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

    //Update the material properties of the entire sublayer
    void Sublayer::UpdateAllBlocksMaterial(const std::string & NewName, double NewA, double NewZ, double NewDensity) {
        for (Block & modifiedBlock: blocks) {
            modifiedBlock.DefineBlockMaterial(NewName, NewA, NewZ, NewDensity);
            modifiedBlock.SubLayerWasModified(true);
            modifiedBlock.BuildBlock();
        }
        FullUpdate = true;
    }
    // ------------------------------------------------------------------
    // Geometry Construction
    // ------------------------------------------------------------------

    /// Build the ROOT geometry volume associated with this block.
    ///
    /// This creates the TGeoShape, TGeoVolume, and assigns visualization
    /// properties based on modification flags.
    /// A valid TGeoManager must exist prior to calling this method.

    void Sublayer::BuildSubLayer() {
        BuildAssembly();
    }

    // ------------------------------------------------------------------
    // Validation and Accessors
    // ------------------------------------------------------------------

     TGeoVolumeAssembly * Sublayer::GetSubLayerVolume() const {
        return SubLayer;
    }

    // ------------------------------------------------------------------
    // Diagnostics and Debugging
    // ------------------------------------------------------------------

    /// Print a detailed summary of the block geometry, material,
    /// modification flags, and internal ROOT pointers.

    void Sublayer::GetBlockSummary(int id) {
        if (id >= 1 && id <= static_cast < int > (blocks.size()))
            blocks[id - 1].BlockSummary();
        else
            std::cerr << "Invalid block ID: " << id << std::endl;
    }

    void Sublayer::GetAllBlockSummary() {
        for (const auto & b: blocks) {
            if (!b.IsValid()) {
                std::cerr << "Error: Invalid block\n";
            }
            b.BlockSummary();
        }
    }

    void Sublayer::GetSubLayerSummary() {
    std::cout << "SubLayer Index: " << SubLayerID << "\n"
            << "SubLayer Name: " << SubLayerName << "\n"
            << "Number of Blocks: " << blocks.size() << "\n"
            << "Sublayer Updates: Full(" << std::boolalpha << FullUpdate
            << ") Blocks(" << BlocksUpdate << ")" << std::endl;
        }


    void Sublayer::GetBlockVertices(TGeoManager* mgr, TString SubLayerNode_path, TString filename ) const {

        // I need to pass a node so that i can acces the volume.

        // dynamic_cast to confirm it is actually an assembly
        TGeoVolumeAssembly* SubLayerVolume = dynamic_cast<TGeoVolumeAssembly*>(SubLayer);

        TString SubLayerVol_name = SubLayerVolume->GetName();

        if (filename.IsNull()) {
            
            TString ModularEarthDir = gSystem->pwd();
            filename = ModularEarthDir+ "/tutorials/" + SubLayerVol_name + "_default.csv";
            printf("No filename provided. Using default name: %s\n", filename.Data());

        }
        
        FILE* f = fopen(filename.Data(), "w");
        
        if (!f) {
            printf("ERROR: could not open file — %s\n", strerror(errno));
            return;
        }

        fprintf(f, "block_id,mod_flag,phi1,phi2,theta1,theta2,x,y,z\n");

        if (!SubLayerVolume) {
            printf("  WARNING: %s is not a TGeoVolumeAssembly\n",
            SubLayerVol_name.Data());
            fclose(f);
            return;
        }
        
        

        printf("SUBLAYER NODE: %s\n", SubLayerNode_path.Data());

        int nDaughters = SubLayerVolume->GetNdaughters();
        printf("  BLOCKS IN THIS SUBLAYER: %d\n", nDaughters);

        for (int i = 0; i < nDaughters; i++) {
            int modFlag = blocks[i].IsBlockModified()    ? 1
                        : blocks[i].IsSubLayerModified() ? 2
                        : 0;
            printf("  BLOCK [%d]: modFlag=%d\n", i, modFlag);
            TGeoNode* bnode = SubLayerVolume->GetNode(i);
            TString BlockVol_path = SubLayerNode_path + TString("/") + bnode->GetName();
            
            if (!mgr->cd(BlockVol_path)) {
            printf("  WARNING: path %s is not valid\n", BlockVol_path.Data());  
            continue;
            }

            printf("  BLOCK [%d]: %s\n", i, BlockVol_path.Data());

            TGeoSphere* blk  = dynamic_cast<TGeoSphere*>(bnode->GetVolume()->GetShape());

            if (!blk) continue;

            mgr->cd(BlockVol_path);

            TGeoMatrix* globalMatrix = mgr->GetCurrentMatrix();  // full chain

            double rmax     = blk->GetRmax();
            double rmin     = blk->GetRmin();
            double phis[2]   = { blk->GetPhi1(),   blk->GetPhi2()   };
            double thetas[2] = { blk->GetTheta1(),  blk->GetTheta2() };
            printf("  Angular coordinates limits: (%.4f, %.4f,%.4f, %.4f)\n", phis[0] ,phis[1] ,thetas[0],thetas[1] );
            printf("  Radial coordinates limits: (%.4f, %.4f)\n", rmin, rmax );
            
            for (double phi_deg : phis) {
                for (double theta_deg : thetas) {
                    double phi   = phi_deg   * TMath::DegToRad();
                    double theta = theta_deg * TMath::DegToRad();

                    double local[3] = {
                        rmax * TMath::Sin(theta) * TMath::Cos(phi),
                        rmax * TMath::Sin(theta) * TMath::Sin(phi),
                        rmax * TMath::Cos(theta)
                    };
                    double world[3];
                    globalMatrix->LocalToMaster(local, world);

                    printf("  World: (%.4f, %.4f, %.4f)\n", world[0], world[1], world[2]);
                    fprintf(f, "%d,%d,%.6f,%.6f,%.6f,%.6f,%.4f,%.4f,%.4f\n",
                        i, modFlag,
                        blk->GetPhi1(), blk->GetPhi2(),
                        blk->GetTheta1(), blk->GetTheta2(),
                        world[0], world[1], world[2]);

                }
            }
                
        }
        fclose(f);
    }

};
