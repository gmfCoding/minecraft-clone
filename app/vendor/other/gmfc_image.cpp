#include "gmfc_image.hpp"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image() : sizeX(0), sizeY(0), channels(0)
{

}

void Image::Load(const std::string &path)
{
    mPath = path;
    data = (PixelData*)stbi_load(path.c_str(), &sizeX, &sizeY, &channelCount, 4);
    channels = channelCount;
    c_data = data;
}

const PixelData* Image::GetPixelAt(int x, int y)
{
    return &data[y * sizeX + x];
}

Image::~Image()
{
    std::cout << "Deleteing Image: " << mPath;
    std::cout << "At: " << this << std::endl;
    stbi_image_free(data);
}