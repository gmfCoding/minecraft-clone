#include "gmfc_image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image() : sizeX(0), sizeY(0), channels(0)
{

}

void Image::Load(const std::string &path)
{
    data = stbi_load(path.c_str(), &sizeX, &sizeY, &channelCount, 0);
    c_data = data;
}

const unsigned char* Image::GetPixelAt(int x, int y)
{
    return &data[y * sizeX + x];
}

Image::~Image()
{
    stbi_image_free(data);
}