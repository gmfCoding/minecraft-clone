#ifndef BLOCKS_H
#define BLOCKS_H
#include <map>
#include <string>
#include <vector>


class Blocks
{
    public:
    enum FACE
    {
        NONE=0, 		// 0xb000000
        PYT=1,  		// 0xb100000
        NYB=2,  		// 0xb010000
        NZF=4,			// 0xb001000
        PZB=8,			// 0xb000100
        NXL=16,			// 0xb000010
        PXR=32,			// 0xb000001
        SIDES=120,		// 0xb001111
        TOPBOTTOM=3,	// 0xb110000
        ALL=63			// 0xb111111       
    };

    

    class BlockConfig {
        public:
        const std::string name;
        const int col_id;
        //const FACE textures = FACE::NONE;
        std::map<FACE, std::string> faceFiles;
        const std::map<FACE, int> animated;
        std::vector<std::string> textureFiles;
        int textures;

        BlockConfig(const std::string& p_name, int p_col_id, int p_textures, std::initializer_list<std::string> p_textureFiles);
    };
    
    static std::map<int, std::string> blockIDToName;
    static std::map<std::string, int> blockNameToID;
    static std::map<int, Blocks::BlockConfig*> blockIDToConfig;
    static bool blocksInitialised;

    static void InitialiseBlocks();
};

// std::map<int, std::string> Blocks::blockIDToName;
// std::map<std::string, int> Blocks::blockNameToID;
// std::map<int, Blocks::BlockConfig*> Blocks::blockIDToConfig;

#endif