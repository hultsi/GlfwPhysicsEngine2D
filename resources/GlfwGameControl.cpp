#include "./../headers/GlfwHeaders.h"
#include <iostream>
#include <vector>

GlfwGameControl::GlfwGameControl(float gravity)
{
    std::cout << "Game initialized" << std::endl;

    GlfwCollision collisionObj(this);
    this->glfwCollision = collisionObj;
}

void GlfwGameControl::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
    }
    /*if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        glfwSquareAll.at(0).move(2);
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        glfwSquareAll.at(0).move(0, 2);
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        glfwSquareAll.at(0).move(0, -2);
    }*/
}
void GlfwGameControl::test()
{
    //glfwSquareAll.at(0).move(-2);
}
GlfwSquare *GlfwGameControl::createObject(GlfwSquare obj)
{
    this->glfwSquareAll.emplace_back(obj);
    glfwSquareAll.back().pointCollisionControl(&glfwCollision);

    return &this->glfwSquareAll.back();
}

void GlfwGameControl::drawAll()
{
    for (int i = 0; i < this->glfwSquareAll.size(); i++)
    {
        glfwSquareAll.at(i).draw();
    }
}

void GlfwGameControl::updateAll(double msPerFrame)
{
    msPerFrame *= 0.001;
    for (int i = 0; i < this->glfwSquareAll.size(); i++)
    {
        glfwSquareAll.at(i).update(msPerFrame);
    }
}

void GlfwGameControl::resetPerformance()
{
    this->lastTime = glfwGetTime();
    this->nbFrames = 0;
}

double GlfwGameControl::getPerformance(bool printPerformance)
{
    this->currentTime = glfwGetTime();
    this->nbFrames++;
    if (this->currentTime - this->lastTime >= 1)
    {
        if (printPerformance)
            std::cout << "ms/frame " << 1000.0 / static_cast<double>(nbFrames) << std::endl;
        this->SPF = 1000.0 / static_cast<double>(nbFrames);
        nbFrames = 0;
        lastTime += 1.0;
    }
    return this->SPF;
}

std::vector<GlfwSquare> *GlfwGameControl::getSquares()
{
    return &this->glfwSquareAll;
}