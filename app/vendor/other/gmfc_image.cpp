#include "gmfc_image.hpp"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image() : sizeX(0), sizeY(0), channels(0)
{

}

Image::Image(PixelData* pPixels, int pSizeX, int pSizeY, int pChannels) : sizeX(pSizeX), sizeY(pSizeY), channels(pChannels)
{
    this->data = new PixelData[pSizeX * pSizeY];
    PixelData none = PixelData();

    for (size_t i = 0; i < pSizeX * pSizeY; i++)
    {
        
        this->data[i] = none;

        if(pChannels > 0)
            this->data[i].r = pPixels[i].r;
        if(pChannels > 1)
            this->data[i].g = pPixels[i].g;
        if(pChannels > 2)
            this->data[i].b = pPixels[i].b;
        if(pChannels > 3)
            this->data[i].a = pPixels[i].a;
    }

    c_data = data;
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
    stbi_image_free(data);
}

PixelData::PixelData() : r(0), g(0), b(0), a(0)
{

}

PixelData::PixelData(unsigned char pr, unsigned char pg, unsigned char pb, unsigned char pa) : r(pr), g(pg), b(pb), a(pa)
{
    
}