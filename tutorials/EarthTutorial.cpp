#include "Earth.h"

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


void EarthTutorial() {

    ModularEarth::Earth ToyEarth("TestName", "prem_44layers", 60, 60);
    ToyEarth.SetEarthModel("prem_44layers");
    ToyEarth.GetModelSummary();
    ToyEarth.BuiltEarth();

    ToyEarth.GetGeoManager()->SetVisLevel(3);      // reach blocks at depth 4
    //ToyEarth.GetGeoManager()->DefaultColors();     // unique color per material/layer
    ToyEarth.GetTopVolume()->SetVisibility(kFALSE); // hide outer sphere so layers show through

    TCanvas *c = new TCanvas("c", "EarthTutorial", 1200, 900);
    ToyEarth.GetTopVolume()->Draw("gl");
    c->Update();

}