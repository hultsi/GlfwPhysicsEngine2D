#include "./../headers/GlfwHeaders.h"
#include <iostream>
#include <vector>

GlfwGameControl::GlfwGameControl(float gravity)
{
    std::cout << "Game initialized" << std::endl;

    GlfwCollision collisionObj(this);
    glfwCollision = collisionObj;
}

void GlfwGameControl::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    GlfwGameControl *gameControl = (GlfwGameControl *)glfwGetWindowUserPointer(window);
    std::vector<GlfwSquare> *squares = gameControl->getSquares();
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        squares->at(0).move(-8);
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        squares->at(0).move(8);
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        squares->at(0).move(0, 8);
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        squares->at(0).move(0, -8);
    }
}

GlfwSquare *GlfwGameControl::createObject(GlfwSquare obj)
{
    glfwSquareAll.emplace_back(obj);
    glfwSquareAll.back().pointCollisionControl(&glfwCollision);

    return &glfwSquareAll.back();
}

DebugCircle *GlfwGameControl::createObject(DebugCircle obj)
{
    debugCircleAll.emplace_back(obj);

    return &debugCircleAll.back();
}

void GlfwGameControl::drawAll()
{
    for (int i = 0; i < glfwSquareAll.size(); i++)
    {
        glfwSquareAll.at(i).draw();
    }
    for (int i = 0; i < debugCircleAll.size(); i++)
    {
        debugCircleAll.at(i).draw();
    }
}

void GlfwGameControl::updateAll(double msPerFrame)
{
    for (int i = 0; i < glfwSquareAll.size(); i++)
    {
        glfwSquareAll.at(i).update(msPerFrame);
    }
}

void GlfwGameControl::resetPerformance()
{
    lastTime = glfwGetTime();
    nbFrames = 0;
}

// Returns milliseconds per each frame
double GlfwGameControl::getPerformance(bool printPerformance)
{
    currentTime = glfwGetTime();
    nbFrames++;
    if (currentTime - lastTime >= 1)
    {
        if (printPerformance)
            std::cout << "ms/frame " << 1000.0 / static_cast<double>(nbFrames) << std::endl;
        SPF = 1000.0 / static_cast<double>(nbFrames);
        nbFrames = 0;
        lastTime += 1.0;
    }
    return SPF;
}

std::vector<GlfwSquare> *GlfwGameControl::getSquares()
{
    return &glfwSquareAll;
}