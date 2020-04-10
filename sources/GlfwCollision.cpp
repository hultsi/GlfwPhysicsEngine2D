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
std::unordered_map<std::string, GlfwSquare> GlfwCollision::withConvex(GlfwSquare *sqObj, std::unordered_map<std::string, GlfwSquare> &colliders)
{
    std::unordered_map<std::string, GlfwSquare> collidingSquares;

    bool collision = true; // Algorithm "assumes" for collision initially

    Coords coords1;
    std::vector<float> coords1Proj;
    Coords coords2;
    std::vector<float> coords2Proj;

    std::array<int, 2> minMaxInd;
    GlfwSquare *sq1, *sq2;
    float theta, d1, d2, q1, q2;
    Vector2d P;

    //for (int i = 0; i < colliders.size(); i++) //loop over shapes
    for (auto const &[key, val] : colliders)
    {
        sq1 = sqObj;
        sq2 = &colliders[key];
        for (int n = 0; n < 2; n++) // loop over both shapes to get all necessary projections
        {
            if (sqObj == &colliders[key])
                break;
            if (n > 0)
            {
                sq1 = &colliders[key];
                sq2 = sqObj;
            }
            coords1 = sq1->getCoordinates(true);
            coords2 = sq2->getCoordinates(true);
            for (int j = 0; j < coords1.size() / 2; j++) // Loop over half of the points that make the square (can be applied to others this way)
            {
                P = getProjectionVector(coords1.at(j + 1), coords1.at(j));
                // Find min-max projections from first square
                coords1Proj = getProjections(coords1, P);
                minMaxInd = findMinMax(coords1Proj);
                d1 = coords1Proj[minMaxInd[0]];
                d2 = coords1Proj[minMaxInd[1]];
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
                collidingSquares.emplace(key, val);
            }
            else if (n == -1)
                break;
        }
    }

    return collidingSquares;
}

/**
 * Takes in a target convex object and objects that the convex object collides with and prevents
 * the target penetrating the collider objects.
 */
std::unordered_map<std::string, GlfwSquare> GlfwCollision::preventPenetration(GlfwSquare *sqObj, std::unordered_map<std::string, GlfwSquare> &colliders)
{
    std::unordered_map<std::string, GlfwSquare> collidersOut;

    float angleSq = sqObj->velocity.angle();
    float speedSq = sqObj->velocity.length();

    float angleCollider;
    float speedCollider;

    int ind = 0;
    while (colliders.size() > 0)
    {
        collidersOut.clear();
        collidersOut = colliders;

        speedSq = (speedSq > 1 ? speedSq - 1 : 0);
        sqObj->velocity.x = speedSq * std::cos(angleSq);
        sqObj->velocity.y = speedSq * std::sin(angleSq);

        for (auto const &[key, val] : colliders)
        {
            angleCollider = colliders[key].velocity.angle();
            speedCollider = colliders[key].velocity.length();
            speedCollider = (speedCollider > 1 ? speedCollider - 1 : 0);

            colliders[key].velocity.x = speedCollider * std::cos(angleCollider);
            colliders[key].velocity.y = speedCollider * std::sin(angleCollider);
        }

        colliders = withConvex(sqObj, colliders);
    }

    return collidersOut;
}

void GlfwCollision::pointsOfCollision(GlfwSquare *sqObj, std::unordered_map<std::string, GlfwSquare> &colliders)
{
    Coords coords1;
    std::vector<float> coords1Proj;
    Coords coords2;
    std::vector<float> coords2Proj;

    std::array<int, 2> minMaxInd;
    GlfwSquare *sq1, *sq2;
    float theta, d1, d2, q1, q2;
    Vector2d P;

    //for (int i = 0; i < colliders.size(); ++i) //loop over shapes
    for (auto const &[key, val] : colliders)
    {
        sq1 = sqObj;
        sq2 = &colliders[key];
        for (int n = 0; n < 2; ++n) // loop over both shapes to get all necessary projections
        {
            if (sqObj == &colliders[key])
                break;
            if (n > 0)
            {
                sq1 = &colliders[key];
                sq2 = sqObj;
            }
            coords1 = sq1->getCoordinates(true);
            coords2 = sq2->getCoordinates(true);
            for (int j = 0; j < coords1.size() / 2; ++j) // Loop over half of the points that make the rect
            {
                P = getProjectionVector(coords1.at(j + 1), coords1.at(j));

                // Find min-max projections from first square
                coords1Proj = getProjections(coords1, P);
                minMaxInd = findMinMax(coords1Proj);
                d1 = coords1Proj[minMaxInd[0]];
                d2 = coords1Proj[minMaxInd[1]];
                // Find min-max projections from second square
                coords2Proj = getProjections(coords2, P);
                minMaxInd = findMinMax(coords2Proj);
                q1 = coords2Proj[minMaxInd[0]];
                q2 = coords2Proj[minMaxInd[1]];

                // Check if there's a point near a line and draw a debug circle
                // TODO: Fix
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