#include "Layer.h"
#include "Sublayer.h"

#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<fstream>

#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoVolume.h"
#include "TGeoShape.h"
#include "TGeoMatrix.h"
#include "TH1F.h"
#include "TPolyLine3D.h"
#include "TPolyMarker3D.h"
#include "TPolyLine3D.h"
#include "TView.h"
#include "TCanvas.h"
#include "TGraph.h"

namespace ModularEarth {

    void Layer::SetSubLayers(const std::vector < SublayerConfig > & configs) {
        layerVolume = new TGeoVolumeAssembly(layerName.c_str());

        // Pre-allocate so push_back never reallocates the sublayers vector mid-loop.
        // Without this, the vector's growth triggers Sublayer's copy constructor on existing
        // elements. Because Sublayer holds std::string members whose move constructor is not
        // guaranteed noexcept on all platforms, std::vector falls back from move to copy,
        // which shallow-copies TGeoVolume* pointers and crashes inside ROOT's object tracking.
        sublayers.reserve(configs.size());

        for (const auto & cfg: configs) {
            Sublayer sub;
            sub.SetSubLayerIndex(cfg.sublayerID, i_ly);
            sub.SetSubLayerMaterial(cfg.materialName, cfg.atomicMass, cfg.atomicNumber, cfg.density);
            sub.CreateSubLayerBlocks(cfg.rmin, cfg.rmax, dphi, dth);
            sublayers.push_back(std::move(sub));
        }
    }

    void Layer::UpdateBlocksInSubLayer(size_t sublayerIndex,
        const std::vector < int > & blockIDs,
            const std::string & newName,
                const std::vector < double > & newA,
                    const std::vector < double > & newZ,
                        const std::vector < double > & newDensity) {
        if (sublayerIndex < 1 || sublayerIndex > sublayers.size()) {
            std::cerr << "Sublayer index " << sublayerIndex << " is out of bounds! Skipping...\n";
            return;
        }

        if (blockIDs.size() != newA.size() ||
            blockIDs.size() != newZ.size() ||
            blockIDs.size() != newDensity.size()) {
            std::cerr << "Inconsistent sizes in block update vectors.\n";
            return;
        }

        Sublayer & sl = sublayers[sublayerIndex - 1];
        bool anyModified = false;

        for (size_t i = 0; i < blockIDs.size(); ++i) {
            sl.UpdateBlockMaterial(blockIDs[i], newName, newA[i], newZ[i], newDensity[i]);
            std::cout << "Block " << blockIDs[i] <<
                " in Sublayer " << sublayerIndex <<
                " updated with material: " << newName << "\n";
            anyModified = true;
        }

        if (anyModified) {
            sl.BuildSubLayer(); // Rebuild only if changes were made
        } else {
            std::cerr << "No valid blocks were modified.\n";
        }
    }

    void Layer::UpdateSublayer(size_t sublayerIndex,
        const std::string & newName,
            double newA,
            double newZ,
            double newDensity) {
        if (sublayerIndex < 1 || sublayerIndex > sublayers.size()) {
            std::cerr << "Invalid sublayer index: " << sublayerIndex << "\n";
            return;
        }

        Sublayer & sl = sublayers[sublayerIndex - 1];
        sl.UpdateAllBlocksMaterial(newName, newA, newZ, newDensity);
        sl.BuildSubLayer();

    }

    void Layer::BuildLayer() {
        for (auto & sl: sublayers) {
            sl.BuildSubLayer();
            layerVolume->AddNode(sl.GetSubLayerVolume(), sl.GetSubLayerIndex());
        }
    }

    TGeoVolumeAssembly * Layer::GetLayerVolume() const {
        return layerVolume;
    }


    void Layer::SubLayerSummary(size_t sublayerIndex) const {
        if (sublayerIndex < 1 || sublayerIndex > sublayers.size()) {
            std::cerr << "Invalid sublayer index.\n";
            return;
        }
        
        // Use a const reference — Sublayer copy is deleted to prevent aliasing ROOT pointers.
        const Sublayer& sl = sublayers[sublayerIndex - 1];
        sl.GetAllBlockSummary();
    }

    void Layer::BlockSummary(size_t sublayerIndex, int blockIndex) const {
        if (sublayerIndex < 1 || sublayerIndex > sublayers.size()) {
            std::cerr << "Invalid sublayer index.\n";
            return;
        }
        
        // Use a const reference — Sublayer copy is deleted to prevent aliasing ROOT pointers.
        const Sublayer& sl = sublayers[sublayerIndex - 1];
        sl.GetBlockSummary(blockIndex);
    }

