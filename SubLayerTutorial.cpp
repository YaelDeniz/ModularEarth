#include "inc/Sublayer.h" 
//Test for Tracking in SubLayers
void SubLayerExample() {

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

    top->AddNode(TestSubLayer1.GetSubLayerVolume() // GetSubLayerVolume return the actual TGeoManager Volume, this need to be add to the top volume or to anothr TGeoAssembly.
                , 1, new TGeoTranslation(0, 0, 0));

    ModularEarth::Sublayer TestSubLayer2;
    TestSubLayer2.SetSubLayerID(1); //Set SubLayer Idex
    TestSubLayer2.SetSubLayerMaterial("TestMaterial2", 2,3,23.4); //Sublayer Material must be defined before creating sublayer volume
    TestSubLayer2.CreateSubLayerBlocks(3880,4680,60,30); //Create a sublayer given their inner and outer radius and lon and lat bin size
    TestSubLayer2.BuildSubLayer(); // Build the layer

    top->AddNode(TestSubLayer2.GetSubLayerVolume() // GetSubLayerVolume return the actual TGeoManager Volume, this need to be add to the top volume or to anothr TGeoAssembly.
                , 1, new TGeoTranslation(0, 0, 0));

    //TGeoVolume* testvol = new TGeoVolume("TestVolumeEarth", new TGeoSphere("SubLayerTestVolume", 3480, 3880), medVac);
    
    //top->AddNode(testvol,1, new TGeoTranslation(0, 0, 0));
    
    geoManager->CloseGeometry(); //Here we finished the model creation. "Closed" means that no more modification can be made to the model.
    geoManager->SetTopVisible();     // Ensure top volume shows up

    //Tracking Example
    double lat = 0.0;
    double lon = 90.0;
    double radius = 6371;
    
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
    double zenith = 180.0 - (epszenith);
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

    std::vector<std::pair<const TGeoMaterial*, double>> segs;

    auto& geo = *gGeoManager;
    geo.InitTrack(r_src.X(), r_src.Y(), r_src.Z(), n_dir.X(), n_dir.Y(), n_dir.Z());
    if (!geo.FindNode()) return; // outside

    // If starting on a boundary, nudge forward a hair
    if (geo.IsOnBoundary()) {
        const double eps = 1e-7; // cm
        double x[3] = {geo.GetCurrentPoint()[0],geo.GetCurrentPoint()[1],geo.GetCurrentPoint()[2]};
        double d[3] = {geo.GetCurrentDirection()[0],geo.GetCurrentDirection()[1],geo.GetCurrentDirection()[2]}; 
        geo.SetCurrentPoint(x[0]+eps*d[0], x[1]+eps*d[1], x[2]+eps*d[2]);
        geo.FindNode();
    }

    //Some Tolerance
    const double BIG = 1e30;
    const double eps = 1e-7; // cm

    while (!geo.IsOutside()) {
        // ---- The material you are currently inside ----
        TGeoVolume* vol = geo.GetCurrentVolume();
        TGeoMaterial* mat = vol && vol->GetMedium() ? vol->GetMedium()->GetMaterial() : nullptr;

        // Ask distance to the next boundary and STEP there (this updates point/path)
       
        geo.FindNextBoundaryAndStep(); // s is the path inside *current* material
        
        double s = geo.GetStep(); // step to next boundary
        // Record / print the segment you just traversed
        if (s > 0 && mat) {
            segs.emplace_back(mat, s);

            // Optional: log *after* stepping, so coordinates are at the boundary you reached
            double x[3] = {geo.GetCurrentPoint()[0],geo.GetCurrentPoint()[1],geo.GetCurrentPoint()[2]};
            double r_bnd = std::sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);

            std::cout << std::fixed << std::setprecision(3)
            << "Boundary radius " << r_bnd
            << " Coordinates " << x[0] << " " << x[1] << " " << x[2]
            << " | Step Length: " << s << " cm\n";
            std::cout << "ρ: " << mat->GetDensity()
            << " g/cm³, Z/A: " << (mat->GetA() ? mat->GetZ()/mat->GetA() : 0.0)
            << ", Material: " << mat->GetName()
            << ", Volume: " << (vol ? vol->GetName() : "NULL") << "\n\n"
            << std::boolalpha << "  Entering Volume: " << gGeoManager->IsEntering() 
            << std::boolalpha << "  Exiting Volume: " << gGeoManager->IsExiting() << "\n\n" << std::endl;
        }

        if (geo.IsOutside()) break;

        // If we’re sitting on a boundary, nudge into the next volume to avoid re-crossing
        //if (geo.IsOnBoundary() || geo.IsCrossingBoundary()) {
            //double x[3]; geo.GetCurrentPoint();
            //double d[3]; geo.GetCurrentDirection();
            //geo.SetCurrentPoint(x[0]+eps*d[0], x[1]+eps*d[1], x[2]+eps*d[2]);
          //  geo.FindNode();
        //}
    }

    //////

    top->Draw("gl");
    //detMarker->Draw("same");
    NuDetMarkers->Draw("same");
    //trackingpoints->Draw("same");
    //srcDetMarkers->Draw("same");
    nuLine->Draw("same");
    TView *view = gPad->GetView();
    //view->ShowAxis();
}
