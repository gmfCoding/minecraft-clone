#ifndef IMAGE_H_INCLUDE
#define IMAGE_H_INCLUDE

#include <string>

struct PixelData
{
    unsigned char r, g, b, a;
    PixelData(unsigned char pr, unsigned char pg, unsigned char pb, unsigned char pa);
    PixelData();
};


class Image
{
    std::string mPath;
    int sizeX, sizeY, channelCount;
    PixelData* data;

public:
    Image();
    Image(PixelData* pPixels, int pSizeX, int pSizeY, int pChannels);
    void Load(const std::string &path);
    const PixelData* GetPixelAt(int x, int y);

    std::string& Path = mPath;
    const int& width = sizeX;
    const int& height = sizeY;
    int channels = channelCount;
    const PixelData* c_data = data;
    
    ~Image();
};

#endif