void ModularEarthTutorial() {

    //Comparion with OscProbe

    TMultiGraph * ThePaths = new TMultiGraph();

    TGraph * ModularEarthProfile = new TGraph();

    TGraph * OscProbProfile = new TGraph();

    /////////////////////////////////////////
    ModularEarth ToyEarth;
    ToyEarth.SetEarthBinning(30.0,60.0);
    ToyEarth.SetEarthModel("./PremTables/prem_15layers.txt");

    //== Target Sublayers for ubdates ==
    std::vector<int> blockIDs = {1,5,6,9};
    std::vector<int> blockIDs1 = {2,3,6,10};
    std::vector<int> blockIDs2 = {4,7,9,12};
    std::string newName = "UpdatedBlockConfirm";
    std::vector<double> newA = {1,1,1,1};
    std::vector<double> newZ = {1,1,1,1};
    std::vector<double> newDensity = {1,1,1,1};

    //Update list
    std::vector<SublayersUpdates> updatelist;
    
    updatelist.push_back(SublayersUpdates(
        1, 1,                        // Layer index, Sublayer index
        blockIDs,                   // Block IDs
        newName,          // New material name
        newA,         // A
        newZ,               // Z
        newDensity             // Densities
    ));

    updatelist.push_back(SublayersUpdates(
        2, 2,                        // Layer index, Sublayer index
        blockIDs1,                   // Block IDs
        newName,          // New material name
        newA,         // A
        newZ,               // Z
        newDensity             // Densities
    ));

    updatelist.push_back(SublayersUpdates(
        1, 3,                        // Layer index, Sublayer index
        blockIDs2,                   // Block IDs
        newName,          // New material name
        newA,         // A
        newZ,               // Z
        newDensity             // Densities
    ));

    //ToyEarth.UpdateLayers(updatelist);
    ToyEarth.BuildEarth();

    ToyEarth.SetDetectorLocation(0.0,-90.0,6371); //lon lat rad:  Currently for SouthPole;
    ToyEarth.GetEarthPath(179.99,0.0); 
    ToyEarth.Visualize();

    /////// Comparison with OscProb
    //TCanvas* c1 = new TCanvas("c1","Paths (TMultiGraph/TGraph)", 800, 600);
    //c1->cd();

    //ThePaths->Add(ModularEarthProfile);   // assuming ThePaths is a TMultiGraph
    //ThePaths->Draw("APL");                // axes, points, line
    // ThePaths->Add(OscProbProfile);     // if/when needed
    //c1->Update();
    //ThePaths->Add(OscProbProfile);

    ///////
    //TCanvas* c2 = new TCanvas("c2","Geometry (TGeo + 3D markers)", 900, 600);
    //c2->cd();
    //top->Draw("gl");
    //detMarker->Draw("same");
    //NuDetMarkers->Draw("same");
    //trackingpoints->Draw("same");
    //srcDetMarkers->Draw("same");
    //nuLine->Draw("same");
    //TView *view = gPad->GetView();
    //c2->Update();


    //view->ShowAxis();
}