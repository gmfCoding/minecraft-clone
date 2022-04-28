
#include "Blocks.hpp"


std::map<std::string, RectUV>* Blocks::worldTextureUVMap = nullptr;

bool Blocks::blocksInitialised = false;
std::map<int, std::string> Blocks::blockIDToName = std::map<int, std::string> ();
std::map<std::string, int> Blocks::blockNameToID = std::map<std::string, int> ();
std::map<int, Blocks::BlockConfig *> Blocks::blockIDToConfig = std::map<int, Blocks::BlockConfig *> ();

Blocks::BlockConfig::BlockConfig(const std::string& p_name, int p_col_id, int p_textures, std::vector<std::string> p_textureFiles)  : name(p_name), col_id(p_col_id), textures(p_textures), textureFiles(p_textureFiles)
{   
    int top = p_textures >> (4*5) & 0xF;
    int bottom = p_textures >> (4*4)   & 0xF;
    int left = p_textures >> (4*3) & 0xF;
    int right = p_textures >> (4*2) & 0xF;
    int forward = p_textures >> (4*1)& 0xF;
    int backward = p_textures & 0xF;
    faceFiles = new std::map<FACE, std::string>();
    (*faceFiles)[FACE::RPX] = p_textureFiles[right];
    (*faceFiles)[FACE::TPY] = p_textureFiles[top];
    (*faceFiles)[FACE::BPZ] = p_textureFiles[backward];
    (*faceFiles)[FACE::LNX] = p_textureFiles[left];
    (*faceFiles)[FACE::BNY] = p_textureFiles[bottom];
    (*faceFiles)[FACE::FNZ] = p_textureFiles[forward];
}

void Blocks::InitialiseBlocks()
{
    if (blocksInitialised == false)
    {
        blocksInitialised = true;

        std::vector<BlockConfig>* blocks = new std::vector<BlockConfig>();
        blocks->push_back(BlockConfig("grass", 0x169E26, 0x120000, {"grass_side","grass_top","dirt"}));
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

RectUV Blocks::GetUVForFace(FACE face, int blockID)
{
    std::string texture = (*blockIDToConfig[blockID]->faceFiles)[face];

    return (*worldTextureUVMap)[texture];
}

std::set<std::string> Blocks::GetTextureNames()
{
    std::set<std::string> filesPaths;

    for (auto &&i : blockIDToConfig)
    {
        for (auto &&t : i.second->textureFiles)
        {
            filesPaths.insert(t);
        }
    }

    return filesPaths;
}