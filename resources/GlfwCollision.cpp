#include "./../headers/GlfwHeaders.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <array>

GlfwCollision::GlfwCollision()
{
}
GlfwCollision::GlfwCollision(GlfwGameControl *gameControl)
{
    this->gameControl = gameControl;
}

//Uses Separating Axis Theorem (SAT)
//TODO: Change to "Rectangle" instead...
std::vector<GlfwSquare *> GlfwCollision::withSquare(GlfwSquare *sqObj)
{
    std::vector<GlfwSquare> *squaresAll = gameControl->getSquares();
    std::vector<GlfwSquare *> collidingSquares;

    bool collision = true; // Algorithm "assumes" for collision initially
    Coords coords1;
    Coords coords2;

    GlfwSquare *sq1, *sq2;
    float theta, d1, d2, q1, q2;
    Vector2d P;
    for (int i = 0; i < squaresAll->size(); i++) //loop over squares
    {
        for (int n = 0; n < 2; n++) // loop over both squares to get all necessary projections
        {
            if (sqObj == &squaresAll->at(i))
                break;
            if (n == 0)
            {
                sq1 = sqObj;
                sq2 = &squaresAll->at(i);
            }
            else
            {
                sq1 = &squaresAll->at(i);
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
                collidingSquares.emplace_back(sq1);
            }
            if (n == -1)
            {
                break;
            }
        }
    }

    return collidingSquares;
}

std::vector<GlfwSquare *> GlfwCollision::preventPenetration(GlfwSquare *sqObj)
{
    int index;
    std::vector<GlfwSquare *> collidingSquares;
    std::vector<GlfwSquare *> collidingSquaresInit = withSquare(sqObj);
    int Sq_N = collidingSquaresInit.size();
    GlfwSquare *currentSq = sqObj;
    Vector2d vel;
    while (withSquare(sqObj).size() != 0)
    {
        collidingSquares = withSquare(sqObj);
        if (index % (Sq_N + 1) == 0)
        {
            currentSq = sqObj;
        }
        else
        {
            currentSq = collidingSquares[(index % (Sq_N + 1)) - 1];
        }

        if (std::abs(currentSq->velocity.x * currentSq->velocity.x + currentSq->velocity.y * currentSq->velocity.y) <= 1)
        {
            currentSq->velocity.x = 0;
            currentSq->velocity.y = 0;
        }
        if (std::abs(currentSq->D_rotation) <= 0.025)
            currentSq->D_rotation = 0;

        if (currentSq->velocity.x != 0)
            currentSq->velocity.x = (currentSq->velocity.x > 0 ? currentSq->velocity.x - 1 : currentSq->velocity.x + 1);
        if (currentSq->velocity.y != 0)
            currentSq->velocity.y = (currentSq->velocity.y > 0 ? currentSq->velocity.y - 1 : currentSq->velocity.y + 1);
        if (currentSq->D_rotation != 0)
            currentSq->D_rotation = (currentSq->D_rotation > 0 ? currentSq->D_rotation - 0.025 : currentSq->D_rotation + 0.025);

        ++index;
    }
    return collidingSquaresInit;
}

