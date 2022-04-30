#pragma once

#include <string>
#include <map>
#include <any>

#include "LoadGlad.h"

class MaterialProperties
{
    public:
    std::map<std::string, std::any> properties;

    public:
    bool HasValue(const std::string& name);

    template<typename T>
    T GetValue(const std::string& name)
    {
        std::map<std::string, std::any>::iterator it = this->properties.find(name);
        std::any result;
        if(it != this->properties.end())
        {
        result = it->second;
        }

        return std::any_cast<T>(result);
    }

    template<typename T>
    T& GetValueRef(const std::string& name)
    {
        std::map<std::string, std::any>::iterator it = this->properties.find(name);
        if(it == this->properties.end())
        {
            T* val = nullptr;
            return *val;
        }

        return std::any_cast<T&>(it->second);
    }

};


class Material {

public:
    std::string materialName;
    GLuint programID;

    MaterialProperties defaults;
    

    Material(const std::string _materialName);

    virtual void Setup();
    virtual void Bind();
};

class VertexFragmentCombinationMaterial : public Material {
    public:
    std::string vertexPath;
    std::string fragmentPath;

    VertexFragmentCombinationMaterial(std::string _materialName, std::string _vertexPath, std::string _fragmentPath);
};