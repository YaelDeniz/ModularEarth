class Sublayer {

private:
    TGeoVolumeAssembly* SubLayer = nullptr;
    std::vector<Block> blocks;
    int SubLayerID = 1;
    std::string SubLayerName = Form("SubLayer_%d", SubLayerID);
    std::string SubLayerMaterialName = "Iron";
    double SubLayerA = 55.85;
    double SubLayerZ = 26;
    double SubLayerDensity = 7.87;

    double Nphi = 4;
    double Nth = 3;
    double dphi = 90;
    double dth = 60;

    // Internal function to rebuild the geometry assembly
    void BuildAssembly() {
        if (SubLayer) delete SubLayer;
        SubLayer = new TGeoVolumeAssembly(SubLayerName.c_str());

        for (const auto& b : blocks) {
            if (b.GetVolume() == nullptr) {
                std::cerr << "Block is empty volume!\n";
            }

            if (!b.IsValid()) {
                std::cerr << "Error: Invalid block detected. Blocks were not built properly\n";
                b.BlockSummary();
            }

            SubLayer->AddNode(b.GetVolume(), b.GetBlockID(), new TGeoTranslation(0, 0, 0));
        }
    }

public:
    Sublayer() = default;

    void GetAllBlockSummary() {
        for (const auto& b : blocks) {
            if (b.GetVolume() == nullptr) {
                std::cerr << "Block is empty volume!\n";
            } else if (!b.IsValid()) {
                std::cerr << "Error: Invalid block detected. Blocks were not built properly\n";
                b.BlockSummary();
            } else {
                b.BlockSummary();
            }
        }
    }

    void GetBlockSummary(int id) {
        if (id >= 1 && id <= blocks.size())
            blocks[id - 1].BlockSummary();
        else
            std::cerr << "Invalid block ID: " << id << std::endl;
    }

    TGeoVolumeAssembly* GetSubLayerVolume() const {
        return SubLayer;
    }

    void ModifyBlockMaterial(int id, const std::string& NewName, double NewA, double NewZ, double NewDensity) {
        if (id < 1 || id > blocks.size()) {
            std::cerr << "Block index out of bounds. Index must be between 1 and " << blocks.size() << std::endl;
            return;
        }

        Block modifiedBlock = blocks[id - 1];
        modifiedBlock.DefineBlockMaterial(NewName, NewA, NewZ, NewDensity);
        modifiedBlock.WasModified(true);
        modifiedBlock.BuildBlock();
        blocks[id - 1] = modifiedBlock;

        BuildAssembly(); //Rebuild the new assembly
    }

    //Modified multiple blocks
    void ModifyMultipleBlocksMaterial(const std::vector<int>& ids,
                                  const std::string& NewName,
                                  double NewA, double NewZ, double NewDensity) {
    bool anyModified = false;

    for (int id : ids) {
        if (id < 1 || id > blocks.size()) {
            std::cerr << "Block ID " << id << " is out of bounds! Skipping...\n";
            continue;
        }

        Block modifiedBlock = blocks[id - 1];
        modifiedBlock.DefineBlockMaterial(NewName, NewA, NewZ, NewDensity);
        modifiedBlock.WasModified(true);   // Mark as modified before building
        modifiedBlock.BuildBlock();        // Color will reflect change
        blocks[id - 1] = modifiedBlock;
        anyModified = true;
    }

    if (anyModified) {
        BuildAssembly();  // Rebuild only if at least one block changed
    } else {
        std::cerr << "No valid blocks were modified.\n";
    }
}

    //Create the blocks forming the Sublayer. The layer is binned based on the bin width
    void SetBlocks(double r1, double r2, double dphi, double dth) {

        //Bid Width is not the best approach
        //Adjust for potential widths yielding noninterger binning

        int Nphi = 360 / dphi;
        int Nth = 180 / dth;
        int blockID = 1;

        for (int i = 0; i < Nth; ++i) {
            double th1 = i * dth;
            double th2 = (i + 1) * dth;

            for (int j = 0; j < Nphi; ++j) {
                double phi1 = j * dphi;
                double phi2 = (j + 1) * dphi;

                Block block;
                block.SetBlockGeometry(r1, r2, th1, th2, phi1, phi2, blockID);
                block.DefineBlockMaterial(SubLayerMaterialName, SubLayerA, SubLayerZ, SubLayerDensity);
                block.BuildBlock();
                blocks.push_back(block);
                blockID++;

                if (block.GetVolume() != nullptr) {
                    std::cout << "Block is a valid volume!\n";
                    block.BlockSummary();
                }
            }
        }

        BuildAssembly();
    }

    void SetLayerMaterial(const std::string& name, double A, double Z, double density) {
        SubLayerMaterialName = name;
        SubLayerA = A;
        SubLayerZ = Z;
        SubLayerDensity = density;
    }
};

