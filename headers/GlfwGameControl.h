#pragma once
#include "./GlfwHeaders.h"
#include <vector>
#include <unordered_map>

class GlfwGameControl
{
public:
    //TODO: Change to maps
    std::unordered_map<std::string, GlfwSquare *> rectAll;
    std::unordered_map<std::string, DebugCircle *> debugCircleAll;
    std::unordered_map<std::string, DebugLine *> debugLineAll;

    GlfwGameControl(float gravity = 0);
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

    GlfwSquare *createObject(GlfwSquare obj);
    DebugCircle *createObject(DebugCircle obj);
    DebugLine *createObject(DebugLine obj);

    void resetPerformance();
    double getPerformance(bool printPerformance = true);
    void updateAll(double msPerFrame = 1);
    void drawAll();

private:
    std::unordered_map<std::string, GlfwSquare> rectAll_private;
    std::unordered_map<std::string, DebugCircle> debugCircleAll_private;
    std::unordered_map<std::string, DebugLine> debugLineAll_private;

    double lastTime = glfwGetTime();
    double currentTime;
    double SPF = 16.666;
    int nbFrames = 0;

    GlfwCollision glfwCollision;
};