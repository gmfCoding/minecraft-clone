#include <iostream>
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
#include "engine.h"
#include "PlayerMove.h"

#include <glm/gtx/string_cast.hpp>

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

class Mineclone : public Engine {

    PlayerMove playerMove{};
    Camera* camera;

    World* world;
    Object* object;

    void Start() override {
        Engine::Start();
        camera = new Camera(70.0f, 4.0f / 3.0f, 0.1f, 100.0f);
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

        Mesh* mesh = GenerateCubeMesh();

        const std::string defMat = "default";
        object = new Object();

        // Object* object = new Object("default", mesh);
        // object->renderer = new MeshRenderer("default");
        // object->renderer->Bind(mesh);
        // object->position = {0.0f,0.0f,0.0f};

        object->renderer = new MeshRenderer("default");
        object->renderer->Bind(mesh);

        //renderer->Bind(mesh);
        //object->renderer = renderer;
        //world = World::LoadWorld("world-imports/waterthing.json", object);
        //world = World::LoadWorld("world-imports/grasswater.json", object);
        world = World::LoadWorld("world-imports/desolateisland.json", object);

        glm::mat4 mat4id = glm::mat4(1.0);
        *object->PtrTransform() = glm::translate(mat4id, glm::vec3(0.0f,0.0f,0.5f));
    }

    void Update() override {
        Input();
        camera->Control(playerMove);
        glClearColor(235/255, 171/255, 87/255, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float rotation = glfwGetTime();

        object->transform = glm::scale(object->transform, glm::vec3(glfwGetTime()/10.0));
        
        world->Render();

        glfwSwapBuffers(window);

        KeyMode* tmp = keyStateCurrent;
        // switch buffers
        keyStateCurrent = keyStatePrevious;
        for (size_t i = 0; i < RECKEY_COUNT; i++)
        {
            if (keyStateCurrent[i] == KeyMode::Press)
            { keyStateCurrent[i] == KeyMode::Hold; continue; }

            keyStateCurrent[i] == KeyMode::None;
        }
        
        
        keyStatePrevious = keyStateCurrent;
        glfwPollEvents();    
    }

    void Input()
    {
        if(onKeyUpdate)
        {
#define KEYCONTROL(keyId, bvar) if(keyStateCurrent[GLFW_KEY_##keyId] == KeyMode::Press || keyStateCurrent[GLFW_KEY_##keyId] == KeyMode::Hold) { bvar = true;} else{bvar = false;}
            KEYCONTROL(A,playerMove.Left)
            KEYCONTROL(S,playerMove.Backwards)
            KEYCONTROL(D,playerMove.Right)
            KEYCONTROL(W,playerMove.Forward)
            KEYCONTROL(Q,playerMove.Down)
            KEYCONTROL(E,playerMove.Up)
        }
    }
};



int main() {
    Mineclone engine = Mineclone();
    return engine.Intialise();
}
