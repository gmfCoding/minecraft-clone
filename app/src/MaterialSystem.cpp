#include "LoadGlad.h"
#include <map>
#include "MaterialSystem.hpp"
#include "fileio.hpp"
#include "gldebug.hpp"
#include <vector>

std::map<std::string, Material*> MaterialSystem::materialMap = std::map<std::string, Material*>();
void MaterialSystem::AddMaterial(Material* material)
{
    materialMap[material->materialName] = material;
}

GLuint MaterialSystem::CreateVFProgram(const std::string& vertexPath, const std::string& fragmentPath)
{
    GLCall(GLuint vertShader = glCreateShader(GL_VERTEX_SHADER));
    GLCall(GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER));

    std::string vertShaderStr = readFile(vertexPath);
    std::string fragShaderStr = readFile(fragmentPath);
    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();

    GLCall(glShaderSource(vertShader, 1, &vertShaderSrc, NULL));
    GLCall(glCompileShader(vertShader));
    DebugShaderInfo(vertShader);

    GLCall(glShaderSource(fragShader, 1, &fragShaderSrc, NULL));
    GLCall(glCompileShader(fragShader));
    DebugShaderInfo(fragShader);

    GLCall(GLuint program = glCreateProgram());
    GLCall(glAttachShader(program, vertShader));
    GLCall(glAttachShader(program, fragShader));
    GLCall(glLinkProgram(program));

    DebugProgramInfo(program);


    GLCall(glDeleteShader(vertShader));
    GLCall(glDeleteShader(fragShader));

    return program;
}

void MaterialSystem::DebugShaderInfo(GLuint shader)
{
    GLint result = GL_FALSE;
    int logLength;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> shaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(shader, logLength, NULL, &shaderError[0]);
}

void MaterialSystem::DebugProgramInfo(GLuint program)
{
    GLint result = GL_FALSE;
    int logLength;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> programError( (logLength > 1) ? logLength : 1 );
    glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
}