#include "Sublayer.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdio>   // needed for FILE, fopen, fprintf, fclose

#include "TSystem.h"
#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoVolume.h"
#include "TGeoShape.h"
#include "TGeoMatrix.h"
#include "TCanvas.h"
#include "TView.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TPolyMarker3D.h"

// What's a node? Is a placed instance of a volume (Shape + material) together with  local transformation matrix (position/rotation relative to its mother vol)
// A node is characterized by a copy number (node id). Nodes are created indirectly when calling MotherVolume->AddNode("DaughterVol", NodeID, TransformationMatrix)
// Mother volume is a containing volume of a volume



TString GetSubLayerNodePath(TGeoManager* mgr, int sl) {

    // GetTopNode() gives the top node — not GetNode(0) which searches the current branch
    TGeoNode* TopNode = mgr->GetTopNode();

    TString TopNode_path = TString("/") + TopNode->GetName();

    // TGeoVolume* not TGeoVolumeNode* — that type does not exist
    TGeoVolume* Top = mgr->GetTopVolume();

    int nDaughters_top = Top->GetNdaughters();

    printf("DAUGHTERS IN TOP (# OF SUBLAYERS): %i\n", nDaughters_top);


    TGeoNode* SubLayerNode = Top->GetNode(sl);
    
    TString SubLayer_node_name = SubLayerNode->GetName();
    
    TString SubLNode_path =  TopNode_path + TString("/") + SubLayer_node_name;

    printf("SUBLAYER NODE [%d]: %s\n", sl, SubLayer_node_name.Data());
    printf("SUBLAYER Path [%d]: %s\n", sl, SubLNode_path.Data());

    return SubLNode_path;
}

// I Guess this should be a method in the class sublayer itself, no need to have it external
void GetBlockVertices2(TGeoManager* mgr, int SubLayerID) {

    int sl = SubLayerID - 1;
    // --- 2. Loop over daughters, compute world-frame vertices ---
    TPolyMarker3D* markers = new TPolyMarker3D();
    markers->SetMarkerStyle(20);
    markers->SetMarkerSize(0.8);
    markers->SetMarkerColor(kRed);
    int vertexCount = 0;

    FILE* f = fopen("/home/ydenizhernandez/ModularEarth/tutorials/blocks.csv", "w");
    if (!f) {
    printf("ERROR: could not open file — %s\n", strerror(errno));
    return;
    }
    fprintf(f, "block_id,phi1,phi2,theta1,theta2,x,y,z\n");


    // GetTopNode() gives the top node — not GetNode(0) which searches the current branch
    TGeoNode* TopNode = mgr->GetTopNode();

    TString TopNode_name = TString("/") + TopNode->GetName();

    // TGeoVolume* not TGeoVolumeNode* — that type does not exist
    TGeoVolume* TopVol = mgr->GetTopVolume();

    int nDaughters_top = TopVol->GetNdaughters();

    printf("DAUGHTERS IN TOP (# OF SUBLAYERS): %i\n", nDaughters_top);


    // GetNode(i) on a volume gives its i-th daughter node
    TGeoNode* SubLayerNode = TopVol->GetNode(sl);
    TString SubLayerNode_name = SubLayerNode->GetName();
    TString SubLayerNode_path =  TopNode_name + TString("/") + SubLayerNode_name;

    // dynamic_cast to confirm it is actually an assembly
    TGeoVolumeAssembly* SubLayerVolume =
        dynamic_cast<TGeoVolumeAssembly*>(SubLayerNode->GetVolume());

    if (!SubLayerVolume) {
        printf("  WARNING: %s is not a TGeoVolumeAssembly\n",
            SubLayerNode_name.Data());
    }
    

    TString SubLayerVol_name = SubLayerVolume->GetName();
    
        
    printf("SUBLAYER NODE [%d]: %s\n", sl, SubLayerNode_name.Data());

    int nDaughters = SubLayerVolume->GetNdaughters();
    printf("  BLOCKS IN THIS SUBLAYER: %d\n", nDaughters);

    for (int i = 0; i < nDaughters; i++) {
        TGeoNode* blockNode = SubLayerVolume->GetNode(i);
        TString BlockVol_path = SubLayerNode_path + TString("/") + blockNode->GetName();
        
        if (!mgr->cd(BlockVol_path)) {
        printf("  WARNING: path %s is not valid\n", BlockVol_path.Data());  
        }

        printf("  BLOCK [%d]: %s\n", i, BlockVol_path.Data());
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGeoSphere* block  = dynamic_cast<TGeoSphere*>(blockNode->GetVolume()->GetShape());

        if (!block) continue;

        mgr->cd(BlockVol_path);

        // I don't understand how get to the correct node containing a block.
        TGeoMatrix* globalMatrix = mgr->GetCurrentMatrix();  // full chain

        double rmax     = block->GetRmax();
        double rmin     = block->GetRmin();
        double phis[2]   = { block->GetPhi1(),   block->GetPhi2()   };
        double thetas[2] = { block->GetTheta1(),  block->GetTheta2() };
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
                fprintf(f, "%d,%.6f,%.6f,%.6f,%.6f,%.4f,%.4f,%.4f\n",
                i,
                block->GetPhi1(), block->GetPhi2(),
                block->GetTheta1(), block->GetTheta2(),
                world[0], world[1], world[2]);

                markers->SetNextPoint(world[0], world[1], world[2]);
                vertexCount++;

            }
        }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////            
    }
    fclose(f);

    TCanvas* c1 = new TCanvas("c1", "Outer Surface Vertices", 1000, 800);
    gGeoManager->GetTopVolume()->Draw("gl");   // "ogl" = OpenGL viewer; use "" for legacy pad
    markers->Draw();
 
    if (gPad) {
        gPad->GetView()->ShowAxis();
    }
 
    c1->Update();
}

