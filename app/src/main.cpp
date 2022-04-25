
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <vector>
#include <set>

#include <stb_image.h>

#include "Input.hpp"
#include "Mesh.hpp"
#include "Object.hpp"
#include "Camera.hpp"
#include "Rendering.hpp"
#include "gldebug.hpp"
#include "World.hpp"
#include "engine.hpp"
#include "PlayerMove.hpp"
#include "PlayerController.hpp"
#include "MaterialSystem.hpp"
#include "Material.hpp"
#include "Blocks.hpp"
#include "AtulPwd.hpp"
#include "gmfc_image.hpp"
#include "TextureManager.hpp"
#include "ImageExample.hpp"

#include "GizmoLine.hpp"

class Mineclone : public Engine {

    PlayerMove playerMove{};
    Camera* camera;
    World* world;

    PlayerController* playerController;

    GizmoLine* line1;
    GizmoLine* line2;
    GizmoLine* line3;

    Image image;

    void Start() override {
        Engine::Start();
        targetFPS = 144.0;

        LoadMaterials();

        camera = new Camera(70.0f, 4.0f / 3.0f, 0.1f, 100.0f);
        Renderer::camera = camera;
        camera->SetPosition(glm::vec3(7.0f,2.0f,7.0f));
        camera->UpdateView();

        playerController = new PlayerController();
        playerController->camera = Renderer::camera;

        line1 = new GizmoLine(glm::vec3(0,0,0), glm::vec3(1,0,0));
        line2 = new GizmoLine(glm::vec3(0,0,0), glm::vec3(0,1,0));
        line3 = new GizmoLine(glm::vec3(0,0,0), glm::vec3(0,0,1));
        
        std::cout << glm::vec3(1,0,0).y << std::endl;
        line1->setColor(glm::vec3(1,0,0) + glm::vec3(0));
        line2->setColor(glm::vec3(0,1,0) + glm::vec3(0));
        line3->setColor(glm::vec3(0,0,1) + glm::vec3(0));

        Blocks::InitialiseBlocks();
        auto textures = Blocks::GetTextureNames();
        std::set<std::string> fstfImages;
        #define COMMONPATH(i) getAssetPathMany({"textures",i})

        fstfImages.insert({COMMONPATH("red"), COMMONPATH("green"), COMMONPATH("blue"),  COMMONPATH("pink")});

        TextureManager::CreateAtlasFromFiles(textures);
        
        input.onMouseChangedArr.push_back([this](void* _input){ playerController->OnMouseInput(_input);});

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

        //world = World::LoadWorld("world-imports/single.json");
        world = World::LoadWorld("world-imports/grasswater.json");
        //world = World::LoadWorld("world-imports/desolateisland.json");
        world->transform = glm::translate(glm::vec3(1,0,1));
        glm::mat4 mat4id = glm::mat4(1.0);

    }


    /*
        Setup block texture side definitions: DONE
        Loop over all textures 'pragma once'
        Create an atlas map of them mapping the filename to UV coords
        Foreach block setup the UV Coords that the face texture corrasponds to.
    */

    void LoadMaterials()
    {
        new VertexFragmentCombinationMaterial("basic",          getAssetPathMany({"shaders", "basic_vertex.shader"}),   getAssetPathMany({"shaders", "basic_fragment.shader"}));
        new VertexFragmentCombinationMaterial("alt_textured",   getAssetPathMany({"shaders", "alt_tex_vertex.shader"}), getAssetPathMany({"shaders", "alt_tex_fragment.shader"}));
        new VertexFragmentCombinationMaterial("default",        getAssetPathMany({"shaders", "vertex.shader"}),         getAssetPathMany({"shaders", "fragment.shader"}));
    }

    void Update() override {

        Input();
        playerController->Control(playerMove);

        glClearColor(135/255.0f, 206/255.0f, 235/255.0f, 1.0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        float time = glfwGetTime();
        Renderer::camera->UpdateView();


        #pragma region RENDER_3D_GIZMO
            line1->setMVP(Renderer::camera->projection * Renderer::camera->view);
            line2->setMVP(Renderer::camera->projection * Renderer::camera->view);
            line3->setMVP(Renderer::camera->projection * Renderer::camera->view);

            line1->draw(Renderer::camera);
            line2->draw(Renderer::camera);
            line3->draw(Renderer::camera);
        #pragma endregion

        
        world->renderer->Render();


        EndUpdate();
    }

    void EndUpdate()
    {
        glfwSwapBuffers(window);

        KeyMode* tmp = keyStateCurrent;
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
