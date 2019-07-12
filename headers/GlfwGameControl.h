#pragma once
#include "./GlfwHeaders.h"

class GlfwGameControl
{
public:
    GlfwGameControl(float gravity = 0);
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

    float *getGravity();
    GlfwSquare *createObject(GlfwSquare obj, bool applyGravity = false);
    void drawAll();

private:
    std::vector<GlfwSquare> glfwSquareAll;
    float gravity;
};