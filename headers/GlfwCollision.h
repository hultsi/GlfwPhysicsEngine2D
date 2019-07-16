#pragma once
#include "./GlfwHeaders.h"

class GlfwGameControl;

class GlfwCollision
{
public:
    GlfwCollision();
    GlfwCollision(GlfwGameControl *gameControl);

    GlfwSquare *withSquare(GlfwSquare *sqObj);

private:
    GlfwGameControl *gameControl;
};