#ifndef BLOCKS_H
#define BLOCKS_H
#include <map>
#include <string>
#include <vector>
#include <set>
#include "CommonData.hpp"

using namespace block;

class Blocks
{    
public:
    class BlockConfig {
        public:
        const std::string name;
        const int col_id;
        //const FACE textures = FACE::NONE;
        std::map<FACE, std::string>* faceFiles;
        const std::map<FACE, int> animated;
        std::vector<std::string> textureFiles;
        int textures;

        BlockConfig(const std::string& p_name, int p_col_id, int p_textures, std::vector<std::string> p_textureFiles);
    };
    
    static std::map<int, std::string> blockIDToName;
    static std::map<std::string, int> blockNameToID;
    static std::map<int, Blocks::BlockConfig*> blockIDToConfig;
    static std::map<std::string, RectUV>* worldTextureUVMap;
    static bool blocksInitialised;

    static void InitialiseBlocks();

    static std::set<std::string> GetTextureNames();

    static RectUV GetUVForFace(FACE face, int blockID);
};

// std::map<int, std::string> Blocks::blockIDToName;
// std::map<std::string, int> Blocks::blockNameToID;
// std::map<int, Blocks::BlockConfig*> Blocks::blockIDToConfig;

#endif