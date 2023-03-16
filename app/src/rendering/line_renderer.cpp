#include "rendering/line_renderer.hpp"

LineRenderer::LineRenderer(Material pMaterial, bool pDynamic) : material(pMaterial), dynamic{pDynamic}, properties{pMaterial.defaults}
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines.data(), dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

void LineRenderer::Render() 
{
    glUseProgram(material.programID);
    auto camera = Mineclone::instance->camera;
    glm::mat4 mvp = camera->projection * camera->view;

    GLuint uniTransform = glGetUniformLocation(material.programID, "MVP");
    glUniformMatrix4fv(uniTransform, 1, GL_FALSE,  glm::value_ptr(this->transform->transform));

    glm::vec3 colour = properties.GetValueRef<glm::vec3>("color");
    glUniform3fv(glGetUniformLocation(material.programID, "color"), 1, &colour.x);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, (lines.size()/2));
}