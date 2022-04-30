#include "Material.hpp"
#include "MaterialSystem.hpp"
#include <map>
#include <any>
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

bool MaterialProperties::HasValue(const std::string& name)
{
    std::map<std::string, std::any>::iterator it = this->properties.find(name);
    return it != this->properties.end();
}