    void Layer::LayerSummary() const {
        std::cout << "=== Layer Summary: " << layerName << " ===\n";
        std::cout << " Number of Sublayers:" << sublayers.size() << std::endl;
    }

    void Layer::Call_The_Roll(){
        
        Printf("Calling the roll for current model setup: Method from Layer %s ===\n", layerName.c_str());
        TGeoNode* TopNode = gGeoManager->GetTopNode();

        TString TopNode_path = TString("/") + TopNode->GetName();

        //TGeoVolume* volume 
        TGeoVolume* Top = gGeoManager->GetTopVolume();

        int nDaughters_top = Top->GetNdaughters(); // Number of Layers in the top volume.

        for (int k = 0; k < nDaughters_top; ++k) {
            TGeoNode* TestNode = Top->GetNode(k);
            TString TestNode_name = TestNode->GetName();
            printf("NODE IN TOP [%d]: %s\n", k, TestNode_name.Data());
            TGeoVolumeAssembly* TestAssembly = dynamic_cast<TGeoVolumeAssembly*>(TestNode->GetVolume());
            if (!TestAssembly) {
                printf("  WARNING: %s is not a TGeoVolumeAssembly\n",
                TestNode->GetName());
                continue;
            }
            else {
                int nsublayer = TestAssembly->GetNdaughters();
                printf("Total number of SubLayers in Layer %d: %i\n", k, nsublayer);
            }
        }

    }
    
    void Layer::SubLayerBlockMap( int sl_id) const
    {

        TGeoNode* TopNode = gGeoManager->GetTopNode();
        TString TopNode_path = TString("/") + TopNode->GetName();

        TGeoVolume* Top = gGeoManager->GetTopVolume();
        int nDaughters_top = Top->GetNdaughters();

        printf("Total layers in top volume: %i\n", nDaughters_top);
        printf("This Layer's index (i_ly): %i\n", i_ly);

        // Find layer node by copy number (robust against skipped layers).
        TGeoNode* LayerNode = nullptr;
        for (int k = 0; k < nDaughters_top; ++k) {
            TGeoNode* n = Top->GetNode(k);
            if (n && n->GetNumber() == i_ly) { LayerNode = n; break; }
        }

        if (!LayerNode) {
            printf("  WARNING: LayerNode is null for i_ly %d\n", i_ly);
            return;
        }

        TString LayerNode_name =  LayerNode->GetName();

        TString LayerNode_path =  TopNode_path + TString("/") + LayerNode_name;

        TGeoVolumeAssembly* LayerAssembly = dynamic_cast<TGeoVolumeAssembly*>(LayerNode->GetVolume()); // ssign variable type to LayerAssembly

        if (!LayerAssembly) {
            printf("  WARNING: %s is not a TGeoVolumeAssembly\n",
            LayerNode->GetName());
        }
        else {

            int nDaughters_in_layer = LayerAssembly->GetNdaughters();
            
            printf("Total number of SubLayers mother Layer: %i\n", nDaughters_in_layer);

            if (sl_id < 1 || sl_id > nDaughters_in_layer){
                
                printf("Index cannot be < 1 or exceed daughters in mother layer (# OF SUBLAYERS): %i\n", nDaughters_in_layer);      
            
            }

            else {
                // Find sublayer node by copy number (robust against non-sequential IDs).
                TGeoNode* SubLayerNode = nullptr;
                int SubLayerPos = -1;
                for (int k = 0; k < nDaughters_in_layer; ++k) {
                    TGeoNode* n = LayerAssembly->GetNode(k);
                    if (n && n->GetNumber() == sl_id) { SubLayerNode = n; SubLayerPos = k; break; }
                }

                if (!SubLayerNode) {
                    printf("  WARNING: SubLayerNode not found for sl_id %d\n", sl_id);
                    return;
                }

                TString SubLayerNode_name = SubLayerNode->GetName();

                TString SubLayerNode_path =  LayerNode_path + TString("/") + SubLayerNode_name;

                printf("SUBLAYER node name [%d]: %s\n", sl_id, SubLayerNode_name.Data());

                printf("SUBLAYER Path [%d]: %s\n", sl_id, SubLayerNode_path.Data());

                TString filename = LayerNode_name + "-" + SubLayerNode_name + "_BlockMap.csv";

                sublayers[SubLayerPos].GetBlockVertices(gGeoManager, SubLayerNode_path, filename);
            }
        }
    } //SubLayerBlockMap

};




 