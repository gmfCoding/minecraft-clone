
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <vector>
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

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

    PlayerController* playerController;

    Object* posXCube;
    Object* negXCube;

    Object* posYCube;
    Object* negYCube;

    Object* posZCube;
    Object* negZCube;

    Object* cubie;


    GizmoLine* line1;
    GizmoLine* line2;
    GizmoLine* line3;

    Image image;


    Object* CreatePosCube(Mesh* mesh, glm::vec3 direction)
    {
        Object* cube = new Object();
        cube->renderer = new MeshRenderer("default");
        cube->renderer->Bind(mesh);
        
        cube->SetScale(glm::vec3(1.0f,0.1f,0.1f));
        
        cube->SetPosition(direction * 2.0f);
        for (size_t i = 0; i < 3; i++)
        {
            if(direction[i] < 0)
            {
                direction[0] = 1.0 - abs(direction[0]);
                direction[1] = 1.0 - abs(direction[1]);
                direction[2] = 1.0 - abs(direction[2]);
                break;
            }
        }

        
        cube->colour = glm::vec4(direction, 1);
        return cube;
    }


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

        Mesh* mesh = GenerateCubeMesh();

        cubie = new Object();
        cubie->renderer = new MeshRenderer("default");
        cubie->renderer->Bind(mesh);
        cubie->SetScale(glm::vec3(0.1f,0.1f,0.1f));
        cubie->SetPosition(glm::vec3(0,0,0));

        posXCube = CreatePosCube(mesh, glm::vec3(1,0,0));

        negXCube = CreatePosCube(mesh, glm::vec3(-1,0,0));
        posYCube = CreatePosCube(mesh, glm::vec3(0,1,0));
        negYCube = CreatePosCube(mesh, glm::vec3(0,-1,0));
        posZCube = CreatePosCube(mesh, glm::vec3(0,0,1));
        negZCube = CreatePosCube(mesh, glm::vec3(0,0,-1));

        line1 = new GizmoLine(glm::vec3(0,0,0), glm::vec3(1,0,0));
        line2 = new GizmoLine(glm::vec3(0,0,0), glm::vec3(0,1,0));
        line3 = new GizmoLine(glm::vec3(0,0,0), glm::vec3(0,0,1));
        
        std::cout << glm::vec3(1,0,0).y << std::endl;
        line1->setColor(glm::vec3(1,0,0) + glm::vec3(0));
        line2->setColor(glm::vec3(0,1,0) + glm::vec3(0));
        line3->setColor(glm::vec3(0,0,1) + glm::vec3(0));

        Blocks::InitialiseBlocks();


        LoadTextureExample();
        
        input.onMouseChangedArr.push_back([this](void* _input){ playerController->OnMouseInput(_input);});

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 

        //world = World::LoadWorld("world-imports/single.json");
        world = World::LoadWorld("world-imports/grasswater.json");
        //world = World::LoadWorld("world-imports/desolateisland.json");
        world->transform = glm::translate(glm::vec3(1,0,1));
        glm::mat4 mat4id = glm::mat4(1.0);


        TexturedQuadExample();
    }


    /*
        Setup block texture side definitions: DONE
        Loop over all textures 'pragma once'
        Create an atlas map of them mapping the filename to UV coords
        Foreach block setup the UV Coords that the face texture corrasponds to.
    */

    void LoadTextureExample()
    {
        std::string filePath = Blocks::blockIDToConfig[Blocks::blockNameToID["sand"]]->textureFiles[0];
        std::string path = getAssetPathMany({"textures", filePath+".png"});
        image.Load(path);
    }

    void LoadMaterials()
    {
        new VertexFragmentCombinationMaterial("basic",          getAssetPathMany({"shaders", "basic_vertex.shader"}),   getAssetPathMany({"shaders", "basic_fragment.shader"}));
        new VertexFragmentCombinationMaterial("alt_textured",   getAssetPathMany({"shaders", "alt_tex_vertex.shader"}), getAssetPathMany({"shaders", "alt_tex_fragment.shader"}));
        new VertexFragmentCombinationMaterial("default",        getAssetPathMany({"shaders", "vertex.shader"}),         getAssetPathMany({"shaders", "fragment.shader"}));
    }

    ImageExample ie;

    void TexturedQuadExample()
    {
        ie = ImageExample();
        ie.Init(&image);
    }


    void Update() override {
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        Input();
        playerController->Control(playerMove);

        float rotation = glfwGetTime();
        Renderer::camera->UpdateView();

        Renderer::RenderObject(negZCube);

        line1->setMVP(Renderer::camera->projection * Renderer::camera->view);
        line2->setMVP(Renderer::camera->projection * Renderer::camera->view);
        line3->setMVP(Renderer::camera->projection * Renderer::camera->view);

        line1->draw(Renderer::camera);
        line2->draw(Renderer::camera);
        line3->draw(Renderer::camera);
        
        ie.Update();
        
        
        world->renderer->Render();


        ImGUIExample();


        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);  
    }

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    void ImGUIExample()
    {
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
    }

    bool hideMouse = false;
    void Input()
    {
        if(onKeyUpdate)
        {
#define KEYCONTROL(keyId, bvar) if(input.isKeyHeld(KeyCode::KEY_##keyId)) { bvar = true;} else{bvar = false;}
            KEYCONTROL(A,playerMove.Left)
            KEYCONTROL(S,playerMove.Backwards)
            KEYCONTROL(D,playerMove.Right)
            KEYCONTROL(W,playerMove.Forward)
            KEYCONTROL(Q,playerMove.Down)
            KEYCONTROL(E,playerMove.Up)


            if(input.isKeyDown(KeyCode::KEY_ESCAPE))
            {
                hideMouse = !hideMouse;

                glfwSetInputMode(window, GLFW_CURSOR, hideMouse ? GLFW_CURSOR_DISABLED: GLFW_CURSOR_NORMAL); 
                input.MouseIgnoreNextDelta();
            }
        }
    }
};


int main() {
    Mineclone engine = Mineclone();
    return engine.Intialise();
}
