#ifndef TEXTUREMANAGER_INCLUDE_H
#define TEXTUREMANAGER_INCLUDE_H

#include "LoadGlad.h"
#include "gmfc_image.hpp"
#include <map>
#include <set>
#include <vector>
#include <glm.hpp>

class RectUV
{
    public:
    glm::vec2 topRight;
    glm::vec2 topLeft;
    glm::vec2 bottomRight;
    glm::vec2 bottomLeft;
    
    RectUV (glm::vec2 p_topRight, glm::vec2 p_topLeft, glm::vec2 p_bottomRight, glm::vec2 p_bottomLeft, float size)
    {
        topRight = glm::vec2(p_topRight/size);
        topLeft = glm::vec2(p_topLeft/size);
        bottomRight = glm::vec2(p_bottomRight/size);
        bottomLeft = glm::vec2(p_bottomLeft/size);
    }
};


class TextureManager
{

    static std::map<std::string, Image*> images;
    static std::map<std::string, int> imageToGpuID;
    public:
    static std::tuple<unsigned int, Image*> LoadTextureGPU(std::string path);

    /// Creates a texture atlas (one large texture) from multiple input texture file
    /// Each input texture must be square and and they all must have the same resolution
    
    static void CreateAtlasFromFiles(std::set<std::string> files, int &pixelsX,int &pixelsY, PixelData* &pixels, std::map<std::string, RectUV> &uvTrackMap);
};
#endif