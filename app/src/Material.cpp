#include "Material.hpp"
#include "MaterialSystem.hpp"

#include "LoadGlad.h"

void Material::Setup() {

}

void Material::Bind() {
    glUseProgram(this->programID);
}


Material::Material(const std::string _materialName) : materialName(_materialName)
{
    MaterialSystem::AddMaterial(this);
}

VertexFragmentCombinationMaterial::VertexFragmentCombinationMaterial(const std::string _materialName, std::string _vertexPath, std::string _fragmentPath) : Material(_materialName), vertexPath(_vertexPath), fragmentPath(_fragmentPath)
{
    programID = MaterialSystem::CreateVFProgram(vertexPath, fragmentPath);
}

std::vector<glm::vec2> VertexMaterial::GenerateUVs(int32_t colour)
{
    size_t index = this->colourIndexMap[colour];
    float fidx = (float)index;
    std::vector<glm::vec2> coords = std::vector<glm::vec2>{
        glm::vec2(fidx - 1.0f, 0.f),
        glm::vec2(fidx - 1.0f, 1.0f),
        glm::vec2(fidx, 0.0f),
        glm::vec2(fidx,1.0f)
    };

    return coords;
}

VertexMaterial::VertexMaterial(std::vector<int32_t> _colours) : VertexFragmentCombinationMaterial("VoxelMat", "./shaders/world_vert.shader", "./shaders/world_frag.shader")
{
    
}

