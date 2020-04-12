#pragma once
#include "./GlfwHeaders.h"

class GlfwGameControl;

class GlfwCollision
{
public:
    GlfwCollision();
    GlfwCollision(GlfwGameControl *gameControl);
    std::unordered_map<std::string, GlfwSquare> withConvex(GlfwSquare *sqObj, std::unordered_map<std::string, GlfwSquare> &colliders);

    /* Also saves colliding points to sqObj */
    std::unordered_map<std::string, GlfwSquare> preventPenetration(GlfwSquare *sqObj, std::unordered_map<std::string, GlfwSquare> &colliders);

    void pointsOfCollision(GlfwSquare *sqObj, std::unordered_map<std::string, GlfwSquare> &colliders);

private:
    // Returns a vector that starts from 0 and has the same angle
    // as the line drawn from point1 to point2
    Vector2d getProjectionVector(Vector2d point1, Vector2d point2);

    // Calculate projections of coords on the projVec
    std::vector<float> getProjections(Coords coords, Vector2d projVec);

    // Find min-max projection. Returns index [min, max].
    std::array<int, 2> findMinMax(std::vector<float> arr);

    // Sort projections. Returns indices in sorted manner.
    std::vector<int> sortProjections(std::vector<float> arr);

    GlfwGameControl *gameControl = NULL;
};