#include "Input.hpp"

class GLFWwindow;

void Input::OnMousePosChanged(GLFWwindow* window, double x, double y, getcursorposfun glfwGetCursorPos)
{
    if (firstTime)
    {
        glfwGetCursorPos(window, &prevMousePosX, &prevMousePosY);
        firstTime = false;
    }
    
    glfwGetCursorPos(window, &currentMousePosX, &currentMousePosY);
    mouseXDelta = currentMousePosX - prevMousePosX;
    mouseYDelta = prevMousePosY - currentMousePosY;
    prevMousePosX = currentMousePosX;
    prevMousePosY = currentMousePosY;

    for (auto &&cb : onMouseChangedArr)
    {
        cb(this);
    }
}