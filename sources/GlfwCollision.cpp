#include "./../headers/GlfwHeaders.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <array>

GlfwCollision::GlfwCollision() {}

GlfwCollision::GlfwCollision(GlfwGameControl *gameControl)
{
    this->gameControl = gameControl;
}

//Uses Separating Axis Theorem (SAT)
std::vector<GlfwSquare> GlfwCollision::withConvex(GlfwSquare *sqObj, std::vector<GlfwSquare> &colliders)
{
    std::vector<GlfwSquare> collidingSquares;

    bool collision = true; // Algorithm "assumes" for collision initially
    Coords coords1;
    Coords coords2;
    std::array<int, 2> minMaxInd;
    std::vector<float> coords2Proj;

    GlfwSquare *sq1, *sq2;
    float theta, d1, d2, q1, q2;
    Vector2d P;

    for (int i = 0; i < colliders.size(); i++) //loop over shapes
    {
        for (int n = 0; n < 2; n++) // loop over both shapes to get all necessary projections
        {
            if (sqObj == &colliders.at(i))
                break;
            if (n == 0)
            {
                sq1 = sqObj;
                sq2 = &colliders.at(i);
            }
            else
            {
                sq1 = &colliders.at(i);
                sq2 = sqObj;
            }
            coords1 = sq1->getCoordinates(true);
            coords2 = sq2->getCoordinates(true);
            for (int j = 0; j < coords1.size() / 2; j++) // Loop over half of the points that make the square (can be applied to others this way)
            {
                P = getProjectionVector(coords1.at(j + 1), coords1.at(j));
                // Coords1 create the projection plane/line
                d1 = coords1.at(j).dot(P);
                d2 = coords1.at(j + 1).dot(P);
                if (d2 < d1)
                {
                    d1 = d2;
                    d2 = coords1.at(j).dot(P);
                }
                /* Second square */
                // Find min-max projections from second square
                coords2Proj = getProjections(coords2, P);
                minMaxInd = findMinMax(coords2Proj);
                q1 = coords2Proj[minMaxInd[0]];
                q2 = coords2Proj[minMaxInd[1]];

                // Check if there's overlap on the projected 1D line
                if (q1 >= d1 && q1 <= d2 || q2 >= d1 && q2 <= d2 ||
                    d1 >= q1 && d1 <= q2 || d2 >= q1 && d2 <= q2)
                {
                    // Collision happens in this projection
                    // do something?
                }
                else
                {
                    // Collision is not happening with these squares, break loops j and n
                    j = -1;
                    n = -1;
                }

                if (j == -1)
                    break;
            }
            if (n == 1)
            {
                collidingSquares.emplace_back(*sq1);
            }
            if (n == -1)
            {
                break;
            }
        }
    }

    return collidingSquares;
}

/**
 * Takes in a target convex object and objects that the convex object collides with and prevents
 * the target penetrating the collider objects.
 */
std::vector<GlfwSquare> GlfwCollision::preventPenetration(GlfwSquare *sqObj, std::vector<GlfwSquare> &colliders)
{
    std::vector<GlfwSquare> collidersOut;

    float angleSq = sqObj->velocity.angle();
    float speedSq = sqObj->velocity.length();

    std::vector<float> angleCollider;
    std::vector<float> speedCollider;
    for (int i = 0; i < colliders.size(); ++i)
    {
        angleCollider.emplace_back(colliders[i].velocity.angle());
        speedCollider.emplace_back(colliders[i].velocity.length());
    }

    int ind = 0;
    while (colliders.size() > 0)
    {
        collidersOut.clear();
        collidersOut = colliders;

        speedSq = (speedSq > 1 ? speedSq - 1 : 0);
        sqObj->velocity.x = speedSq * std::cos(angleSq);
        sqObj->velocity.y = speedSq * std::sin(angleSq);

        for (int i = 0; i < colliders.size(); ++i)
        {
            speedCollider[i] = (speedCollider[i] > 1 ? speedCollider[i] - 1 : 0);
            colliders[i].velocity.x = speedCollider[i] * std::cos(angleCollider[i]);
            colliders[i].velocity.y = speedCollider[i] * std::sin(angleCollider[i]);
        }

        colliders = withConvex(sqObj, colliders);
    }

    return collidersOut;
}

void GlfwCollision::pointsOfCollision(GlfwSquare *sqObj, std::vector<GlfwSquare> &colliders)
{
    Coords coords1;
    Coords coords2;
    std::array<int, 2> minMaxInd;
    std::vector<float> coords2Proj;

    GlfwSquare *sq1, *sq2;
    float theta, d1, d2, q1, q2;
    Vector2d P;

    for (int i = 0; i < colliders.size(); ++i) //loop over shapes
    {
        for (int n = 0; n < 2; ++n) // loop over both shapes to get all necessary projections
        {
            if (sqObj == &colliders.at(i))
                break;
            if (n == 0)
            {
                sq1 = sqObj;
                sq2 = &colliders.at(i);
            }
            else
            {
                sq1 = &colliders.at(i);
                sq2 = sqObj;
            }
            coords1 = sq1->getCoordinates(true);
            coords2 = sq2->getCoordinates(true);
            for (int j = 0; j < coords1.size() / 2; ++j) // Loop over half of the points that make the rect
            {
                P = getProjectionVector(coords1.at(j + 1), coords1.at(j));
                // Find min-max points of a rectangle's side from the first square
                d1 = coords1.at(j).dot(P);
                d2 = coords1.at(j + 1).dot(P);
                if (d2 < d1)
                {
                    d1 = d2;
                    d2 = coords1.at(j).dot(P);
                }
                /* Second square */
                // Find min-max projections from second square
                coords2Proj = getProjections(coords2, P);
                minMaxInd = findMinMax(coords2Proj);
                q1 = coords2Proj[minMaxInd[0]];
                q2 = coords2Proj[minMaxInd[1]];

                // Check if there's a point near a line and draw a debug circle
                if ((q1 - d2) <= 1 && (q1 - d2) >= 0)
                {
                    gameControl->createObject(DebugCircle(coords2[minMaxInd[0]].x, coords2[minMaxInd[0]].y, 10));
                }
                else if ((d1 - q2) <= 1 && (d1 - q2) >= 0)
                {
                    gameControl->createObject(DebugCircle(coords2[minMaxInd[1]].x, coords2[minMaxInd[1]].y, 10));
                }
            }
        }
    }
}

Vector2d GlfwCollision::getProjectionVector(Vector2d point1, Vector2d point2)
{
    Vector2d P;
    float theta;

    theta = std::atan2((point2.y - point1.y), (point2.x - point1.x));
    theta = (theta < 0 ? theta + 2 * M_PI : theta);
    P.x = std::cos(theta);
    P.y = std::sin(theta);

    return P;
}

std::vector<float> GlfwCollision::getProjections(Coords coords, Vector2d projVec)
{
    std::vector<float> out;

    for (int i = 0; i < coords.size(); ++i)
    {
        out.emplace_back(coords.at(i).dot(projVec));
    }

    return out;
}

std::array<int, 2> GlfwCollision::findMinMax(std::vector<float> arr)
{
    std::array<int, 2> minMax;
    float min, max;
    min = arr[0];
    max = arr[0];
    minMax[0] = 0;
    minMax[1] = 0;
    for (int k = 1; k < arr.size(); ++k)
    {
        if (arr[k] < min)
        {
            min = arr[k];
            minMax[0] = k;
        }
        if (arr[k] > max)
        {
            max = arr[k];
            minMax[1] = k;
        }
    }

    return minMax;
}