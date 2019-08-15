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
            if (n == -1)
            {
                break;
            }
            if (n == 1)
            {
                collidingSquares.emplace_back(sq1);
            }
        }
    }

    return collidingSquares;
}

std::vector<GlfwSquare *> GlfwCollision::preventPenetration(GlfwSquare *sqObj)
{
    double v, theta;
    std::vector<GlfwSquare *> collidingSquares;
    while (this->withSquare(sqObj).size() != 0 && (sqObj->D_CMx != 0 || sqObj->D_CMy != 0))
    {
        collidingSquares = this->withSquare(sqObj);
        v = std::sqrt(sqObj->D_CMx * sqObj->D_CMx + sqObj->D_CMy + sqObj->D_CMy);
        theta = std::atan2(sqObj->D_CMy, sqObj->D_CMx);
        v -= 1;
        if (std::abs(v) < 1)
        {
            sqObj->setSpdX(0);
            sqObj->setSpdY(0);
        }
        else
        {
            sqObj->setSpdX(v * std::cos(theta));
            sqObj->setSpdY(v * std::sin(theta));
        }
    }

    return collidingSquares;
}

// Find separating line using SAT and see which point(s) are very close to that line
float GlfwCollision::pointOfCollision(GlfwSquare *square1, std::vector<GlfwSquare *> squareOthers)
{
    bool collision = true; // Algorithm "assumes" for collision initially
    Coords coords1;
    Coords coords2;

    Vector2d *vectorHolder1;
    Vector2d *vectorHolder2;
    Vector2d *vectorHolder3;
    Vector2d *vectorHolder4;

    GlfwSquare *sq1, *sq2;
    float theta, d1, d2, q1, q2;
    std::vector<float> d;
    std::vector<float> q;

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
                vectorHolder1 = &coords1.at(j);
                vectorHolder2 = &coords1.at(j + 1);
                d.emplace_back(coords1.at(j).dot(P));
                d.emplace_back(coords1.at(j + 1).dot(P));
                if (d.at(1) < d.at(0))
                {
                    d.at(0) = d.at(1);
                    d.at(1) = coords1.at(j).dot(P);
                    vectorHolder1 = vectorHolder2;
                    vectorHolder2 = &coords1.at(j);
                }
                /* Second square */
                // Find min-max projections from second square
                vectorHolder3 = &coords2.at(0);
                vectorHolder4 = &coords2.at(0);
                q1 = coords2.at(0).dot(P);
                q2 = coords2.at(0).dot(P);
                for (int k = 1; k < 4; k++)
                {
                    if (coords2.at(k).dot(P) < q1)
                    {
                        q1 = coords2.at(k).dot(P);
                        vectorHolder3 = &coords2.at(k);
                    }
                    if (coords2.at(k).dot(P) > q2)
                    {
                        q2 = coords2.at(k).dot(P);
                        vectorHolder4 = &coords2.at(k);
                    }
                }
                // Check if there's overlap on the projected 1D line
                if (q1 >= d.at(0) && q1 <= d.at(1) || q2 >= d.at(0) && q2 <= d.at(1) ||
                    d.at(0) >= q1 && d.at(0) <= q2 || d.at(1) >= q1 && d.at(1) <= q2)
                {
                    // Collision happens in this projection
                    // do something?
                }
                else
                {
                    // Collision is not happening with these squares
                    // This should only happen for one projection in _almost_ all cases
                    // since we know collision has happened already.
                    //std::cout << "x1: " << coords1.at(j).x << " and y1: " << coords1.at(j).y << "\n";
                    //std::cout << "x2: " << coords1.at(j + 1).x << " and y2: " << coords1.at(j + 1).y << "\n";
                    //std::cout << "x3: " << vectorHolder1->x << " and y3: " << vectorHolder1->y << "\n";
                    //std::cout << "x4: " << vectorHolder2->x << " and y4: " << vectorHolder2->y << "\n";

                    //ASDASD make better
                    // TODO: Abstractify following smallest distance algorithm
                    std::vector<Vector2d *> points;
                    float dist, dist1, dist2;
                    dist1 = std::abs(d.at(0) - q1);
                    dist2 = std::abs(d.at(0) - q2);
                    if (dist1 < dist2)
                    {
                        dist = dist1;
                        points.emplace_back(vectorHolder1);
                        points.emplace_back(vectorHolder3);
                    }
                    else
                    {
                        dist = dist2;
                        points.emplace_back(vectorHolder2);
                        points.emplace_back(vectorHolder4);
                    }
                    dist1 = std::abs(d.at(1) - q1);
                    dist2 = std::abs(d.at(1) - q2);
                    if (dist1 < dist)
                    {
                        points.at(0) = vectorHolder1;
                        points.at(1) = vectorHolder3;
                    }
                    else if (dist2 < dist)
                    {
                        points.at(0) = vectorHolder2;
                        points.at(1) = vectorHolder4;
                    }

                    gameControl->createObject(DebugCircle(points.at(1)->x,points.at(1)->y,10));
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

    return 0;
}