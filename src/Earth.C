#include "Earth.h"
#include "Layer.h"
#include "Sublayer.h"

#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<fstream>
#include <sstream>

#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoVolume.h"
#include "TGeoShape.h"
#include "TGeoMatrix.h"
#include "TH1F.h"
#include "TPolyMarker3D.h"
#include "TPolyLine3D.h"
#include "TView.h"
#include "TCanvas.h"
#include "TGraph.h"

namespace ModularEarth {

    // Parse Earth model from file (rmax, ρ, Z/A, index)
    void Earth::SetEarthModel(TString table_name) {

        TString ModularEarthDir = gSystem->pwd();
        TString ModelTableDir = ModularEarthDir + "/tables/";
        std::string filename;
        if (table_name.IsNull()){
        
        //convert TString to string

        TString TablePath = ModelTableDir + TableName + ".txt";
        filename=TablePath.Data(); //Assign default name
        }
        else
        {

        TString TablePath = ModelTableDir + table_name + ".txt";
        filename=TablePath.Data();

        }

        std::ifstream infile(filename);
        if (!infile) {
            std::cerr << "ERROR: Could not open model file: " << filename << std::endl;
            return;
        }

        std::string line;
        double radiusprev = 0.0;
        int indexprev = -1;
        int slindex = 1;

        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            double radius, density, z_a;
            int index;

            if (!(iss >> radius >> density >> z_a >> index)) {
                std::cerr << "WARNING: Invalid line skipped: " << line << std::endl;
                continue;
            }

            if (index != indexprev && indexprev != -1) {
                layersconfigs.push_back({"Layer", indexprev+1, theta_width, phi_width, sublayersconfigs});
                sublayersconfigs.clear();
                slindex = 1;
            }

            sublayersconfigs.push_back({slindex, radiusprev, radius, density, a_mass, z_a, "SublayerMaterial"});
            radiusprev = radius;
            indexprev = index;
            slindex++;
        }

        if (!sublayersconfigs.empty()) {
            layersconfigs.push_back({"Layer", indexprev+1, theta_width, phi_width, sublayersconfigs});
        }
    }

    void Earth::GetConfigsSummary() const {
        if (layersconfigs.empty()) {
            std::cout << "No layer configuration loaded in the current Earth model.\n"
                    << "Use Earth::SetEarthModel to load a predefined model, "
                    << "or add layers before calling the config summary.\n";
            return;
        }

        for (size_t ly = 0; ly < layersconfigs.size(); ++ly) {
            const auto& layer = layersconfigs[ly];

            std::cout << "\n--- Layer index: " << ly << "Index check: " << layer.layerindex << " ---\n";

            if (layer.sublayersconfig.empty()) {
                std::cout << "  Incomplete configuration: no sublayers in this layer. "
                        << "Please redo the configs.\n";
                continue;
            }

            for (const auto& slcfg : layer.sublayersconfig) {
                std::cout << "  Sublayer " << slcfg.layerID
                        << " | rmin: " << slcfg.rmin
                        << " | rmax: " << slcfg.rmax
                        << " | \u03c1: "   << slcfg.density
                        << " | Z/A: " << slcfg.atomicMass << "\n";
            }
        }
    }

    // Change angular resolution (before building Earth)
    void Earth::SetEarthBinning(double dth, double dphi) {
        theta_width = dth;
        phi_width = dphi;
    }

    

};
