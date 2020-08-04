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
        obj.name = "rect_" + rectAll_private.size();

    rectAll_private.emplace(obj.name, obj);
    rectAll_private[obj.name].setGameControl(this);
    rectAll_private[obj.name].pointCollisionControl(&glfwCollision);

    rectAll.emplace(obj.name, &rectAll_private.at(obj.name));

    return rectAll.at(obj.name);
}

DebugCircle *GlfwGameControl::createObject(DebugCircle obj)
{
    if (obj.name == "")
        obj.name = "debug_circle_" + debugCircleAll_private.size();
    debugCircleAll_private.emplace(obj.name, obj);

    debugCircleAll.emplace(obj.name, &debugCircleAll_private.at(obj.name));

    return debugCircleAll.at(obj.name);
}

DebugLine *GlfwGameControl::createObject(DebugLine obj)
{
    if (obj.name == "")
        obj.name = "debug_line_" + debugLineAll_private.size();
    debugLineAll_private.emplace(obj.name, obj);

    debugLineAll.emplace(obj.name, &debugLineAll_private.at(obj.name));

    return debugLineAll.at(obj.name);
}

void GlfwGameControl::drawAll()
{
    for (auto const &[key, val] : rectAll_private)
    {
        rectAll_private.at(key).draw();
    }
    for (auto const &[key, val] : debugCircleAll_private)
    {
        debugCircleAll_private.at(key).draw();
    }
    for (auto const &[key, val] : debugLineAll_private)
    {
        debugLineAll_private.at(key).draw();
    }
}

void GlfwGameControl::updateAllBegin(double msPerFrame)
{
    for (auto const &[key, val] : rectAll_private)
    {
        rectAll_private.at(key).updateBegin();
    }
}

void GlfwGameControl::updateAll(double msPerFrame)
{
    std::unordered_map<std::string, GlfwSquare *> colliders;
    std::vector<std::unordered_map<std::string, GlfwSquare *>> colliders2;

    for (auto const &[key, val] : rectAll_private)
        rectAll_private.at(key).update();

    for (auto const &[key, val] : this->rectAll)
    {
        //First collision check
        colliders = glfwCollision.withConvex(this->rectAll.at(key), this->rectAll, false);

        if (colliders.size() != 0)
        {
            // Maybe use line-line intersection here instead to detect collision.
            glfwCollision.preventPenetration(this->rectAll.at(key), colliders, false);
        }
    }
    //TODO: optimize (move only the ones that are moved)
    for (auto const &[key, val] : rectAll_private)
        rectAll_private.at(key).move();

    for (auto const &[key, val] : this->rectAll)
    {
        glfwCollision.pointsOfCollision(val, val->collidingObjects, false);

        if (val->collisionPoints.size() != 0)
        {
            glfwCollision.calculateImpulse(this->rectAll.at(key), val->collisionPoints, this->rectAll.at(key)->restitution);
            glfwCollision.calculateForceMoment(this->rectAll.at(key), val->collisionPoints);
        }
    }
}

void GlfwGameControl::updateAllEnd(double msPerFrame)
{
    for (auto const &[key, val] : rectAll_private)
    {
        rectAll_private.at(key).updateEnd();
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