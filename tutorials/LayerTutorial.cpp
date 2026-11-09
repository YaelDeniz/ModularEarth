#include "Layer.h"

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


void LayerTutorial() {
    //Comparion with OscProbe

    int dtheta = 10;
    int dphi = 10;

    TMultiGraph * ThePaths = new TMultiGraph();

    TGraph * ModularEarthProfile = new TGraph();
    std::vector<double> ModularPathL;
    std::vector<double> ModularPathR;
    std::vector<double> ModularPathDensity;


    TGraph * OscProbProfile = new TGraph();

    //---------Create TOP volume
    TGeoManager *geoManager = new TGeoManager("TopVol", "Earth in Sphere demo");
    // Define vacuum for outer container
    TGeoMaterial *matVac = new TGeoMaterial("Vacuum", 0, 0, 0);
    TGeoMedium *medVac = new TGeoMedium("Vacuum", 1, matVac);
    // Create top-level box (10x10x10 km)
    //TGeoVolume *top = new TGeoVolume("TopBox", new TGeoBBox("Box", 5000, 5000, 5000), medVac);
    TGeoVolume* top = new TGeoVolume("EarthTopSphere", new TGeoSphere("Sphere", 0, 7000), medVac);
    geoManager->SetTopVolume(top);

    ////////////////////////////// Layer Test

    // SubLayer How to:
    // 1) To create a Layer do:
    //
    //    Layer LayerObject("Layer Name",LayerIndex,dlon,dlat)- Create a sublayer object with class constructor, dlon and dlat are lon lat bin size
    //
    // 2) Create a list containing information of the sublayers with sublayersconfigOC
    //
    //      std::vector<ModularEarth::SublayerConfig> sublayersconfig = {
    //          {SubLayerIndex1, rmin, rmax, rho, z, A, "SubLayerMateialName"},
    //          {...},
    //          {...},
    //          ...
    //          {SubLayerIndexN, rmin, rmax, rho, z, A, "SubLayerMateialName"},
    //      };
    //
    // 3) from the configuration list, Create the sublayer and stack them in the assembly making a layer with
    //
    //   LayerObject.SetSubLayers(sublayersconfig);   
    //
    // 4) Build the Layer Volume Assembly and ad it to top
    //
    //    LayerObject.BuildLayer();
    //    top->AddNode(LayerObject.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    //




    //Layer InnerCore("InnerCore",0,dphi,dtheta); // Layer constructor indicating Layer name, index, lon bin size and lat bin size
    std::vector<ModularEarth::SublayerConfig> sublayersconfigIC = {{1,0, 1221.5, 12.9807, 0.4691, 1, "Iron"}}; //Struct containing the list of sublayers of a layer
    //syntax {sublayer index, rmin,rmax,Z,A,rho, material name}
    //InnerCore.SetSubLayers(sublayersconfigIC);
    //InnerCore.BuildLayer();
    //Make Sphere

    TGeoMaterial *matIC = new TGeoMaterial("SolidNiFe",  0.4691, 1, 12.9807);
    TGeoMedium *medIC = new TGeoMedium("SolidNiFe", 1, matIC);
    TGeoVolume* InnerCore = new TGeoVolume("InnerCore_1", new TGeoSphere("Sphere", 0, 1221.5), medIC);

    //
    ModularEarth::Layer OuterCore("OuterCore",2,dphi,dtheta);
    std::vector<ModularEarth::SublayerConfig> sublayersconfigOC = {
        {1,1221.5, 1946.7, 11.9512, 0.4691, 1, "FeNi"},
        {2,1946.7, 2475.1, 11.4726, 0.4691, 1, "FeNi"},
        {3,2475.1, 2900.3, 10.9884, 0.4691, 1, "FeNi"},
        {4,2900.3, 3260.6, 10.5008	, 0.4691, 1, "FeNi"},
        {5,3260.6, 3480.0, 10.0861, 0.4691, 1, "FeNi"}
    };
    OuterCore.SetSubLayers(sublayersconfigOC);
    OuterCore.BuildLayer();

    ModularEarth::Layer LowerMantle("LowerMantle",3,dphi,dtheta);
    std::vector<ModularEarth::SublayerConfig> sublayersconfigLM = {
        {1,3480.0, 4476.0, 5.3225, 0.4954, 1, "SilicateLM"},
        {2,4476.0, 5378.4, 4.8332, 0.4691, 1, "SilicateLM"},
        {3,5378.4, 5701.0, 4.4872, 0.4691, 1, "SilicateLM"}
    };
    LowerMantle.SetSubLayers(sublayersconfigLM);
    LowerMantle.BuildLayer();

    ModularEarth::Layer UpperMantle("UpperMantle",4,dphi,dtheta);
    std::vector<ModularEarth::SublayerConfig> sublayersconfigUM = {
        {1,5701.0, 5971.0, 3.8983	, 0.4954, 1, "SilicateUM"},
        {2,5971.0, 6346.6, 3.4306	, 0.4691, 1, "SilicateUM"},
        {3,6346.6, 6356.0, 2.9000	, 0.4956, 1, "SilicateC"}
    };
    UpperMantle.SetSubLayers(sublayersconfigUM);
    UpperMantle.BuildLayer();

    ModularEarth::Layer Crust("Crust",5,dphi,dtheta);
    std::vector<ModularEarth::SublayerConfig> sublayersconfigC = {
        //{1,6346.6, 6356.0, 2.9000	, 0.4956, 1, "SilicateC"},
        {1,6356.0, 6368.0, 2.6000	, 0.4956, 1, "SilicateC"}
    };
    Crust.SetSubLayers(sublayersconfigC);
    Crust.BuildLayer();

    ModularEarth::Layer Ocean("Ocean",6,dphi,dtheta);
    std::vector<ModularEarth::SublayerConfig> sublayersconfigO = {
        {1,6368.0, 6371.0, 1.0200	, 0.5525, 1, "SilicateO"}
    };
    Ocean.SetSubLayers(sublayersconfigO);
    Ocean.BuildLayer();

    ModularEarth::Layer Atmosphere("Atmosphere",7,dphi,dtheta);

      std::vector<ModularEarth::SublayerConfig> sublayersconfigAtm = {
        {1,6371.0, 6386.0	, 0.001	, 0.4991, 1, "Vac"}
    };
    Atmosphere.SetSubLayers(sublayersconfigAtm);
    Atmosphere.BuildLayer();


    top->AddNode(InnerCore,                     1,                            new TGeoTranslation(0, 0, 0));
    top->AddNode(OuterCore.GetLayerVolume(),    OuterCore.GetLayerIndex(),    new TGeoTranslation(0, 0, 0));
    top->AddNode(LowerMantle.GetLayerVolume(),  LowerMantle.GetLayerIndex(),  new TGeoTranslation(0, 0, 0));
    top->AddNode(UpperMantle.GetLayerVolume(),  UpperMantle.GetLayerIndex(),  new TGeoTranslation(0, 0, 0));
    top->AddNode(Crust.GetLayerVolume(),        Crust.GetLayerIndex(),        new TGeoTranslation(0, 0, 0));
    top->AddNode(Ocean.GetLayerVolume(),        Ocean.GetLayerIndex(),        new TGeoTranslation(0, 0, 0));
    top->AddNode(Atmosphere.GetLayerVolume(),   Atmosphere.GetLayerIndex(),   new TGeoTranslation(0, 0, 0));


    // printf("MODULAR_EARTH_DIR = %s\n", DMODULAR_EARTH_DIR);

    //UpperMantle.Call_The_Roll(); // Useful for debbuging and model checks... It list all the layers and sublayers in the model and their names+IDs.

    /////////////////////////////
    
    geoManager->CloseGeometry(); //Here we finished the model creation. "Closed" means that no more modification can be made to the model.

    if (!top) return;

    std::cout << "\n========== Main Assemblies ==========\n";

    for (int i = 0; i < top->GetNdaughters(); ++i)
    {
        TGeoNode* mainNode = top->GetNode(i);
        TGeoVolume* mainVol = mainNode->GetVolume();

        std::cout << "\nMain daughter " << i
                  << ": " << mainVol->GetName()
                  << " [" << mainVol->ClassName() << "]\n";

        int totalShellAssemblies = 0;
        int totalSphereNodes = 0;

        if (mainVol->GetNdaughters() > 0){

        for (int j = 0; j < mainVol->GetNdaughters(); ++j)
        {
            TGeoNode* shellNode = mainVol->GetNode(j);
            TGeoVolume* shellVol = shellNode->GetVolume();

            totalShellAssemblies++;

            int nSpheres = 0;

            for (int k = 0; k < shellVol->GetNdaughters(); ++k)
            {
                TGeoNode* sphereNode = shellVol->GetNode(k);
                TGeoVolume* sphereVol = sphereNode->GetVolume();

                if (sphereVol->GetShape()->IsA() == TGeoSphere::Class())
                    nSpheres++;
            }

            totalSphereNodes += nSpheres;

            std::cout << "  Shell assembly " << j
                      << ": " << shellVol->GetName()
                      << " | daughters = " << shellVol->GetNdaughters()
                      << " | TGeoSpheres = " << nSpheres
                      << "\n";
        }

        std::cout << "  Total shell assemblies: "
                  << totalShellAssemblies << "\n";

        std::cout << "  Total TGeoSphere placements: "
                  << totalSphereNodes << "\n";
        }
        else {
            std::cout << "  Unique volume.\n";
            if (mainVol->GetShape()->IsA() == TGeoSphere::Class())
                std::cout << "  Shape is a TGeoSphere.\n";
        }
    }

    std::cout << "\n=====================================\n";

}

