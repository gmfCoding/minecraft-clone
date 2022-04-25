#include "TextureManager.hpp"


std::map<std::string, Image*> TextureManager::images;
std::map<std::string, int> TextureManager::imageToGpuID;


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