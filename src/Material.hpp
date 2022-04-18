#pragma once
#include <string>
#include <map>
#include <vector>
#include <glm/vec2.hpp>
#include "LoadGlad.h"

class Material {

public:
    std::string materialName;
    GLuint programID;

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

class VertexMaterial : public VertexFragmentCombinationMaterial {
public:
    std::vector<int32_t> colours;
    std::vector<int32_t> voxelColours;
    std::map<int32_t, size_t> colourIndexMap;
    
    VertexMaterial(std::vector<int32_t> colours);

    std::vector<glm::vec2> GenerateUVs(int32_t colour);
};