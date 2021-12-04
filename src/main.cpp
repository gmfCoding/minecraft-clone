#include <iostream>
#include "glad.h"
#include "GL/gl.h"
#include <GLFW/glfw3.h>  
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Mesh.hpp"
#include "Object.hpp"

#include "Rendering.hpp"
#include "gldebug.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


Object* GenerateExample()
{
    Mesh* mesh = new Mesh();
    mesh->vertices.push_back({ 0.5f,  0.5f, 0.0f});
    mesh->indices.push_back(0);
    mesh->vertices.push_back({-0.5f,  0.5f, 0.0f});
    mesh->indices.push_back(1);
    mesh->vertices.push_back({-0.5f, -0.5f, 0.0f});
    mesh->indices.push_back(2);
    Object* object = new Object();
    object->renderer = new MeshRenderer("default");
    object->renderer->Bind(mesh);
    *(object->PtrPosition()) = {0.0f, 0.0f, 0.0f};
    return object;
}

void GLAPIENTRY MessageCallback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam )
{
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ), type, severity, message );
}


int main() {

    glm::vec3 cameraPos = glm::vec3(0.0f,0.0f,3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    glm::vec3 view;


    glm::mat4 test = glm::mat4(1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f,9.0f,10.0f,11.0f,12.0f,13.0f,14.0f,15.0f,16.0f);

    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f); 

    Mesh* mesh = new Mesh();

        glm::vec3 vertices[] = {
        glm::vec3(0, 0, 0),
        glm::vec3(1, 0, 0),
        glm::vec3(1, 1, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 1, 1),
        glm::vec3(1, 1, 1),
        glm::vec3(1, 0, 1),
        glm::vec3(0, 0, 1),
    };

    unsigned short indices[] = {
        0, 2, 1, //face front
        0, 3, 2,
        2, 3, 4, //face top
        2, 4, 5,
        1, 2, 5, //face right
        1, 5, 6,
        0, 7, 4, //face left
        0, 4, 3,
        5, 4, 7, //face back
        5, 7, 6,
        0, 6, 7, //face bottom
        0, 1, 6
    };
    
    mesh->vertices = std::vector<glm::vec3>(vertices, vertices + 8);
    mesh->indices = std::vector<unsigned short>(indices, indices + 36);

    const std::string defMat = "default";
    Object* object = new Object();

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_CULL_FACE); 
    glViewport(0, 0, 800, 600);
    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback( MessageCallback, nullptr);

    Renderer::CreateProgram("default", "./shaders/vertex.shader", "./shaders/fragment.shader");


    // Object* object = new Object("default", mesh);
    // object->renderer = new MeshRenderer("default");
    // object->renderer->Bind(mesh);
    // object->position = {0.0f,0.0f,0.0f};

    object->renderer = new MeshRenderer("default");
    object->renderer->Bind(mesh);

    //renderer->Bind(mesh);
    //object->renderer = renderer;
    
    Object* secondary = GenerateExample();
#define MAPX 2
#define MAPY 2
#define MAPZ 2

    Object* map[MAPX * MAPY * MAPZ];

    int m = 0;
    for (size_t x = 0; x < MAPX; x++)
    {
        for (size_t y = 0; y < MAPY; y++)
        {
            for (size_t z = 0; z < MAPZ; z++)
            {
                map[m] = new Object();
                map[m]->renderer = object->renderer;
                *(map[m]->PtrPosition()) = {x/1.0f,y/1.0f,z/1.0f};
                m++;
            }
        }
    }

    
    glm::mat4 mat4id = glm::mat4(1.0);
    *object->PtrTransform() = glm::translate(mat4id, glm::vec3(0.0f,0.0f,0.5f));
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        float rotation = glfwGetTime();

        //object->transform = glm::scale(object->transform, glm::vec3(glfwGetTime()/10.0));
        // for (size_t i = 0; i < MAPX * MAPY* MAPZ; i++)
        // {
        //     Renderer::RenderObject(map[i]);
        // }

        glm::vec3 angles = glm::eulerAngles(glm::quat());
        //angles.x = rotation;
        object->SetRotation(glm::quat(angles));
        Renderer::RenderObject(object);
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    glfwTerminate();
    return 0;
}

