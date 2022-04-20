#pragma once
#include <vector>
#include <functional>

struct GLFWwindow;

#ifndef def_getcursorposfun
typedef void (* getcursorposfun)(GLFWwindow*, double*,double*);
#endif

class Input {
    double prevMousePosX = 400;
    double prevMousePosY = 300;
public:

    double currentMousePosX;
    double currentMousePosY;

    double mouseXDelta = 0;
    double mouseYDelta = 0;

    void OnMousePosChanged(GLFWwindow* window, double x, double y, getcursorposfun getcursor);
    std::vector<std::function<void(void*)>> onMouseChangedArr;
};

