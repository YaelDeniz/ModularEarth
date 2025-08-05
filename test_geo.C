#include <TGeoManager.h>
#include <TGeoMaterial.h>
#include <TGeoMedium.h>
#include <TGeoVolume.h>
#include <TGeoShape.h>
#include <TGeoMatrix.h>

void test_geo() {
    // Create a ROOT geometry manager
    TGeoManager *geom = new TGeoManager("world", "Test geometry with ROOT");

    // Define a vacuum material and medium
    TGeoMaterial *matVacuum = new TGeoMaterial("Vacuum", 0, 0, 0);
    TGeoMedium *medVacuum = new TGeoMedium("Vacuum", 1, matVacuum);

    // Create a world box (TOP volume)
    TGeoVolume *top = geom->MakeBox("TOP", medVacuum, 200, 200, 200);
    geom->SetTopVolume(top);

    // Create a cube volume inside the world
    TGeoVolume *cube = geom->MakeBox("Cube", medVacuum, 100, 100, 100);
    top->AddNode(cube, 1, new TGeoTranslation(0, 0, 0));

    // Create a sphere inside the cube
    TGeoVolume *sphere = geom->MakeSphere("Sphere", medVacuum, 0, 50);
    top->AddNode(sphere, 2, new TGeoTranslation(0, 0, 0));

    // Close and validate geometry
    geom->CloseGeometry();
    geom->CheckOverlaps();

    // Draw the top volume with OpenGL viewer
    top->Draw("gl");  // Launches 3D viewer
}
