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

// In ModularEarth each SubLayer object is an assembly (TGeoAssembly ) of block volumes.
// Each sublayer has a index the identifies it, then a user can access each sublayer by specificyn thta index
// Once a specific sublayer is selected, the user can acces to any of the constinuest blocks and modify its density annd composion
// If a block is modified it will be flag by ModularEarth, so that later the user can keep track of wath blocks were modified.
// The user can generated a projections of a layers and its blocks using BlockMapTutorial, so that each the user can inspoect each block and
// idenftify the index of each block for a particular layer.



void BlockMapTutorial() {

    //Load ModularEarth libray
    const int loadStatus = gSystem->Load("./libModularEarth.so");
    if (loadStatus < 0) {
        std::cerr << "ERROR: could not load ./libModularEarth.so\n";
        return;
    }

   
    TGeoManager *geoManager = new TGeoManager("TopVol", "Earth in Sphere demo");
    // Create the material of the TOP medium, For this volume there is no other reson fo it no to be  vacuum 
    TGeoMaterial *matVac = new TGeoMaterial("Vacuum", 0, 0, 0);
    // Create the medium of the TOP medium
    TGeoMedium *medVac = new TGeoMedium("Vacuum", 1, matVac);
    //Create the TOP volume (The volume that contains the main geometry)
    TGeoVolume* top = new TGeoVolume("EarthTopSphere", new TGeoSphere("Sphere", 0, 7000), medVac);

    geoManager->SetTopVolume(top);

    //Create a test SubLayer with index 1
    ModularEarth::Sublayer TestSubLayer1;
    TestSubLayer1.SetSubLayerID(1); //Set SubLayer Idex
    TestSubLayer1.SetSubLayerMaterial("TestMaterial1", 1,1,0.5); //Sublayer Material must be defined before creating sublayer volume
    TestSubLayer1.CreateSubLayerBlocks(3480,3880,60,30); //Create a sublayer given their inner and outer radius and lon and lat bin size

    //Modify a set of blocks' material to test the modification-tracking flag.
    //This must happen before BuildSubLayer() so the assembly is built with the updated block volumes.
    TestSubLayer1.UpdateMultipleBlocksMaterial({1, 3, 5, 7}, "ModifiedTestMaterial", 2, 2, 1.0);

    TestSubLayer1.BuildSubLayer(); // Build the layer

    ModularEarth::Sublayer TestSubLayer2;
    TestSubLayer2.SetSubLayerID(2); //Set SubLayer Idex
    TestSubLayer2.SetSubLayerMaterial("TestMaterial2", 1,1,0.5); //Sublayer Material must be defined before creating sublayer volume
    TestSubLayer2.CreateSubLayerBlocks(3480,3880,60,30); //Create a sublayer given their inner and outer radius and lon and lat bin size
    TestSubLayer2.BuildSubLayer(); // Build the layer

    //Add Subalyers into the Mother volume
    top->AddNode(TestSubLayer1.GetSubLayerVolume() , 1, new TGeoTranslation(0, 0, 0));// GetSubLayerVolume return the actual TGeoManager Volume, this need to be add to the top volume or to anothr TGeoAssembly.
    top->AddNode(TestSubLayer2.GetSubLayerVolume() , 1, new TGeoTranslation(0, 0, 0));


    TString Path = GetSubLayerNodePath(geoManager,  0);
    
    //Generate a file containing all the block outer vertices, this will be use to visulize the sublayer later 
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