#include "./../headers/GlfwHeaders.h"
#include <cmath>
#include <iostream>

GlfwCollision::GlfwCollision()
{
}
GlfwCollision::GlfwCollision(GlfwGameControl *gameControl)
{
    this->gameControl = gameControl;
}

//TODO: Use separating axis theorem to do this
GlfwSquare *GlfwCollision::withSquare(GlfwSquare *sqObj)
{
    std::vector<GlfwSquare> *squaresAll = gameControl->getSquares();

    bool collision = true; // Algorithm "assumes" for collision initially
    Coords coords1 = sqObj->getCoordinates();
    Coords coords2;
    // Define P unit vector which is parallel
    // to one of the sides of the square
    float theta, d1, d2;
    Vector2d P;
    for (int i = 0; i < squaresAll->size(); i++)
    {
        for (int j = 0; j < coords1.size() / 2; j++)
        {
            /* First square */
            // Square side angle
            theta = std::atan((coords1.at(j + 1).y - coords1.at(j + 1).y) / (coords1.at(j).x - coords1.at(j).x));
            // And finally P
            P.x = std::cos(theta);
            P.y = std::sin(theta);
            //Here d2 = d1+width OR d2 = d1+height
            d1 = coords1.at(0).dot(P);
            d2 = coords1.at(1).dot(P);
            if (d2 < d1)
            {
                d1 = d2;
                d2 = coords1.at(0).dot(P);
            }
            /* Second square */
            // Calculate projections of each corner to P
            if (&squaresAll->at(i) != sqObj)
            {
                coords2 = squaresAll->at(i).getCoordinates();
                for (int k = 0; k < 4; k++)
                {
                    if (coords2.at(k).dot(P) >= d1 && coords2.at(k).dot(P) <= d2)
                    {
                        break; // Collision happens in this projection
                    }
                    if (k == 3) //Collision not found!
                        return nullptr;
                }
            }
        }
    }
    return nullptr;
}