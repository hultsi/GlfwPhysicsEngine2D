#pragma once
#include "./GlfwHeaders.h"

class GlfwGameControl;

class GlfwCollision
{
public:
    GlfwCollision();
    GlfwCollision(GlfwGameControl *gameControl);
    std::vector<GlfwSquare> withConvex(GlfwSquare *sqObj, std::vector<GlfwSquare> &colliders);

    /* Also saves colliding points to sqObj */
    std::vector<GlfwSquare> preventPenetration(GlfwSquare *sqObj, std::vector<GlfwSquare> &colliders);

    void pointsOfCollision(GlfwSquare *sqObj, std::vector<GlfwSquare> &colliders);

private:
    GlfwGameControl *gameControl = NULL;
};