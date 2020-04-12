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
std::unordered_map<std::string, GlfwSquare *> GlfwCollision::withConvex(GlfwSquare *sqObj, std::unordered_map<std::string, GlfwSquare *> &colliders)
{
    std::unordered_map<std::string, GlfwSquare *> collidingSquares;

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
        sq2 = colliders.at(key);
        for (int n = 0; n < 2; n++) // loop over both shapes to get all necessary projections
        {
            if (sqObj == colliders.at(key))
                break;
            if (n > 0)
            {
                sq1 = colliders.at(key);
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
std::unordered_map<std::string, GlfwSquare *> GlfwCollision::preventPenetration(GlfwSquare *sqObj, std::unordered_map<std::string, GlfwSquare *> &colliders)
{
    std::unordered_map<std::string, GlfwSquare *> collidersOut;

    int ind = 0;
    int sign = 0;
    while (colliders.size() > 0)
    {
        collidersOut.clear();
        collidersOut = colliders;

        sqObj->velocity = decreaseVelocity(sqObj->velocity);
        for (auto const &[key, val] : colliders)
        {
            colliders.at(key)->velocity = decreaseVelocity(colliders.at(key)->velocity);
        }

        colliders = withConvex(sqObj, colliders);
    }

    return collidersOut;
}

void GlfwCollision::pointsOfCollision(GlfwSquare *sqObj, std::unordered_map<std::string, GlfwSquare *> &colliders)
{
    Coords coords1;
    std::vector<float> coords1Proj_1;
    std::vector<float> coords1Proj_2;
    Coords coords2;
    std::vector<float> coords2Proj_1;
    std::vector<float> coords2Proj_2;

    std::vector<int> sortedInds;
    std::vector<Vector2d> collidingPoints;
    // Temp pointers for colliders
    GlfwSquare *sq1, *sq2;
    // Used for projections
    float theta, d1, d2, q1, q2;
    // Used for checking if collision point is touching a side
    float w1, w2, w3;
    int ind1, ind2;

    Vector2d P;
    for (auto const &[key, val] : colliders)
    {
        collidingPoints.clear();
        sq1 = sqObj;
        sq2 = colliders.at(key);
        for (int n = 0; n < 2; ++n) // loop over both shapes to get all necessary projections
        {
            if (sqObj == colliders.at(key))
                break;
            if (n > 0)
            {
                sq1 = colliders.at(key);
                sq2 = sqObj;
            }
            coords1 = sq1->getCoordinates(true);
            coords2 = sq2->getCoordinates(true);
            for (int j = 0; j < coords1.size() / 2; ++j) // Loop over half of the points that make the rect
            {
                P = getProjectionVector(coords1.at(j + 1), coords1.at(j));

                // Find min-max projections from first square
                coords1Proj_1 = getProjections(coords1, P);
                sortedInds = sortProjections(coords1Proj_1);
                d1 = coords1Proj_1[sortedInds[0]];
                d2 = coords1Proj_1[sortedInds[sortedInds.size() - 1]];
                // Find min-max projections from second square
                coords2Proj_1 = getProjections(coords2, P);

                // Find min-max projections from in +90deg projection as well
                P.rotate(M_PI / 2);
                coords1Proj_2 = getProjections(coords1, P);
                coords2Proj_2 = getProjections(coords2, P);

                // Check if there's a point near a line and draw a debug circle
                for (int i = 0; i < coords2Proj_1.size(); ++i)
                {
                    if (((coords2Proj_1[i] - d2) <= 1 && (coords2Proj_1[i] - d2) >= 0) ||
                        ((d1 - coords2Proj_1[i]) <= 1 && (d1 - coords2Proj_1[i]) >= 0))
                    {
                        ind1 = sortedInds[sortedInds.size() - 1];
                        ind2 = sortedInds[sortedInds.size() - 2];
                        w1 = std::abs(coords1Proj_2[ind1] - coords2Proj_2[i]);
                        w2 = std::abs(coords1Proj_2[ind2] - coords2Proj_2[i]);
                        w3 = std::abs(coords1Proj_2[ind1] - coords1Proj_2[ind2]);
                        //TODO: save colliding points to the objects
                        if (w1 < w3 && w2 < w3)
                        {
                            gameControl->createObject(DebugCircle(coords2[i].x, coords2[i].y, 10));
                            collidingPoints.emplace_back(Vector2d(coords2[i].x, coords2[i].y));
                            //sqObj->collidingPoints.emplace(sq2,)
                        }
                    }
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

std::vector<int> GlfwCollision::sortProjections(std::vector<float> arr)
{
    std::vector<int> sortedInds;
    for (int i = 0; i < arr.size(); ++i)
        sortedInds.emplace_back(i);

    int temp = 0;
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;

                temp = sortedInds[j];
                sortedInds[j] = sortedInds[j + 1];
                sortedInds[j + 1] = temp;
            }
        }
    }

    return sortedInds;
}

Vector2d GlfwCollision::decreaseVelocity(Vector2d &vel)
{
    Vector2d out;
    float angle = vel.getAngle();
    float speed = vel.getLength();
    int sign = 0;

    speed = (speed > 1 ? speed - 1 : 0);
    sign = getSign(vel.x);
    out.x = sign * speed * std::cos(angle);
    sign = getSign(vel.y);
    out.y = sign * speed * std::sin(angle);

    return out;
}