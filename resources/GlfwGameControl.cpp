#include "./../headers/GlfwHeaders.h"
#include <iostream>

GlfwGameControl::GlfwGameControl(float gravity)
{
    std::cout << "Game initialized" << std::endl;

    this->gravity = gravity;
}

void GlfwGameControl::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

GlfwSquare *GlfwGameControl::createObject(GlfwSquare obj, bool applyGravity)
{
    this->glfwSquareAll.emplace_back(obj);
    if (applyGravity)
    {
        this->glfwSquareAll.back().applyGravity(&this->gravity);
    }

    return &this->glfwSquareAll.back();
}

void GlfwGameControl::drawAll()
{
    for (int i = 0; i < this->glfwSquareAll.size(); i++)
    {
        glfwSquareAll[i].draw();
    }
}

float *GlfwGameControl::getGravity()
{
    return &this->gravity;
}