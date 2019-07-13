#pragma once
#include "./GlfwHeaders.h"

class GlfwGameControl
{
public:
    GlfwGameControl(float gravity = 0);
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

    GlfwSquare *createObject(GlfwSquare obj);

    void updateAll();
    void drawAll();

private:
    std::vector<GlfwSquare> glfwSquareAll;
};