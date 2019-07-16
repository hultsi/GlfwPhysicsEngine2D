#include "./../headers/GlfwHeaders.h"
#include <cmath>
#include <iostream>

GlfwCollision::GlfwCollision()
{
}

//TODO: Use separating axis theorem to do this
GlfwSquare *GlfwCollision::withSquare(GlfwSquare *sqObj)
{
    GlfwSquare *collisionObj = nullptr;
    Coords coords1 = sqObj->getCoordinates();

    // Define P unit vector which is parallel
    // to one of the sides of the square
    float theta, d;
    Vector2d P;
    // Square side angle
    theta = std::atan((coords1.at(1).y - coords1.at(0).y) / (coords1.at(1).x - coords1.at(0).x));
    // And finally P
    P.x = std::cos(theta);
    P.y = std::sin(theta);

    d = coords1.at(0).dot(P);
    std::cout << d << std::endl;

    return collisionObj;
}