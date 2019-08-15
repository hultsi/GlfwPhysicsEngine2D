#pragma once
#include "./GlfwHeaders.h"

class GlfwGameControl;

class GlfwCollision
{
public:
    GlfwCollision();
    GlfwCollision(GlfwGameControl *gameControl);
    std::vector<GlfwSquare *> withSquare(GlfwSquare *sqObj);
    std::vector<GlfwSquare *> preventPenetration(GlfwSquare *sqObj);
    float pointOfCollision(GlfwSquare *square1, std::vector<GlfwSquare *> squareOthers);

private:
    GlfwGameControl *gameControl;
};