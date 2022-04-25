#pragma once
#include "Input.hpp"
class GLFWwindow;

class Engine {

    public:
    int Intialise();
    virtual void Start();
    virtual void Update();


    GLFWwindow* window;
    double timeLastFrame;
    double deltaTime;
    double targetFPS = 60.0;
    bool quit = false;
    enum KeyMode{
        Release,
        Press,
        Hold,
        None
    };

    #define RECKEY_COUNT 120
    KeyMode _keyStateBuffer1[RECKEY_COUNT] = {KeyMode::None};
    KeyMode _keyStateBuffer2[RECKEY_COUNT] = {KeyMode::None};

    KeyMode* keyStateCurrent;
    KeyMode* keyStatePrevious;
    bool onKeyUpdate;

    Input input;

    void engine_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

extern Engine *currentEngine;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursorpos_callback(GLFWwindow* window, double x, double y);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);