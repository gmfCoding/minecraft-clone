#include "engine.h" 
#include "glad.h"
#include "GL/gl.h"
#include <iostream>
#include <string>
#include "Rendering.hpp"
#include <GLFW/glfw3.h>

Engine *currentEngine;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    currentEngine->engine_key_callback(window, key, scancode, action, mods);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Engine::engine_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    onKeyUpdate = true;
    if(action == GLFW_PRESS)
        keyStateCurrent[key] = KeyMode::Press;

    if(action == GLFW_RELEASE)
        keyStateCurrent[key] = KeyMode::Release;
}

void Engine::Start() {
    currentEngine = this;
    keyStateCurrent = &_keyStateBuffer1[0];
    keyStatePrevious = &_keyStateBuffer2[0];

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

    glEnable(GL_DEPTH_TEST);  
    glEnable(GL_CULL_FACE); 
    glViewport(0, 0, 800, 600);
    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback( MessageCallback, nullptr);

    Renderer::CreateProgram("default", "./shaders/vertex.shader", "./shaders/fragment.shader");

    this->Start();

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    while(!glfwWindowShouldClose(window)) {
        this->Update();
    }
    glfwTerminate();
    return 0;
}