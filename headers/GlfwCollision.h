#pragma once
#include "./GlfwHeaders.h"

class GlfwGameControl;

class GlfwCollision
{
public:
    int debugInt = 0;

    GlfwCollision();
    GlfwCollision(GlfwGameControl *gameControl);
    std::unordered_map<std::string, GlfwSquare *> withConvex(GlfwSquare *sqObj, std::unordered_map<std::string, GlfwSquare *> &colliders, bool withVelocity = true);

    /* Also saves colliding points to sqObj */
    std::unordered_map<std::string, GlfwSquare *> preventPenetration(GlfwSquare *sqObj, std::unordered_map<std::string, GlfwSquare *> &colliders, bool withVelocity = true);

    void pointsOfCollision(GlfwSquare *sqObj, std::unordered_map<std::string, GlfwSquare *> &colliders, bool withVelocity = true);

    bool linesIntersect(float x0_1, float y0_1, float x0_2, float y0_2, float x1_1, float y1_1, float x1_2, float y1_2);
    Vector2d intersectionPoint(float x0_1, float y0_1, float x0_2, float y0_2, float x1_1, float y1_1, float x1_2, float y1_2);

    void calculateImpulse(GlfwSquare *sqObj, std::unordered_map<GlfwSquare *, std::unordered_map<std::string, std::vector<Vector2d>>> collisionPoints, float restitution);

    void calculateForceMoment(GlfwSquare *sqObj, std::unordered_map<GlfwSquare *, std::unordered_map<std::string, std::vector<Vector2d>>> collisionPoints);

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

    Vector2d decreaseVelocity(Vector2d &vel);
    float decreaseAngularVelocity(float omega, float angSpd);
    Vector2d decreasePosition(Vector2d &pos, Vector2d &prevPos, Vector2d &vel);
    float decreaseAngle(float angle, float prevAngle, float omega);
    // Returns true if the 2 points are equal (<.1 diff) and
    // form a colliding side instead of a colliding single point
    bool formCollidingLine(float a, float b);

    //
    std::unordered_map<std::string, std::vector<Vector2d>> getNearbyPoints(std::vector<float> proj1, std::vector<float> proj2);

    // Sign of a number
    template <typename T>
    int getSign(T val)
    {
        return (T(0) < val) - (val < T(0));
    }

    GlfwGameControl *gameControl = NULL;
};