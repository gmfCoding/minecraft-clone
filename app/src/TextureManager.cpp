#include "TextureManager.hpp"
#include "fileio.hpp"
#include <glm.hpp>
#include <iostream>

std::map<std::string, Image*> TextureManager::images;
std::map<std::string, int> TextureManager::imageToGpuID;

// TODO: 
// Does loading G:\Example\resources\textures\x.png and loading resources\textures\x.png count as two seperate images within our maps?
std::tuple<unsigned int, Image*> TextureManager::LoadTextureGPU(const std::string& path)
{
    if(images.count(path) != 1)
    {
        images[path] = new Image();
        
        Image* image = images[path];
        image->Load(path);


        GLuint texture = UploadNamedTexture(image, path);

        return std::tuple<unsigned int, Image*>(texture, image);
    }
    else
    {
        return std::tuple<unsigned int, Image*>(imageToGpuID[path], images[path]);
    }
}

GLuint TextureManager::UploadTexture(Image* image)
{
    unsigned int texture;
    glGenTextures(1, &texture);  
    glBindTexture(GL_TEXTURE_2D, texture);

    GLenum format = GL_RED;

    switch (image->channels)
    {
        case 1:
            format = GL_RG;
            break;
        case 2:
            format = GL_RG;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->c_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    return texture;
}


GLuint TextureManager::GetNamedTexture(const std::string& name)
{
    return imageToGpuID[name];
}

GLuint TextureManager::UploadNamedTexture(Image* image, const std::string& name)
{
    GLuint texID = UploadTexture(image);
    MapGPUTexture(texID, name);
    return texID;
}

void TextureManager::MapGPUTexture(GLuint textureID, const std::string& name)
{
    imageToGpuID[name] = textureID;
}

void TextureManager::CreateAtlasFromFiles(std::set<std::string> files, int &pixelsX, int &pixelsY, PixelData* &pixels, std::map<std::string, RectUV> &uvTrackMap)
{    
    int size = 0;

    bool initialised = false;
    
    std::vector<Image*> loadedImages;
    std::vector<std::string> filePaths;
    for (auto &&i : files)
    {
        Image* image = new Image();
        image->Load(getAssetPathMany({ "textures", i+".png"}));
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

    pixelsX = sizeA;
    pixelsY = sizeA;
    // Store it the atlas pixels some where
    pixels = (PixelData*) calloc(pixelsX * pixelsY, sizeof(PixelData));

    for (size_t j = 0; j < length; j++)
    {
        for (size_t k = 0; k < length; k++)
        {
            int index = j + length * k;
            if(index >= loadedImages.size())
                continue;

            const PixelData* data = loadedImages[index]->c_data;

            std::string fp = filePaths[index];
            // p_topRight, glm::vec2 p_topLeft, glm::vec2 p_bottomRight, glm::vec2 p_bottomLeft

            // if there was only one image in the atlas data would look like this
            // glm::vec2(0.0, 0.0), glm::vec2(0.0, 1.0), glm::vec2(1.0, 1.0), glm::vec2(1.0, 0.0)

            uvTrackMap.insert({fp, RectUV(glm::vec2(size * float(j),size * float(k)), glm::vec2(size * j,     size * float(k + 1)), glm::vec2(size * (j+1), size * float(k + 1)), glm::vec2(size * float(j+1), size * k), float(sizeA))});

            for (size_t sY = 0; sY < size; sY++)
            {
                for (size_t sX = 0; sX < size; sX++)
                {
                   pixels[(j*size + sX) + (sizeA * (k*size+sY))] = data[sX + size * sY];
                }
            }
        }
    }

    for (auto i : loadedImages)
    {
        delete i;
    }

    loadedImages.clear();
}