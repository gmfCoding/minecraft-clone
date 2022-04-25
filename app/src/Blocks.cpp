
#include "Blocks.hpp"
#include <iostream>

bool Blocks::blocksInitialised = false;
std::map<int, std::string> Blocks::blockIDToName = std::map<int, std::string> ();
std::map<std::string, int> Blocks::blockNameToID = std::map<std::string, int> ();
std::map<int, Blocks::BlockConfig *> Blocks::blockIDToConfig = std::map<int, Blocks::BlockConfig *> ();

Blocks::BlockConfig::BlockConfig(const std::string& p_name, int p_col_id, int p_textures, std::initializer_list<std::string> p_textureFiles) : name(p_name), col_id(p_col_id), textures(p_textures), textureFiles(p_textureFiles)
{   

}


void Blocks::InitialiseBlocks()
{
    if (blocksInitialised == false)
    {
        blocksInitialised = true;

        std::vector<BlockConfig>* blocks = new std::vector<BlockConfig>();
        blocks->push_back(BlockConfig("grass", 0x169E26, 0x120000, {"grass_sides","grass_top","dirt"}));
        blocks->push_back(BlockConfig("sand", 0xF7F3CE, 0x000000, {"sand"}));
        blocks->push_back(BlockConfig("water", 0x193A91, 0x100000, {"water", "water"}));
        blocks->push_back(BlockConfig("log", 0x73322E, 0x110000, {"log", "log_top"}));
        
        for (auto &i : *blocks)
        {
            blockIDToName[i.col_id] = i.name;
            blockNameToID[i.name] = i.col_id;
            blockIDToConfig[i.col_id] = &i;
        }
    }
}

