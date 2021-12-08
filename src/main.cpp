#include <iostream>
#include "glad.h"
#include "GL/gl.h"
#include <GLFW/glfw3.h>  
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include "Mesh.hpp"
#include "Object.hpp"
#include "Camera.hpp"
#include "Rendering.hpp"
#include "gldebug.hpp"
#include "World.h"

#include <glm/gtx/string_cast.hpp>

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

Mesh* GenerateCubeMesh()
{
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

    return mesh;
}

struct PlayerMove
{
    bool Left;
    bool Right;
    bool Forward;
    bool Backwards;
    bool Down;
    bool Up;
};

PlayerMove playerMove{};

#define KEYPRESS(keyId) key == GLFW_KEY_##keyId && action = GLFW_PRESS
#define KEYRELEASE(keyId) key == GLFW_KEY_##keyId && action = GLFW_RELEASE
#define KEYCONTROL(keyId, bvar) if(key == GLFW_KEY_##keyId) { if (action == GLFW_PRESS) {bvar = true;}else{bvar = false;}}  \

// class InputAction {
//     int key;

// }

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //KEYCONTROL(A,playerMove.Left)
    if(key == GLFW_KEY_A)
    { 
        if (action == GLFW_PRESS) 
        {playerMove.Left = true;}
        else{playerMove.Left = false;}
    }
    KEYCONTROL(S,playerMove.Backwards)
    KEYCONTROL(D,playerMove.Right)
    KEYCONTROL(W,playerMove.Forward)
    KEYCONTROL(Q,playerMove.Down)
    KEYCONTROL(E,playerMove.Up)
}

void ControlCamera(PlayerMove move, Camera* camera)
{
    glm::vec3* pos = camera->PtrPosition();

    glm::vec3 cameraDirection = glm::normalize(*pos - glm::vec3(0.0f));
    glm::vec3 cameraLeft = -glm::normalize(glm::cross(glm::vec3(0,1,0), cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, -cameraLeft);

    if(move.Left) {
        *pos = *pos + cameraLeft;}
    if(move.Right) {*pos = *pos - cameraLeft;}
    if(move.Forward) {*pos = *pos + cameraDirection;}
    if(move.Backwards) {*pos = *pos - cameraDirection;}

    if(move.Up) {*pos = *pos + cameraUp;}
    if(move.Down) {*pos = *pos - cameraUp;}

    camera->UpdateView();
}

int main() {
    Camera* camera = new Camera(70.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    Renderer::camera = camera;
    camera->SetPosition(glm::vec3(6.0f,3.5f,7.0f)*2.0f);
    camera->UpdateView();

    glm::vec3 cameraPos = glm::vec3(6.0f,3.5f,7.0f)*2.0f;
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    // glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    // glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    // glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    glm::mat4 cameraMatrix = glm::lookAt(
        cameraPos, // the position of your camera, in world space
        cameraTarget,   // where you want to look at, in world space
        up        // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
    );

    std::cout << glm::to_string(camera->view) << std::endl;

    std::cout << glm::to_string(cameraMatrix) << std::endl;

    Mesh* mesh = GenerateCubeMesh();

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

    glfwSetKeyCallback(window, key_callback);

    glEnable(GL_DEPTH_TEST);  
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

    //World* notWorld = World::LoadWorld("world-imports/waterthing.json", object);
    World* notWorld = World::LoadWorld("world-imports/grasswater.json", object);

    glm::mat4 mat4id = glm::mat4(1.0);
    *object->PtrTransform() = glm::translate(mat4id, glm::vec3(0.0f,0.0f,0.5f));
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    while(!glfwWindowShouldClose(window)) {

        ControlCamera(playerMove, camera);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float rotation = glfwGetTime();

        object->transform = glm::scale(object->transform, glm::vec3(glfwGetTime()/10.0));
        
        notWorld->Render();

        // for (size_t x = 0; x < MAPX; x++)
        // {
        //     for (size_t y = 0; y < MAPY; y++)
        //     {
        //         for (size_t z = 0; z < MAPZ; z++)
        //         {
        //             Renderer::RenderObject(map[x][y][z]);
        //         }
        //     }
        // }
        
        //Renderer::RenderObject(object);
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    glfwTerminate();
    return 0;
}

