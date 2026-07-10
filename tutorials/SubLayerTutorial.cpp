#include "Sublayer.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>

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



//Test for Tracking in SubLayers
void SubLayerTutorial() {

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

    top->AddNode(TestSubLayer1.GetSubLayerVolume() , 1, new TGeoTranslation(0, 0, 0));// GetSubLayerVolume return the actual TGeoManager Volume, this need to be add to the top volume or to anothr TGeoAssembly.

    ModularEarth::Sublayer TestSubLayer2;
    TestSubLayer2.SetSubLayerID(2); //Set SubLayer Idex
    TestSubLayer2.SetSubLayerMaterial("TestMaterial2", 2,3,23.4); //Sublayer Material must be defined before creating sublayer volume
    TestSubLayer2.CreateSubLayerBlocks(3880,4680,60,30); //Create a sublayer given their inner and outer radius and lon and lat bin size
    TestSubLayer2.BuildSubLayer(); // Build the layer

    top->AddNode(TestSubLayer2.GetSubLayerVolume(), 2, new TGeoTranslation(0, 0, 0));// GetSubLayerVolume return the actual TGeoManager Volume, this need to be add to the top volume or to anothr TGeoAssembly.

    //TGeoVolume* testvol = new TGeoVolume("TestVolumeEarth", new TGeoSphere("SubLayerTestVolume", 3480, 3880), medVac);
    
    //top->AddNode(testvol,1, new TGeoTranslation(0, 0, 0));
    
    geoManager->CloseGeometry(); //Here we finished the model creation. "Closed" means that no more modification can be made to the model.

    //geoManager->SetTopVisible();     // Ensure top volume shows up

      // ---- Draw to a canvas
    TCanvas *c = new TCanvas("c", "SubLayerTutorial", 1200, 900);

    // SAFE in batch/headless: "ogl" or default (no OpenGL)
    // Try "ogl" first; if you used "gl" and it fails in batch, switch to plain "ogl" or "".
    top->Draw("gl");      // or: top->Draw("");

    c->Update();

    // Save into tutorials/

    c->SaveAs("SubLayerTutorial.png");
}