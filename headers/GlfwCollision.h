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
    // Returns a vector that starts from 0 and has the same angle
    // as the line drawn from point1 to point2
    Vector2d getProjectionVector(Vector2d point1, Vector2d point2);

    // TODO: Functions for getting min max projection for rect 1 and 2
    //

    GlfwGameControl *gameControl = NULL;
};