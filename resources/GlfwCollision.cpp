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

    GlfwSquare *sq1, *sq2;
    float theta;
    std::array<float,2> indSq1; // from min to max projection coordinate
    std::array<float,4> indSq2; // from min to max projection coordinate
    
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
                indSq1[0] = j; //coords1.at(j).dot(P);
                indSq1[1] = j+1; //coords1.at(j + 1).dot(P);
                if (coords1.at(indSq1[1]).dot(P) < coords1.at(indSq1[0]).dot(P))
                {
                    indSq1[0] = j+1;
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
                for (int k = 0; k < 4; ++k) {
                    if (indSq2[0] != k && indSq2[3] != k)
                        indSq2[1] = k;
                }
                for (int k = 0; k < 4; ++k) {
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
                    std::vector<Vector2d> points = collidingPoints(coords1,indSq1,coords2,indSq2,P);
                    //std::cout << points.size() << "\n";
                    for (int i = 0; i < points.size(); ++i) {
                        gameControl->createObject(DebugCircle(points[i].x,points[i].y,10));    
                    }
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

std::vector<Vector2d> GlfwCollision::collidingPoints(const Coords &coords1, const std::array<float,2> indSq1,
                                const Coords &coords2, const std::array<float,4> indSq2, Vector2d P)
{
                    std::vector<Vector2d> points;
                    float dist = 0;
                    int ind = 0;
                    for (int i = 0; i < indSq1.size(); ++i) {
                        for (int k = 0; k < indSq2.size(); ++k) {
                            dist = std::abs(coords1.at(indSq1[i]).dot(P) - coords2.at(indSq2[k]).dot(P));
                            if (dist <= 1) {
                                ind = i;
                                points.emplace_back(coords1[indSq1[i]]);
                                points.emplace_back(coords2[indSq2[k]]);
                            }
                        }   
                    }

                    float theta;
                    if (points.size() > 2) {
                        if (indSq1[ind] > indSq1[1-ind]) {
                            points[2] = coords1[indSq1[ind]+1 % 4];
                            theta = std::atan((coords1.at(indSq1[ind]).y - coords1.at(indSq1[1-ind]).y) / (coords1.at(indSq1[ind]).x - coords1.at(indSq1[1-ind]).x));
                        } else {
                            theta = std::atan((coords1.at(indSq1[1-ind]).y - coords1.at(indSq1[ind]).y) / (coords1.at(indSq1[1-ind]).x - coords1.at(indSq1[ind]).x));
                            points[2] = coords1[indSq1[ind]-1 % 4];
                        }
                        
                        P.x = std::cos(theta+M_PI/2);
                        P.y = std::sin(theta+M_PI/2);

                        Vector2d p;
                        for (int k = 0; k < 3; ++k)
                        {
                            if (points[k].dot(P) > points[k+1].dot(P)) {
                                p = points[k];
                                points[k] = points[k+1];
                                points[k+1] = p;
                                k = 0;
                            }
                        }
                        //points.erase(points.begin());
                        
                    }

                    return points;
};