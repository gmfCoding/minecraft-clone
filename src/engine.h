#include "glfw3.h"

class Engine {
    public:
    virtual void Start();
    virtual void Update();


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

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

};