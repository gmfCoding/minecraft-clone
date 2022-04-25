#ifndef IMAGE_H_INCLUDE
#define IMAGE_H_INCLUDE

#include <string>

class Image
{
    int sizeX, sizeY, channelCount;
    unsigned char *data;

public:
    Image();
    void Load(const std::string &path);
    const unsigned char* GetPixelAt(int x, int y);

    const int& width = sizeX;
    const int& height = sizeY;
    const int& channels = channelCount;
    const unsigned char *c_data = data;
    
    ~Image();
};

#endif