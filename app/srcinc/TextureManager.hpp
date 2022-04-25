#ifndef TEXTUREMANAGER_INCLUDE_H
#define TEXTUREMANAGER_INCLUDE_H

#include "LoadGlad.h"
#include "gmfc_image.hpp"
#include <map>

class TextureManager
{

    static std::map<std::string, Image*> images;
    static std::map<std::string, int> imageToGpuID;
    public:
    static std::tuple<unsigned int, Image*> LoadTextureGPU(std::string path);
};
#endif