// Find separating line using SAT and see which point(s) are very close to that line
std::vector<Vector2d> GlfwCollision::pointsOfCollision(GlfwSquare *square1, std::vector<GlfwSquare *> squareOthers)
{
    bool collision = true; // Algorithm "assumes" for collision initially
    Coords coords1;
    Coords coords2;
    std::vector<Vector2d> points;

    GlfwSquare *sq1, *sq2;
    float theta;
    std::array<float, 2> indSq1; // from min to max projection coordinate
    std::array<float, 4> indSq2; // from min to max projection coordinate

    Vector2d P;
    for (int i = 0; i < squareOthers.size(); i++) //loop over squares
    {
        for (int n = 0; n < 2; n++) // loop over both squares to get all necessary projections
        {
            if (square1 == squareOthers.at(i))
                break;
            if (n == 0)
            {
                sq1 = square1;
                sq2 = squareOthers.at(i);
            }
            else
            {
                sq1 = squareOthers.at(i);
                sq2 = square1;
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
                // Find projection of one side of the rectangle
                // Here d2 = d1+width OR d2 = d1+height
                indSq1[0] = j;     //coords1.at(j).dot(P);
                indSq1[1] = j + 1; //coords1.at(j + 1).dot(P);
                if (coords1.at(indSq1[1]).dot(P) < coords1.at(indSq1[0]).dot(P))
                {
                    indSq1[0] = j + 1;
                    indSq1[1] = j;
                }
                /* Second square */
                // Find min-max projections from second square
                indSq2[0] = 0;
                indSq2[1] = 0;
                indSq2[2] = 0;
                indSq2[3] = 0;
                for (int k = 1; k < 4; ++k)
                {
                    if (coords2.at(k).dot(P) < coords2.at(indSq2[0]).dot(P))
                        indSq2[0] = k;
                    if (coords2.at(k).dot(P) > coords2.at(indSq2[3]).dot(P))
                        indSq2[3] = k;
                }
                for (int k = 0; k < 4; ++k)
                {
                    if (indSq2[0] != k && indSq2[3] != k)
                        indSq2[1] = k;
                }
                for (int k = 0; k < 4; ++k)
                {
                    if (indSq2[0] != k && indSq2[3] != k && indSq2[1] != k)
                        indSq2[2] = k;
                }
                // Check if there's overlap on the projected 1D line
                if (coords2.at(indSq2[0]).dot(P) >= coords1.at(indSq1[0]).dot(P) &&
                        coords2.at(indSq2[0]).dot(P) <= coords1.at(indSq1[1]).dot(P) ||
                    coords2.at(indSq2[3]).dot(P) >= coords1.at(indSq1[0]).dot(P) &&
                        coords2.at(indSq2[3]).dot(P) <= coords1.at(indSq1[1]).dot(P) ||
                    coords1.at(indSq1[0]).dot(P) >= coords2.at(indSq2[0]).dot(P) &&
                        coords1.at(indSq1[0]).dot(P) <= coords2.at(indSq2[3]).dot(P) ||
                    coords1.at(indSq1[1]).dot(P) >= coords2.at(indSq2[0]).dot(P) &&
                        coords1.at(indSq1[1]).dot(P) <= coords2.at(indSq2[3]).dot(P))
                {
                    // Collision happens in this projection
                    // do something?
                }
                else
                {
                    // Collision is not happening with these squares
                    // This should only happen for one projection in _almost_ all cases
                    // since we know collision has happened already.
                    points = collidingPoints(coords1, indSq1, coords2, indSq2, P, theta);
                    for (int i = 0; i < points.size() - 1; ++i)
                    {
                        //std::cout << points[i].x << "\n";
                        //std::cout << points[i].y << "\n";
                        gameControl->createObject(DebugCircle(points[i].x, points[i].y, 10));
                    }
                    // Vector2d vec(points[i].x + cos(3 * M_PI / 2 + sq1->rotation), points[i].y + sin(3 * M_PI / 2 + sq1->rotation));
                    // points.emplace_back(vec);
                    // std::cout << points[points.size() - 1].x << "\n";
                    // std::cout << points[points.size() - 1].y << "\n";

                    j = -1;
                    n = -1;
                }

                if (j == -1)
                    break;
            }
            if (n == -1)
            {
                break;
            }
        }
    }

    return points;
}

std::vector<Vector2d> GlfwCollision::collidingPoints(const Coords &coords1, const std::array<float, 2> indSq1,
                                                     const Coords &coords2, const std::array<float, 4> indSq2, Vector2d P, const float theta)
{
    std::vector<Vector2d> points;
    float dist = 0;
    int ind = 0;
    // Find colliding points. If single points is found, points.size() = 2,
    // otherwise points.size() = 4
    for (int i = 0; i < indSq1.size(); ++i)
    {
        for (int k = 0; k < indSq2.size(); ++k)
        {
            dist = std::abs(coords1.at(indSq1[i]).dot(P) - coords2.at(indSq2[k]).dot(P));
            //std::cout << dist << "\n";
            // 1.1 to avoid rounding errors
            if (dist <= 1.5)
            {
                ind = i;
                points.emplace_back(coords1[indSq1[i]]);
                points.emplace_back(coords2[indSq2[k]]);
            }
        }
    }
    if (points.size() > 2)
    {
        // points[2] will be same as points[0] due to the projection
        // method and that needs to be fixed
        if (indSq1[ind] > indSq1[1 - ind])
        {
            points[2] = coords1[indSq1[ind] + 1 % 4];
        }
        else
        {
            points[2] = coords1[indSq1[ind] - 1 % 4];
        }
        P.x = std::cos(theta + M_PI / 2);
        P.y = std::sin(theta + M_PI / 2);
        Vector2d p;
        // Bubble sort points based on the projection points.dot(P)
        for (int k = 0; k < 3; ++k)
        {
            if (points[k].dot(P) > points[k + 1].dot(P))
            {
                p = points[k];
                points[k] = points[k + 1];
                points[k + 1] = p;
                k = -1;
            }
        }
        // Remove smallest and largest value
        points.erase(points.end() - 1);
        points.erase(points.begin());

        Vector2d vec;
        vec.x = points[1].x - points[0].x;
        vec.y = points[1].y - points[0].y;
        vec.normalize();
        vec.rotate(M_PI / 2);
        points.emplace_back(vec);
    }
    else
    {
        // First point is the is just for creating normal, second is the colliding one
        // x will always be positive
        Vector2d vec;

        vec.x = points[0].x - points[1].x;
        vec.y = points[0].y - points[1].y;
        if (vec.x < 1)
            vec.x = 0;
        if (vec.y < 1)
            vec.y = 0;
        vec.normalize();
        vec.rotate(M_PI / 2);
        points.emplace_back(vec);

        points.erase(points.begin());
    }

    return points;
}