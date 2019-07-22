#pragma once
#include "./GlfwHeaders.h"

class GlfwGameControl;

class GlfwCollision
{
public:
    GlfwCollision();
    GlfwCollision(GlfwGameControl *gameControl);

    float GlfwPointOfCollision();
    void preventPenetration(GlfwSquare *sqObj);

    std::vector<GlfwSquare *> withSquare(GlfwSquare *sqObj);

private:
    GlfwGameControl *gameControl;
};