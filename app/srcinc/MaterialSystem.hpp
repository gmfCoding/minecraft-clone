#pragma once
#include <map>
#include <string>
#include <GLFW/glfw3.h>

#include "Material.hpp"


class MaterialSystem {
    public:

    static std::map<std::string, Material*> materialMap;

    static void AddMaterial(Material* material);

    static GLuint CreateVFProgram(const std::string& vertexPath, const std::string&  fragmentPath);
    static void DebugShaderInfo(GLuint shader);
    static void DebugProgramInfo(GLuint program);
};