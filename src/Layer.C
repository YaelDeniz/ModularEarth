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

        for (const auto & cfg: configs) {
            Sublayer sub;
            sub.SetSubLayerID(cfg.layerID);
            sub.SetSubLayerMaterial(cfg.materialName, cfg.atomicMass, cfg.atomicNumber, cfg.density);
            sub.CreateSubLayerBlocks(cfg.rmin, cfg.rmax, dphi, dth);
            sublayers.push_back(sub);
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
        int id = 1;
        for (auto & sl: sublayers) {
            sl.BuildSubLayer();
            layerVolume -> AddNode(sl.GetSubLayerVolume(), id++);
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
        
        Sublayer sl = sublayers[sublayerIndex - 1];
        sl.GetAllBlockSummary();
    }

    void Layer::BlockSummary(size_t sublayerIndex, int blockIndex) const {
        if (sublayerIndex < 1 || sublayerIndex > sublayers.size()) {
            std::cerr << "Invalid sublayer index.\n";
            return;
        }
        
        Sublayer  sl = sublayers[sublayerIndex - 1];
        sl.GetBlockSummary(blockIndex);
    }

    void Layer::LayerSummary() const {
        std::cout << "=== Layer Summary: " << layerName << " ===\n";
        std::cout << " Number of Sublayers:" << sublayers.size() << std::endl;
    }

    
    void Layer::SubLayerBlockMap( int sl_id) const
    {
        // GetTopNode() gives the top node — not GetNode(0) which searches the current branch
        TGeoNode* TopNode = gGeoManager->GetTopNode();

        TString TopNode_path = TString("/") + TopNode->GetName();

        //TGeoVolume* not TGeoVolumeNode* — that type does not exist
        TGeoVolume* Top = gGeoManager->GetTopVolume();

        int nDaughters_top = Top->GetNdaughters();


        if (sl_id > nDaughters_top){ printf("Idex cannot exceed DAUGHTERS IN TOP (# OF SUBLAYERS): %i\n", nDaughters_top);      
        }
        else {
        int node_id = sl_id - 1;

        TGeoNode* SubLayerNode = Top->GetNode(node_id);

        TString SubLayer_node_name = SubLayerNode->GetName();
        
        TString SubLayerNode_path =  TopNode_path + TString("/") + SubLayer_node_name;

        printf("SUBLAYER NODE [%d]: %s\n", sl_id, SubLayer_node_name.Data());

        printf("SUBLAYER Path [%d]: %s\n", sl_id, SubLayerNode_path.Data());

        sublayers[node_id].GetBlockVertices(gGeoManager, SubLayerNode_path);
        }
        
    }

};




 