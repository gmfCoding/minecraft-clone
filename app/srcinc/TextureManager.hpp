#ifndef TEXTUREMANAGER_INCLUDE_H
#define TEXTUREMANAGER_INCLUDE_H

#include "LoadGlad.h"
#include "gmfc_image.hpp"
#include <map>
#include <set>
#include <vector>
class TextureManager
{

    static std::map<std::string, Image*> images;
    static std::map<std::string, int> imageToGpuID;
    public:
    static std::tuple<unsigned int, Image*> LoadTextureGPU(std::string path);

    /// Creates a texture atlas (one large texture) from multiple input texture file
    /// Each input texture must be square and and they all must have the same resolution
    static void CreateAtlasFromFiles(std::set<std::string> files);
};
#endif