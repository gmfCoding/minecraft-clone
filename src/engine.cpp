#include "engine.h" 
void Engine::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    onKeyUpdate = true;
    if(action == GLFW_PRESS)
        keyStateCurrent[key] = KeyMode::Press;

    if(action == GLFW_RELEASE)
        keyStateCurrent[key] = KeyMode::Release;
}