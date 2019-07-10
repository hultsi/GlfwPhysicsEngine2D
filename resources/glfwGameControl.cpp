#include "./../headers/glHeaders.h"
#include <iostream>

GameControl::GameControl()
{
    std::cout << "Game initialized" << std::endl;
}

void GameControl::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

GlfwSquare *GameControl::createObject(GlfwSquare obj)
{
    this->glfwSquareAll.emplace_back(obj);

    return &this->glfwSquareAll.back();
}

void GameControl::drawAll()
{
    for (int i = 0; i < this->glfwSquareAll.size(); i++)
    {
        glfwSquareAll[i].draw();
    }
}