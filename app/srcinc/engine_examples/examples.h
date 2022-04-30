#include <set>
#include <map>
#include <string>

#include "LoadGlad.h"

#include "fileio.hpp"
#include "gmfc_image.hpp"
#include "CommonData.hpp"
#include "TextureManager.hpp"

#define TEXTUREPATH(i) getAssetPathMany({"textures",i})

void TextureAtlasGeneration()
{
    

    std::set<std::string> images;
    std::map<std::string, RectUV> uvMap;
    
    images.insert({TEXTUREPATH("grass_side.png"), TEXTUREPATH("grass.png"), TEXTUREPATH("water.png"),  TEXTUREPATH("sand.png")});

    int pixelsX = 0;
    int pixelsY = 0;
    PixelData* atlas = nullptr;

    TextureManager::CreateAtlasFromFiles(images, pixelsX, pixelsY, atlas, uvMap);
}


void UploadPixelDataToGPU()
{
    int sizeX = 10, sizeY = 10, channels = 4;
    PixelData* pxData = nullptr; // assume we have pixel data
    Image image = Image(pxData, sizeX, sizeY, channels);

    GLuint textureID = TextureManager::UploadNamedTexture(&image, "named_texture_name");
}

void GetNamedGPUTexture()
{
    // Gets the OpenGL texture ID 'address' named "named_texture_name" created in UploadPixelDataToGPU()
    GLuint textureID = TextureManager::GetNamedTexture("named_texture_name");
}

void LoadImage()
{
    Image image;
    image.Load(TEXTUREPATH("grass_side.png"));
    TextureManager::UploadNamedTexture(&image, "grass");
}