void GetBlockVertices(TGeoManager* mgr, TString TargetSubLayer = "SubLayer_1") {

    // --- 2. Loop over daughters, compute world-frame vertices ---
    TPolyMarker3D* markers = new TPolyMarker3D();
    markers->SetMarkerStyle(20);
    markers->SetMarkerSize(0.8);
    markers->SetMarkerColor(kRed);
    int vertexCount = 0;


    // GetTopNode() gives the top node — not GetNode(0) which searches the current branch
    TGeoNode* TopNode = mgr->GetTopNode();

    TString TopNode_name = TString("/") + TopNode->GetName();

    // TGeoVolume* not TGeoVolumeNode* — that type does not exist
    TGeoVolume* TopVol = mgr->GetTopVolume();

    int nDaughters_top = TopVol->GetNdaughters();

    printf("DAUGHTERS IN TOP (# OF SUBLAYERS): %i\n", nDaughters_top);

    // iterate over all sublayer(sl) nodes, not just index 0
    for (int sl = 0; sl < nDaughters_top; sl++) {

        // GetNode(i) on a volume gives its i-th daughter node
        TGeoNode* SubLayerNode = TopVol->GetNode(sl);
        TString SubLayerNode_name = SubLayerNode->GetName();
        TString SubLayerNode_path =  TopNode_name + TString("/") + SubLayerNode_name;

        // dynamic_cast to confirm it is actually an assembly
        TGeoVolumeAssembly* SubLayerVolume =
            dynamic_cast<TGeoVolumeAssembly*>(SubLayerNode->GetVolume());

        if (!SubLayerVolume) {
            printf("  WARNING: %s is not a TGeoVolumeAssembly\n",
                SubLayerNode_name.Data());
            continue;
        }
        

        TString SubLayerVol_name = SubLayerVolume->GetName();
        

        if (SubLayerVol_name == TargetSubLayer)
        {
            
            printf("  Sublayer Found: %s and %s is the same\n",
                SubLayerVol_name.Data(),TargetSubLayer.Data() );
            
                    printf("SUBLAYER NODE [%d]: %s\n", sl, SubLayerNode_name.Data());

            int nDaughters = SubLayerVolume->GetNdaughters();
            printf("  BLOCKS IN THIS SUBLAYER: %d\n", nDaughters);

            for (int i = 0; i < nDaughters; i++) {
                TGeoNode* blockNode = SubLayerVolume->GetNode(i);
                TString BlockVol_path = SubLayerNode_path + TString("/") + blockNode->GetName();
                
                if (!mgr->cd(BlockVol_path)) {
                printf("  WARNING: path %s is not valid\n", BlockVol_path.Data());  
                }

                printf("  BLOCK [%d]: %s\n", i, BlockVol_path.Data());
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                TGeoSphere* block  = dynamic_cast<TGeoSphere*>(blockNode->GetVolume()->GetShape());

                if (!block) continue;

                mgr->cd(BlockVol_path);

                // I don't understand how get to the correct node containing a block.
                TGeoMatrix* globalMatrix = mgr->GetCurrentMatrix();  // full chain

                double rmax     = block->GetRmax();
                double rmin     = block->GetRmin();
                double phis[2]   = { block->GetPhi1(),   block->GetPhi2()   };
                double thetas[2] = { block->GetTheta1(),  block->GetTheta2() };
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
                        markers->SetNextPoint(world[0], world[1], world[2]);
                        vertexCount++;

                    }
                }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////            
            }

        }

        else {
            printf("  ERROR: [%s] does not match target sublayer [%s]\n",
                SubLayerVol_name.Data(), TargetSubLayer.Data());
            continue;
        }

    }
    
    TCanvas* c1 = new TCanvas("c1", "Outer Surface Vertices", 1000, 800);
    gGeoManager->GetTopVolume()->Draw("gl");   // "ogl" = OpenGL viewer; use "" for legacy pad
    markers->Draw();
 
    if (gPad) {
        gPad->GetView()->ShowAxis();
    }
 
    c1->Update();
}


