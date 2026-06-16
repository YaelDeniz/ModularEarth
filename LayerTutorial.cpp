void TesselatedEarth() {
    //Comparion with OscProbe

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
    //      std::vector<SublayerConfig> sublayersconfig = {
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




    //Layer InnerCore("InnerCore",0,30,60); // Layer constructor indicating Layer name, index, lon bin size and lat bin size
    std::vector<SublayerConfig> sublayersconfigIC = {{1,0, 1221.5, 12.9807, 0.4691, 1, "Iron"}}; //Struct containing the list of sublayers of a layer
    //syntax {sublayer index, rmin,rmax,Z,A,rho, material name}
    //InnerCore.SetSubLayers(sublayersconfigIC);
    //InnerCore.BuildLayer();
    //Make Sphere

    TGeoMaterial *matIC = new TGeoMaterial("SolidNiFe",  0.4691, 1, 12.9807);
    TGeoMedium *medIC = new TGeoMedium("SolidNiFe", 1, matIC);
    TGeoVolume* InnerCore = new TGeoVolume("InnerCore", new TGeoSphere("Sphere", 0, 1221.5), medIC);

    //
    Layer OuterCore("OuterCore",1,30,60);
    std::vector<SublayerConfig> sublayersconfigOC = {
        {1,1221.5, 1946.7, 11.9512, 0.4691, 1, "FeNi"},
        {2,1946.7, 2475.1, 11.4726, 0.4691, 1, "FeNi"},
        {3,2475.1, 2900.3, 10.9884, 0.4691, 1, "FeNi"},
        {4,2900.3, 3260.6, 10.5008	, 0.4691, 1, "FeNi"},
        {5,3260.6, 3480.0, 10.0861, 0.4691, 1, "FeNi"}
    };
    OuterCore.SetSubLayers(sublayersconfigOC);
    OuterCore.BuildLayer();

    Layer LowerMantle("LowerMantle",2,30,60);
    std::vector<SublayerConfig> sublayersconfigLM = {
        {1,3480.0, 4476.0, 5.3225, 0.4954, 1, "SilicateLM"},
        {2,4476.0, 5378.4, 4.8332, 0.4691, 1, "SilicateLM"},
        {3,5378.4, 5701.0, 4.4872, 0.4691, 1, "SilicateLM"}
    };
    LowerMantle.SetSubLayers(sublayersconfigLM);
    LowerMantle.BuildLayer();

    Layer UpperMantle("UpperMantle",3,30,60);
    std::vector<SublayerConfig> sublayersconfigUM = {
        {1,5701.0, 5971.0, 3.8983	, 0.4954, 1, "SilicateUM"},
        {2,5971.0, 6346.6, 3.4306	, 0.4691, 1, "SilicateUM"}
    };
    UpperMantle.SetSubLayers(sublayersconfigUM);
    UpperMantle.BuildLayer();

    Layer Crust("Crust",4,30,60);
    std::vector<SublayerConfig> sublayersconfigC = {
        {1,6346.6, 6356.0, 2.9000	, 0.4956, 1, "SilicateC"},
        {2,6356.0, 6368.0, 2.6000	, 0.4956, 1, "SilicateC"}
    };
    Crust.SetSubLayers(sublayersconfigC);
    Crust.BuildLayer();

    Layer Ocean("Ocean",5,30,60);
    std::vector<SublayerConfig> sublayersconfigO = {
        {1,6368.0, 6371.0, 1.0200	, 0.5525, 1, "SilicateO"}
    };
    Ocean.SetSubLayers(sublayersconfigO);
    Ocean.BuildLayer();

    Layer Atmosphere("Atmosphere",6,30,60);

      std::vector<SublayerConfig> sublayersconfigAtm = {
        {1,6371.0, 6386.0	, 0.001	, 0.4991, 1, "Vac"}
    };
    Atmosphere.SetSubLayers(sublayersconfigAtm);
    Atmosphere.BuildLayer();


    //top->AddNode(InnerCore.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    top->AddNode(InnerCore, 1,new TGeoTranslation(0, 0, 0));
    top->AddNode(OuterCore.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    top->AddNode(LowerMantle.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    top->AddNode(UpperMantle.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    top->AddNode(Crust.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    top->AddNode(Ocean.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    top->AddNode(Atmosphere.GetLayerVolume(), 1,new TGeoTranslation(0, 0, 0));
    

    /////////////////////////////
    
    geoManager->CloseGeometry(); //Here we finished the model creation. "Closed" means that no more modification can be made to the model.
    geoManager->SetTopVisible();     // Ensure top volume shows up

    //Tracking Example--------------------------------------------------------------------------------------------------------------------------
    double lat = -90.0;
    double lon = 0.0;
    double radius = 6368;
    
    double lat_rad = lat * M_PI / 180.0;
    double lon_rad = lon * M_PI / 180.0;

    TVector3 x_hat, y_hat, z_hat;

    // Compute detector location in right handed cartesian coordinate
    double x_det = radius * cos(lat_rad) * cos(lon_rad);
    double y_det = radius * cos(lat_rad) * sin(lon_rad);
    double z_det = radius * sin(lat_rad);

    TVector3 r_det(x_det, y_det, z_det);  // Vector from Earth's center to detector

    //Detector local coordinate system set up:

    // Detector local +z axis is outward radial direction
    z_hat = r_det.Unit();
    // Handle poles: cross with X instead of Z
    if (fabs(z_hat.Z()) > 0.999) {
        x_hat = TVector3(1, 0, 0).Cross(z_hat).Unit();
    } else {
        x_hat = TVector3(0, 0, 1).Cross(z_hat).Unit();  // Local East
    }
    y_hat = z_hat.Cross(x_hat).Unit();  // Local North
    
    //Neutrino Direction in detectors coordinate system 
    double epszenith = 1E-4; //Tolerance for epsilon path 
    //double zenith = 180.0 - (epszenith);
    double zenith = 145;
    double azimuth = 0.0;

    // Convert to radians
    double zen_rad = (180-zenith) * M_PI / 180.0;
    double az_rad  = azimuth * M_PI / 180.0;
    // Neutrino direction in detector frame
    TVector3 dir_local(
        sin(zen_rad) * cos(az_rad),
        sin(zen_rad) * sin(az_rad),
        cos(zen_rad)
    );

    // Convert to global direction using unit vetors representing the detector's local coordinate system
    TVector3 dir_global =
        x_hat * dir_local.X() +
        y_hat * dir_local.Y() +
        z_hat * dir_local.Z();

    TVector3 n_dir = dir_global.Unit();  // Normalize
    TVector3 r0(x_det, y_det, z_det);    // Detector location in RHCC

    //Neutrino Source Location
    // Find the entry point: intersection with R=6386 km (atmosphere height)
    double R = 6386.0;  // km
    double a = 1.0;
    double b = -2.0 * r0.Dot(n_dir);
    double c = r0.Mag2() - R * R;

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        std::cerr << "No intersection with atmospheric sphere!\n";
        return;
    }

    double sqrt_disc = sqrt(discriminant);
    double t1 = (-b + sqrt_disc) / (2.0 * a);
    double t2 = (-b - sqrt_disc) / (2.0 * a);

    // Choose the farther point from the detector (source position)
    double t_entry = std::max(t1, t2);
    TVector3 r_src = r0 - t_entry * n_dir;
    TVector3 rL = r0-r_src;    // Detector location in RHCC

    //Draw The Neutrino Path
    TPolyLine3D* nuLine = new TPolyLine3D(2);
    nuLine->SetPoint(0, r_src.X(), r_src.Y(), r_src.Z());
    nuLine->SetPoint(1, r0.X(),    r0.Y(),    r0.Z());
    nuLine->SetLineColor(kBlue);
    nuLine->SetLineWidth(3);
   
    // Detector marker
    //TPolyMarker3D* detMarker = new TPolyMarker3D(1,1);
    //detMarker->SetPoint(0, r0.X(), r0.Y(), r0.Z());
    //detMarker->SetMarkerColor(6);
    //detMarker->SetMarkerSize(3);

    // Neutrino source marker
    //TPolyMarker3D* srcMarker = new TPolyMarker3D(1,1);
    //srcMarker->SetPoint(0, r_src.X(), r_src.Y(), r_src.Z());
    //srcMarker->SetMarkerColor(9);
    //srcMarker->SetMarkerSize(3);

    TPolyMarker3D *NuDetMarkers= new TPolyMarker3D(100,2); //Markers indicating DetLoc and Inical Neutrino
    NuDetMarkers->SetPoint( 0 , r0.X(),    r0.Y(),    r0.Z()); //Detector
    NuDetMarkers->SetPoint( 1 ,r_src.X(),r_src.Y(),r_src.Z()); //Incoming Neutrino
    //NuDetMarkers->SetPoint( 2 , 0.0,0.0,-3680.000); //Incoming Neutrino
    
    NuDetMarkers->SetMarkerColor(kRed);
    NuDetMarkers->SetMarkerSize(5);

    //TPolyMarker3D *srcDetMarkers= new TPolyMarker3D(2,2); //Markers indicating DetLoc and Inical Neutrino
    //NuDetMarkers->SetPoint( 0 , r0.X(),    r0.Y(),    r0.Z()); //Detector
    //NuDetMarkers->SetPoint( 0 ,r_src.X(),r_src.Y(),r_src.Z()); //Incoming Neutrino
    //NuDetMarkers->SetMarkerColor(kRed);
    //NuDetMarkers->SetMarkerSize(3);

    std::cout << "Magnitude test in the atmosphere" << sqrt( r_src.Mag2() ) << std::endl;
    std::cout << "Magnitude test detector location" << sqrt( r0.Mag2() ) << std::endl;
    std::cout << "Magnitude test normal vector" << sqrt( n_dir.Mag2() ) << std::endl;
    std::cout << "Total Propagation distance" <<  sqrt(rL.Mag2()) << std::endl;

    std::cout << "Source point radius: " << sqrt(r_src.X()*r_src.X() + r_src.Y()*r_src.Y() + r_src.Z()*r_src.Z())  << std::endl;
        
    ////// WIP : Tracking Algorithm


    auto& geo = *gGeoManager;
    geo.InitTrack(r_src.X(), r_src.Y(), r_src.Z(), n_dir.X(), n_dir.Y(), n_dir.Z());
    double SumL = 0; 
    if (!geo.FindNode()) return;

    //NUDGING
    auto nudge_into_next = [&]{

    const double epsnudge = 1e-5; // cm
    std::cout << "Inside of nudging function. Tolerance: "<< epsnudge << std::endl;
    double X[3] = { geo.GetCurrentPoint()[0], geo.GetCurrentPoint()[1], geo.GetCurrentPoint()[2] };
    double D[3] = { geo.GetCurrentDirection()[0], geo.GetCurrentDirection()[1], geo.GetCurrentDirection()[2] };
    geo.SetCurrentPoint(X[0]+epsnudge*D[0], X[1]+epsnudge*D[1], X[2]+epsnudge*D[2]);
    geo.FindNode();
    };

    if (geo.IsOnBoundary()) nudge_into_next();

    const double BIG  = 1e30;
    const double ZEPS = 1e-10;

    while (!geo.IsOutside()) {
    // volume BEFORE stepping (material you’re in)
    TGeoVolume* volBefore = geo.GetCurrentVolume();
    TGeoVolume* vol = geo.GetCurrentVolume();
    TGeoMaterial* mat = (volBefore && volBefore->GetMedium())
                            ? volBefore->GetMedium()->GetMaterial() : nullptr;

    // step to next boundary
     geo.FindNextBoundaryAndStep(BIG, kTRUE);
     double s = geo.GetStep();
    double cradius = 0;
    // guard: if we got a zero step on a boundary, nudge and try next iteration
    if (s <= ZEPS && geo.IsOnBoundary()) { nudge_into_next(); continue; }

    // log segment (after step)
    if (mat && s > ZEPS) {
        double X[3] = { geo.GetCurrentPoint()[0], geo.GetCurrentPoint()[1], geo.GetCurrentPoint()[2] };
        double r_bnd = std::sqrt(X[0]*X[0] + X[1]*X[1] + X[2]*X[2]);
        cradius=r_bnd;


        /*
        std::cout << std::fixed << std::setprecision(3)
            << "Boundary radius " << r_bnd
            << " Coordinates " << X[0] << " " << X[1] << " " << X[2]
            << " | Step Length: " << s << " Path Length" << SumL << " cm\n";
            std::cout << "ρ: " << mat->GetDensity()
            << " g/cm³, Z/A: " << (mat->GetA() ? mat->GetZ()/mat->GetA() : 0.0)
            << ", Material: " << mat->GetName()
            << ", Volume: " << (vol ? vol->GetName() : "NULL") << "\n\n"
            << std::boolalpha << "  Entering Volume: " << gGeoManager->IsEntering() 
            << std::boolalpha << "  Exiting Volume: " << gGeoManager->IsExiting() << "\n\n" << std::endl;
        */

            SumL += s;
            ModularEarthProfile->AddPoint(SumL,mat->GetDensity());
            ModularPathL.push_back(SumL);
            ModularPathR.push_back(r_bnd);
            ModularPathDensity.push_back(mat->GetDensity());

    }

    if (geo.IsOutside()) break;

    // volume AFTER stepping (useful if you want from→to info)
    TGeoVolume* volAfter = geo.GetCurrentVolume();

    // if we’re exactly on a boundary, push into the next volume
    if (geo.IsOnBoundary()){
        std::cout << "NUDGIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIING into next boundary" << std::endl;
        nudge_into_next();

    }

    if (abs(cradius - radius) <= 1E-3 && SumL > 1000.0)
    {
        std::cout << "Neutrino Reach detector"  << cradius << " " << radius << " " << abs(cradius - radius)  << std::endl;
        break; // Stopping at detector location
    }
    

    // (optional) explicit cross-check without flags:
    // if (volAfter != volBefore) { /* crossed a boundary */ }
    }
    /////// Comparison with OscProb

    std::cout << " " << std::endl;

    std::string PremTables = "/home/ydenizhernandez/OscProb/PremTables/";
    std::string PremFile = "prem_15layers.txt";
    std::string PremPath = PremTables+PremFile;
    OscProb::PremModel prem(PremPath);
    double cth = cos(zenith * M_PI/180.0);
    std::cout << M_PI << " " << zen_rad << " " << zenith * M_PI/180.0 << std::endl;
    prem.FillPath(cth); // Fill paths from PREM model
    std::vector<OscProb::NuPath> paths = prem.GetNuPath();

    std::cout << paths.size() << " " << ModularPathL.size() << " " << std::endl;

    for (size_t i = 0; i < ModularPathL.size(); i++)
    {
        std::cout<< i << " " << ModularPathR[i] <<  " " << ModularPathL[i] << " " << ModularPathDensity[i] << std::endl;
    }
    
    std::cout << " " << std::endl;
    
    double sumL_OscProb = 0;

    for (int i = 0; i < paths.size() ; ++i)
    {

    sumL_OscProb += paths[i].length ; 

    OscProbProfile->SetPoint(i,sumL_OscProb,paths[i].density  );
    std::cout << i << " " << sumL_OscProb << " " << paths[i].density << std::endl;

    }

    std::cout << sumL_OscProb << " " << SumL << " " << std::endl;


    
    ///////

    TCanvas* c1 = new TCanvas("c1","Paths (TMultiGraph/TGraph)", 800, 600);
    c1->cd();

    ModularEarthProfile->SetLineColor(kRed);

    ThePaths->Add(ModularEarthProfile);   // assuming ThePaths is a TMultiGraph
    ThePaths->Add(OscProbProfile);
    ThePaths->Draw("APL");                // axes, points, line
    // ThePaths->Add(OscProbProfile);     // if/when needed
    c1->Update();
    //ThePaths->Add(OscProbProfile);

    ///////
    TCanvas* c2 = new TCanvas("c2","Geometry (TGeo + 3D markers)", 900, 600);
    c2->cd();
    top->Draw("gl");
    //detMarker->Draw("same");
    NuDetMarkers->Draw("same");
    //trackingpoints->Draw("same");
    //srcDetMarkers->Draw("same");
    nuLine->Draw("same");
    TView *view = gPad->GetView();
    c2->Update();


    //view->ShowAxis();

}