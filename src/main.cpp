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

int main() {

    std::cout << "Am I working?" << std::endl;
    Camera* camera = new Camera();
    Renderer::camera = camera;
    glm::vec3 cameraPos = glm::vec3(6.0f,3.5f,7.0f);
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

    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(70.0f), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
        4.0f / 3.0f,       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
        0.1f,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        100.0f             // Far clipping plane. Keep as little as possible.
    );

    camera->SetPosition(cameraPos);
    camera->projection = projectionMatrix;
    camera->view = cameraMatrix;


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
#define MAPX 6
#define MAPY 2
#define MAPZ 5

// #define INDEX(x,y,z) x + MAPY * (y + MAPZ * z)
//     Object* map[MAPX][MAPY][MAPZ];
// #define PLACE(x,y,z,b) map[x][y][z]->colour = b * (90.1f + (rand() % 11)) / 100.0f; tmppos = glm::vec3(x, y, z); map[x][y][z]->SetPosition(tmppos);

//     int m = 0;
//     for (size_t x = 0; x < MAPX; x++)
//     {
//         for (size_t y = 0; y < MAPY; y++)
//         {
//             for (size_t z = 0; z < MAPZ; z++)
//             {
//                 map[x][y][z] = new Object();
//                 map[x][y][z]->renderer = object->renderer;
//                 m++;
//             }
//         }
//     }

    World* notWorld = World::LoadWorld("world-imports/waterthing.json", object);

    World* world = new World(MAPX, MAPY, MAPZ, object);

    auto sand = glm::vec4(241, 228, 175, 1.0) * (1.0F/255.0F);
    auto water = glm::vec4(25, 58, 145, 1.0) * (1.0F/255.0F);
    auto grass = glm::vec4(22, 158, 38, 1.0) * (1.0F/255.0F);
    
    glm::vec3 tmppos(0,0,0);
    //Grass
    world->Place(0,0,0, grass);
    world->Place(1,0,0, grass);
    world->Place(2,0,0, grass);
    world->Place(3,0,0, grass);
 
    world->Place(0,0,1, grass);
    world->Place(1,0,1, grass);
    world->Place(2,0,1, grass);
    world->Place(0,0,2, grass);
    world->Place(1,0,2, grass);
 
    world->Place(0,1,0, grass);
    world->Place(1,1,0, grass);
    world->Place(2,1,0, grass);
    world->Place(0,1,1, grass);
    world->Place(1,1,1, grass);
 
    world->Place(4,0,0, sand);
    world->Place(3,0,1, sand);
    world->Place(2,0,2, sand);
    world->Place(3,0,2, sand);
    world->Place(1,0,3, sand);
    world->Place(0,0,3, sand);
 
    world->Place(5,0,0, water);
    world->Place(5,0,1, water);
    world->Place(4,0,1, water);
    world->Place(5,0,2, water);
    world->Place(5,0,2, water);
    world->Place(4,0,2, water);
    world->Place(5,0,3, water);
    world->Place(4,0,3, water);
    world->Place(3,0,3, water);
    world->Place(2,0,3, water);
    world->Place(5,0,4, water);
    world->Place(4,0,4, water);
    world->Place(3,0,4, water);
    world->Place(2,0,4, water);
    world->Place(1,0,4, water);
    world->Place(0,0,4, water);
    
    glm::mat4 mat4id = glm::mat4(1.0);
    *object->PtrTransform() = glm::translate(mat4id, glm::vec3(0.0f,0.0f,0.5f));
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    while(!glfwWindowShouldClose(window)) {
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