class Layer {

    private: TGeoVolumeAssembly * layer = nullptr;
    std::vector < Sublayer > sublayers;
    int layerID = 1;
    std::string LayerTitle = "EarthLayer";
    std::string LayerName = Form("%s_%d", LayerTitle.c_str(), layerID);
    double dphi = 90.0;
    double dth = 60.0;

    public:
        // == Constrcutor ==

        //Layer() = default;
        Layer(std::string name = "DefaultName", int id = 1, double dthsize = 60.0, double dphisize = 90.0): LayerTitle(name),
    layerID(id),
    dphi(dphisize),
    dth(dthsize) {}

    // == Assemble Layer ==
    void SetSubLayers(const std::vector < SublayerConfig > & configs) {
        layer = new TGeoVolumeAssembly(LayerName.c_str());

        for (const auto & cfg: configs) {
            Sublayer sub;
            sub.SetSubLayerID(cfg.layerID);
            sub.SetSubLayerMaterial(cfg.materialName, cfg.atomicMass, cfg.atomicNumber, cfg.density);
            sub.CreateSubLayerBlocks(cfg.rmin, cfg.rmax, dphi, dth);
            sublayers.push_back(sub);

        }
    }
    void BuildLayer() {
        int id = 1;
        for (auto & sl: sublayers) {
            sl.BuildSubLayer();
            layer -> AddNode(sl.GetSubLayerVolume(), id++);

        }
    }
    // == Modify layer ==

    // Modify blocks in a single sublayer 
    void UpdateBlocksInSubLayer(int sublayerIndex,
        const std::vector < int > & blockIDs,
            const std::string & newName,
                const std::vector < double > & newA,
                    const std::vector < double > & newZ,
                        const std::vector < double > & newDensity) {
        if (sublayerIndex < 1 || sublayerIndex > sublayers.size()) {
            std::cerr << " Sublayer index " << sublayerIndex << " is out of bounds! Skipping...\n";
            return;
        }

        if (blockIDs.size() != newA.size() ||
            blockIDs.size() != newZ.size() ||
            blockIDs.size() != newDensity.size()) {
            std::cerr << " Inconsistent sizes in block update vectors.\n";
            return;
        }

        Sublayer & sl = sublayers[sublayerIndex - 1];
        bool anyModified = false;

        for (size_t i = 0; i < blockIDs.size(); ++i) {
            sl.UpdateBlockMaterial(blockIDs[i], newName, newA[i], newZ[i], newDensity[i]);
            std::cout << " Block " << blockIDs[i] <<
                " in Sublayer " << sublayerIndex <<
                " updated with material: " << newName << "\n";
            anyModified = true;
        }

        if (!anyModified) {
            std::cerr << " No valid blocks were modified.\n";
        } else {
            sl.BuildSubLayer(); // Rebuild this specific sublayer's geometry
        }
    }
    // Modify full sublayer
    // Modify full sublayer
    void UpdateSublayer(int sublayerIndex,
        const std::string & newName,
            const double newA,
                const double newZ,
                    const double newDensity) {
        if (sublayerIndex < 1 || sublayerIndex > sublayers.size()) {
            std::cerr << "Invalid sublayer index: " << sublayerIndex << "\n";
            return;
        }

        Sublayer & sl = sublayers[sublayerIndex - 1];
        sl.UpdateAllBlocksMaterial(newName, newA, newZ, newDensity); // <<<< FIXED: removed .c_str() and added ;
        sl.BuildSubLayer();
    }

    // == Sublayer summary ==
    //void LayerSummary(){}

    void SubLayerSummary(int SubLayerIndex) {
        Sublayer sl = sublayers[sublayerIndex - 1];
        sl.GetAllBlockSummary();
        delete sl;
    }
    void BlockSummary(int SubLayerIndex, int BlockIndex) {
        Sublayer sl = sublayers[sublayerIndex - 1];
        sl.GetBlockSummary(BlockIndex);
        delete sl;
    }

    //== Return Volume ==
    TGeoVolumeAssembly * GetLayerVolume() const {
        return layer;
    }

};
