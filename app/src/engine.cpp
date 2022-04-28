#include "engine.hpp" 
#include "LoadGlad.h"
#include "GL/gl.h"
#include <iostream>
#include <string>
#include "Rendering.hpp"
#include <GLFW/glfw3.h>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define def_getcursorposfun
typedef void (* getcursorposfun)(GLFWwindow*, double*,double*);
Engine *currentEngine;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    currentEngine->engine_key_callback(window, key, scancode, action, mods);
}

void cursorpos_callback(GLFWwindow* window, double x, double y)
{
    currentEngine->input.OnMousePosChanged(window, x, y, glfwGetCursorPos);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Engine::engine_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key >= INPUT_KEYCOUNT || action == GLFW_REPEAT)
    {
        return;
    }
    
    onKeyUpdate = true;

    input.OnKeyboardCallback((KeyCode)key, action == GLFW_PRESS ? KeyState::DOWN : action == GLFW_RELEASE ? KeyState::UP : KeyState::NONE);
}

void Engine::Start() {
    currentEngine = this;
}

void Engine::Update() {
    
}

int Engine::Intialise()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    window = glfwCreateWindow(800, 600, "Mineclone", NULL, NULL);
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
    glfwSetCursorPosCallback(window, cursorpos_callback);  
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui_ImplGlfw_InitForOpenGL(window, true);



    glEnable(GL_DEPTH_TEST);  
    glEnable(GL_CULL_FACE); 
    glViewport(0, 0, this->width, this->width);
    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback( MessageCallback, nullptr);
    this->Start();
    double lasttime = glfwGetTime();

    while(!glfwWindowShouldClose(window) && !quit) {
        input.Refresh();
        deltaTime =  glfwGetTime() - timeLastFrame;
        timeLastFrame = glfwGetTime();
        this->Update();

        while (glfwGetTime() < lasttime + 1.0/targetFPS) {
            // TODO: Put the thread to sleep, yield, or simply do nothing
        }
        lasttime += 1.0/targetFPS;

        glfwPollEvents();  
    }
    glfwTerminate();
    return 0;
}