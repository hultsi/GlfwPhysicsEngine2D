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
                /* First square */
                // Define P unit vector which is parallel
                // to one of the sides of the square
                // Square side angle
                theta = std::atan((coords1.at(j + 1).y - coords1.at(j).y) / (coords1.at(j + 1).x - coords1.at(j).x));
                // And finally P
                P.x = std::cos(theta);
                P.y = std::sin(theta);
                // Find min-max projections from first square
                // Here d2 = d1+width OR d2 = d1+height
                d1 = coords1.at(j).dot(P);
                d2 = coords1.at(j + 1).dot(P);
                if (d2 < d1)
                {
                    d1 = d2;
                    d2 = coords1.at(j).dot(P);
                }
                /* Second square */
                // Find min-max projections from second square
                q1 = coords2.at(0).dot(P);
                q2 = coords2.at(0).dot(P);
                for (int k = 1; k < 4; k++)
                {
                    if (coords2.at(k).dot(P) < q1)
                        q1 = coords2.at(k).dot(P);
                    if (coords2.at(k).dot(P) > q2)
                        q2 = coords2.at(k).dot(P);
                }
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
    std::array<int, 2> coords2Ind;

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
                /* First square */
                // Define P unit vector which is parallel
                // to one of the sides of the square
                // Square side angle
                theta = std::atan((coords1.at(j + 1).y - coords1.at(j).y) / (coords1.at(j + 1).x - coords1.at(j).x));
                // And finally P
                P.x = std::cos(theta);
                P.y = std::sin(theta);
                // Find min-max projections from first square
                // Here d2 = d1+width OR d2 = d1+height
                d1 = coords1.at(j).dot(P);
                d2 = coords1.at(j + 1).dot(P);
                if (d2 < d1)
                {
                    d1 = d2;
                    d2 = coords1.at(j).dot(P);
                }
                /* Second square */
                // Find min-max projections from second square
                q1 = coords2.at(0).dot(P);
                q2 = coords2.at(0).dot(P);
                for (int k = 1; k < 4; k++)
                {
                    if (coords2.at(k).dot(P) < q1)
                    {
                        q1 = coords2.at(k).dot(P);
                        coords2Ind[0] = k;
                    }
                    if (coords2.at(k).dot(P) > q2)
                    {
                        q2 = coords2.at(k).dot(P);
                        coords2Ind[1] = k;
                    }
                }
                // Check if there's a point near a line
                if (std::abs(d2 - q1) <= 1)
                {
                    gameControl->createObject(DebugCircle(coords2[coords2Ind[0]].x, coords2[coords2Ind[0]].y, 10));
                }
                else if (std::abs(d1 - q2) <= 1)
                {
                    gameControl->createObject(DebugCircle(coords2[coords2Ind[1]].x, coords2[coords2Ind[1]].y, 10));
                }
            }
        }
    }
}