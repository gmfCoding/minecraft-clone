#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "rendering/line_renderer.hpp"
#include "Mineclone.hpp"
#include "MaterialSystem.hpp"
#include "Material.hpp"
#include "Camera.hpp"

LineRenderer::LineRenderer(Material* pMaterial, bool pDynamic) : material(pMaterial), dynamic{pDynamic}, properties{pMaterial->defaults}, transform{new Transform()}
{

}


LineRenderer::LineRenderer(bool pDynamic) : LineRenderer::LineRenderer(MaterialSystem::materialMap["basic"], pDynamic) 
{

}

void LineRenderer::Bind()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
    Upload();
}

void LineRenderer::Upload()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * lines.size(), lines.data(), dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

void LineRenderer::Render() 
{
    glUseProgram(material->programID);
    auto camera = Mineclone::instance->camera;
	glm::mat4 mvp = camera->projection * camera->view * this->transform->transform;
    GLuint uniTransform = glGetUniformLocation(material->programID, "MVP");
    glUniformMatrix4fv(uniTransform, 1, GL_FALSE,  glm::value_ptr(mvp));

    glm::vec3 colour = properties.GetValueRef<glm::vec3>("color");
    glUniform3fv(glGetUniformLocation(material->programID, "color"), 1, &colour.x);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, (lines.size()));
}