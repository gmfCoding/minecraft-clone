#include "TextureManager.hpp"
#include "fileio.hpp"
#include <glm.hpp>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <iostream>

std::map<std::string, Image*> TextureManager::images;
std::map<std::string, int> TextureManager::imageToGpuID;

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

// TODO: 
// Does loading G:\Example\resources\textures\x.png and loading resources\textures\x.png count as two seperate images within our maps?
std::tuple<unsigned int, Image*> TextureManager::LoadTextureGPU(std::string path)
{
    if(images.count(path) != 1)
    {
        images[path] = new Image();
        Image* image = images[path];
        image->Load(path);

        unsigned int texture;
        glGenTextures(1, &texture);  
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->c_data);
        glGenerateMipmap(GL_TEXTURE_2D);

        return std::tuple<unsigned int, Image*>(texture, image);
    }
    else{
        return std::tuple<unsigned int, Image*>(imageToGpuID[path], images[path]);
    }
}




void TextureManager::CreateAtlasFromFiles(std::set<std::string> files)
{    
    int size = 0;

    bool initialised = false;
    
    std::vector<Image*> loadedImages;
    std::vector<std::string> filePaths;
    for (auto &&i : files)
    {
        Image* image = new Image();
        image->Load(getAssetPathMany({ "textures", i+".png"}));
        std::cout << "Loaded Image:" << image->Path << " | " << image->width << "x" << image->height << "@" << image->channels << std::endl;
        if(!initialised)
        {
            size = image->width;
            initialised = true;
        }
        filePaths.push_back(i);
        loadedImages.push_back(image);
    }

    // Calculate the minumum amount of horizontal/vertical cells (a cell being a "slot" for an image) 
    int length = ceil(sqrt(loadedImages.size()));

    // Calculate the size of the atlas
    int sizeA = length * size;

    // Store it the atlas pixels some where
    std::vector<PixelData> pixels(sizeA * sizeA);

    // Keep a record of where the uvs for certain textures will be
    std::map<std::string, RectUV> uvTrackMap;

    for (size_t j = 0; j < length; j++)
    {
        for (size_t k = 0; k < length; k++)
        {
            int index = j + length * k;
            if(index >= loadedImages.size())
                continue;

            const PixelData* data = loadedImages[index]->c_data;

            std::string fp = filePaths[index];
            uvTrackMap.insert({fp, RectUV(glm::vec2(size * j,     size * k), glm::vec2(size * j,     size * (k + 1)), glm::vec2(size * (j+1), size * (k + 1)), glm::vec2(size * (j+1), size * k), size)});
            std::cout << "Stitching: " << fp << " to atlas @ cell:" << j << "," << k << "(" << (j*size) << "," << (k*size) << "," << (j*size) + size * (k*size) << ")" << std::endl;
            for (size_t sY = 0; sY < size; sY++)
            {
                for (size_t sX = 0; sX < size; sX++)
                {
                   pixels[(j*size + sX) + (sizeA * (k*size+sY))] = data[sX + size * sY];
                }
            }
        }
    }


    std::cout << "Creating Image:" << sizeA << "x" << sizeA << "@" << 4 << std::endl;
    stbi_write_png("stbpng.png", sizeA, sizeA, 4, (&pixels[0].r), sizeA * 4);
}