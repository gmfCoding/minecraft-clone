
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

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

class Mineclone : public Engine {

    PlayerMove playerMove{};
    Camera* camera;
    World* world;

    PlayerController* playerController;

    GizmoLine* line1;
    GizmoLine* line2;
    GizmoLine* line3;

    Image* image;
    ImageExample ie;

    std::map<std::string, RectUV> uvMap;

    void Start() override {
        Engine::Start();
        targetFPS = 144.0;

        limitFPS = true;
        glfwSwapInterval(0);

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
        // fstfImages.insert({TEXTUREPATH("red"), TEXTUREPATH("green"), TEXTUREPATH("blue"),  TEXTUREPATH("pink")});

        int pixelsX = 0;
        int pixelsY = 0;
        PixelData* atlas = nullptr;
        
        TextureManager::CreateAtlasFromFiles(textures, pixelsX, pixelsY, atlas, uvMap);
        Blocks::worldTextureUVMap = &uvMap;

        image = new Image(atlas, pixelsX, pixelsY, 4);

        GLuint textureID = TextureManager::UploadNamedTexture(image, "worldatlas");
        ie.Init(textureID);

        input.onMouseChangedArr.push_back([this](void* _input){ playerController->OnMouseInput(_input);});

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

        #define TEXTUREPATH(i) getAssetPathMany({"textures",i})


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
        auto alttexdebug = new VertexFragmentCombinationMaterial("alt_textured_debug",   getAssetPathMany({"shaders", "alt_tex_vertex_debug.shader"}), getAssetPathMany({"shaders", "alt_tex_fragment_debug.shader"}));
        alttexdebug->defaults.properties["dsp"] = false;
        new VertexFragmentCombinationMaterial("default",        getAssetPathMany({"shaders", "vertex.shader"}),         getAssetPathMany({"shaders", "fragment.shader"}));
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


        line1->setMVP(Renderer::camera->projection * Renderer::camera->view);
        line2->setMVP(Renderer::camera->projection * Renderer::camera->view);
        line3->setMVP(Renderer::camera->projection * Renderer::camera->view);

        line1->draw(Renderer::camera);
        line2->draw(Renderer::camera);
        line3->draw(Renderer::camera);
        
        //ie.Update();
        
        world->renderer->Render();


        ImGUIExample();


        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);  
    }

    bool show_demo_window = false;
    bool show_atlas_tex = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    RectUV genRect = RectUV(glm::vec2(0.0, 0.0), glm::vec2(0.0, 1.0), glm::vec2(1.0, 1.0), glm::vec2(1.0, 0.0), 1.0);
    void ImGUIExample()
    {
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            
            ImGui::Begin("Information");

            if (ImGui::Button("View Atlas"))
                show_atlas_tex = true;
            
            Material* mat = MaterialSystem::materialMap["alt_textured_debug"];
            bool &dsp = mat->defaults.GetValueRef<bool>("dsp");
            ImGui::Checkbox("DSP:", &dsp);
            ImGui::InputFloat2("topRight", &genRect.topRight[0]);
            ImGui::InputFloat2("topLeft", &genRect.topLeft[0]);
            ImGui::InputFloat2("bottomRight", &genRect.bottomRight[0]);
            ImGui::InputFloat2("bottomLeft", &genRect.bottomLeft[0]);

            if (ImGui::Button("Regenerate Mesh"))
            {
                world->renderer->rectuv = genRect;
                world->renderer->Regenerate();
                world->renderer->Bind(world->renderer->m_mesh);
            }

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_atlas_tex)
        {
            ImGui::Begin("World atlas texture", &show_atlas_tex);
            GLuint atlastex_id = TextureManager::GetNamedTexture("worldatlas");
            ImGui::Image((void*)(intptr_t)atlastex_id, ImVec2(48*4, 48*4));

            if (ImGui::Button("Close Me"))
                show_atlas_tex = false;
            ImGui::End();
        }
    }

    bool hideMouse = true;
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
