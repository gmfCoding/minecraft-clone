#include <glad/glad.h>

#include "Mineclone.hpp"
#include "Rendering.hpp"
#include "hpp_mineclone.hpp"


Mineclone* Mineclone::instance;

Mineclone::Mineclone()
{
    if(!instance)
        instance = this;
}

void Mineclone::Start()
{
    camera = new Camera(70.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    playerController = new PlayerController();
    playerController->camera = camera;
    playerController->eyeposition = camera->PtrPosition();
}

void Mineclone::SetupPostProcessing()
{
    postprocess = new PostProcessing();
    postprocess->Init();
    postprocess->enable = true;

    underwater_effect = new PostprocessEffect("underwater", new VertexFragmentCombinationMaterial("postfx_underwater", getAssetPath({"shaders", "vertex_uv.shader"}),   getAssetPath({"shaders", "postfx", "underwater.shader"})));
    underwater_effect->Setup();
    underwater_effect->enabled = false;
}

void Mineclone::Update()
{
    playerController->Control(playerMove);
    Renderer::camera->UpdateView();
    world->renderer->Render();
    worldtest->renderer->Render();
}



void Mineclone::RenderingBegin()
{
    postprocess->Begin();
}

void Mineclone::RenderingEnd()
{
    postprocess->End();
    postprocess->ApplyEffects();
}