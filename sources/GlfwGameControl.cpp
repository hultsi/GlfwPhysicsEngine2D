#include "./../headers/GlfwHeaders.h"
#include <iostream>
#include <vector>

GlfwGameControl::GlfwGameControl(float gravity)
{
    std::cout << "Game initialized" << std::endl;

    glfwCollision = GlfwCollision(this);
}

void GlfwGameControl::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    GlfwGameControl *gameControl = (GlfwGameControl *)glfwGetWindowUserPointer(window);
    std::unordered_map<std::string, GlfwSquare> *squares = gameControl->getSquares();
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        // Do stuff
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        // Do stuff
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        // Do stuff
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        // Do stuff
    }
}

GlfwSquare *GlfwGameControl::createObject(GlfwSquare obj)
{
    if (obj.name == "")
        obj.name = "rect_" + glfwSquareAll.size();

    glfwSquareAll.emplace(obj.name, obj);
    glfwSquareAll[obj.name].setGameControl(this);
    glfwSquareAll[obj.name].pointCollisionControl(&glfwCollision);

    return &glfwSquareAll.at(obj.name);
}

DebugCircle *GlfwGameControl::createObject(DebugCircle obj)
{
    if (obj.name == "")
        obj.name = "debug_circle_" + debugCircleAll.size();
    debugCircleAll.emplace(obj.name, obj);
    debugCircleAll.emplace("", obj);

    return &debugCircleAll.at("");
}

DebugLine *GlfwGameControl::createObject(DebugLine obj)
{
    if (obj.name == "")
        obj.name = "debug_line_" + debugLineAll.size();
    debugLineAll.emplace(obj.name, obj);

    return &debugLineAll.at(obj.name);
}

void GlfwGameControl::drawAll()
{
    for (auto const &[key, val] : glfwSquareAll)
    {
        glfwSquareAll.at(key).draw();
    }
    for (auto const &[key, val] : debugCircleAll)
    {
        debugCircleAll.at(key).draw();
    }
    for (auto const &[key, val] : debugLineAll)
    {
        debugLineAll.at(key).draw();
    }
}

void GlfwGameControl::updateAll(double msPerFrame)
{
    for (auto const &[key, val] : glfwSquareAll)
    {
        glfwSquareAll.at(key).update();
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

std::unordered_map<std::string, GlfwSquare> *GlfwGameControl::getSquares()
{
    return &glfwSquareAll;
}