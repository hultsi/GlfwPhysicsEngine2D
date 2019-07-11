#pragma once
#include "./GlfwHeaders.h"

class GlfwGameControl
{
public:
    GlfwGameControl();
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

    GlfwSquare *createObject(GlfwSquare obj);
    void drawAll();

private:
    std::vector<GlfwSquare> glfwSquareAll;
};