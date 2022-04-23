#include "GizmoLine.hpp"
#include "MaterialSystem.hpp"
#include "Camera.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Rendering.hpp"

GizmoLine::GizmoLine(glm::vec3 start, glm::vec3 end) {
    startPoint = start;
    endPoint = end;
    lineColor = glm::vec3(1,1,1);
    transform = glm::mat4(1.0f);

    shaderProgram = MaterialSystem::materialMap["basic"]->programID;

    vertices = {
            start.x, start.y, start.z,
            end.x, end.y, end.z,

    };
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

int GizmoLine::setMVP(glm::mat4 mvp) {
    this->transform = mvp;
    return 1;
}

int GizmoLine::setColor(glm::vec3 color) {
    this->lineColor = color;
    return 1;
}

int GizmoLine::draw(Camera* camera) {

    glUseProgram(shaderProgram);

    glm::mat4 mvp = camera->projection * camera->view;

    GLuint uniTransform = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(uniTransform, 1, GL_FALSE,  glm::value_ptr(mvp));

    glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, &lineColor[0]);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
    return 1;
}

GizmoLine::~GizmoLine() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}