//Test for Tracking in SubLayers
void BlockMapTutorial() {

    const int loadStatus = gSystem->Load("./libModularEarth.so");
    if (loadStatus < 0) {
        std::cerr << "ERROR: could not load ./libModularEarth.so\n";
        return;
    }

    //---------Create TOP volume
    TGeoManager *geoManager = new TGeoManager("TopVol", "Earth in Sphere demo");
    // Define vacuum for outer container
    TGeoMaterial *matVac = new TGeoMaterial("Vacuum", 0, 0, 0);
    TGeoMedium *medVac = new TGeoMedium("Vacuum", 1, matVac);
    // Create top-level box (10x10x10 km)
    //TGeoVolume *top = new TGeoVolume("TopBox", new TGeoBBox("Box", 5000, 5000, 5000), medVac);
    TGeoVolume* top = new TGeoVolume("EarthTopSphere", new TGeoSphere("Sphere", 0, 7000), medVac);



    geoManager->SetTopVolume(top);





    //Test SubLayer
    ModularEarth::Sublayer TestSubLayer1;
    TestSubLayer1.SetSubLayerID(1); //Set SubLayer Idex
    TestSubLayer1.SetSubLayerMaterial("TestMaterial1", 1,1,0.5); //Sublayer Material must be defined before creating sublayer volume
    TestSubLayer1.CreateSubLayerBlocks(3480,3880,60,30); //Create a sublayer given their inner and outer radius and lon and lat bin size
    TestSubLayer1.BuildSubLayer(); // Build the layer

    ModularEarth::Sublayer TestSubLayer2;
    TestSubLayer2.SetSubLayerID(2); //Set SubLayer Idex
    TestSubLayer2.SetSubLayerMaterial("TestMaterial2", 1,1,0.5); //Sublayer Material must be defined before creating sublayer volume
    TestSubLayer2.CreateSubLayerBlocks(3480,3880,60,30); //Create a sublayer given their inner and outer radius and lon and lat bin size
    TestSubLayer2.BuildSubLayer(); // Build the layer

    TGeoVolumeAssembly* TestSubLayer1Volume = TestSubLayer1.GetSubLayerVolume();
    //GetBlockVertices(geoManager, TestSubLayer1Volume);

    // TestSubLayer1.SetBinMap();
    // TestSubLayer1.AddMarker();

    //TestSubLayer1.GetBlockVertices(mgr, SubLayerNode_path)

    top->AddNode(TestSubLayer1.GetSubLayerVolume() , 1, new TGeoTranslation(0, 0, 0));// GetSubLayerVolume return the actual TGeoManager Volume, this need to be add to the top volume or to anothr TGeoAssembly.
    top->AddNode(TestSubLayer2.GetSubLayerVolume() , 1, new TGeoTranslation(0, 0, 0));


    TString Path = GetSubLayerNodePath(geoManager,  0);
    

    TestSubLayer1.GetBlockVertices(geoManager, Path);

    printf("SUBLAYER Path [%d]: %s\n", 0, Path.Data());

    //GetBlockVertices2(geoManager,1);


    geoManager->CloseGeometry(); //Here we finished the model creation. "Closed" means that no more modification can be made to the model.
    //geoManager->SetTopVisible();     // Ensure top volume shows up
      // ---- Draw to a canvas
    //TCanvas *c = new TCanvas("c", "SubLayerTutorial", 1200, 900);

    // SAFE in batch/headless: "ogl" or default (no OpenGL)
    // Try "ogl" first; if you used "gl" and it fails in batch, switch to plain "ogl" or "".
    //top->Draw("gl");      // or: top->Draw("");

    //c->Update();

    // Save into tutorials/

    //printf("Test for binmap tutorials\n");

    //c->SaveAs("SubLayerBinMapTutorial.png");
}