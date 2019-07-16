#pragma once
#include "./GlfwHeaders.h"
#include <vector>

class GlfwGameControl
{
public:
    GlfwGameControl(float gravity = 0);
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

    GlfwSquare *createObject(GlfwSquare obj);

    std::vector<GlfwSquare> *getSquares();
    void resetPerformance();
    double getPerformance(bool printPerformance = true);
    void updateAll(double msPerFrame = 1);
    void drawAll();

    void test();

private:
    std::vector<GlfwSquare> glfwSquareAll;

    double lastTime = glfwGetTime();
    double currentTime;
    double SPF = 16.666;
    int nbFrames = 0;

    GlfwCollision glfwCollision;
};