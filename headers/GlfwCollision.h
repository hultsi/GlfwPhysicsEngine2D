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

    std::vector<Vector2d> collidingPoints(const Coords &coords1, const std::array<float,2> indSq1,
                                            const Coords &coords2, const std::array<float,4> indSq2,
                                            Vector2d P);

private:
    GlfwGameControl *gameControl;
};