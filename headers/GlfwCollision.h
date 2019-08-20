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

    std::vector<Vector2d> collidingPoints(Coords &coords1, std::array<int,2> indSq1,
                                            Coords &coords2, std::array<int,4> indSq2);

private:
    GlfwGameControl *gameControl;
};