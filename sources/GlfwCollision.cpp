#include "./../headers/GlfwHeaders.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <array>
#include <string>

GlfwCollision::GlfwCollision() {}

GlfwCollision::GlfwCollision(GlfwGameControl *gameControl)
{
    this->gameControl = gameControl;
}

//Uses Separating Axis Theorem (SAT)
std::unordered_map<std::string, GlfwSquare *> GlfwCollision::withConvex(GlfwSquare *sqObj, std::unordered_map<std::string, GlfwSquare *> &colliders, bool withVelocity)
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
            coords1 = sq1->getCoordinates(withVelocity);
            coords2 = sq2->getCoordinates(withVelocity);
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
std::unordered_map<std::string, GlfwSquare *> GlfwCollision::preventPenetration(GlfwSquare *sqObj, std::unordered_map<std::string, GlfwSquare *> &colliders, bool withVelocity)
{
    std::unordered_map<std::string, GlfwSquare *> collidersOut;
    std::unordered_map<std::string, GlfwSquare *> colliders3;

    while (colliders.size() > 0)
    {
        sqObj->collidingObjects = colliders;

        sqObj->position = decreasePosition(sqObj->position, sqObj->previousPosition, sqObj->velocity);
        sqObj->rotation = decreaseAngle(sqObj->rotation, sqObj->previousRotation, sqObj->D_rotation);
        for (auto const &[key, colliderRect] : colliders)
        {
            colliderRect->position = decreasePosition(colliderRect->position, colliderRect->previousPosition, colliderRect->velocity);
            colliderRect->rotation = decreaseAngle(colliderRect->rotation, colliderRect->previousRotation, colliderRect->D_rotation);
            colliders3 = withConvex(colliderRect, gameControl->rectAll, withVelocity);
            preventPenetration(colliderRect, colliders3, withVelocity);
        }
        colliders = withConvex(sqObj, colliders, withVelocity);
    }
    return collidersOut;
}

void GlfwCollision::pointsOfCollision(GlfwSquare *sqObj, std::unordered_map<std::string, GlfwSquare *> &colliders, bool withVelocity)
{
    Coords coords1;
    std::vector<float> proj1;
    std::vector<int> sorted1;
    Coords coords2;
    std::vector<float> proj2;
    std::vector<int> sorted2;

    float coeff;
    Vector2d normal(0, 0);
    std::unordered_map<std::string, std::vector<Vector2d>> collisionPoints;
    std::vector<Vector2d> coords;

    bool proj1OnLeft = false;
    bool collisionFound = false;
    // Temp pointers for colliders
    GlfwSquare *sq1, *sq2, *tmpSq;
    // Used for projections
    float theta, d1, d2, q1, q2;
    // Used for checking if collision point is touching a side
    float w1, w2, w3;
    int ind1, ind2;

    Vector2d P;
    for (auto const &[key, val] : colliders)
    {
        collisionFound = false;
        collisionPoints.clear();
        collisionPoints.emplace("point", NULL);
        collisionPoints.emplace("normal", NULL);
        sq1 = sqObj;
        sq2 = colliders.at(key);
        for (int n = 0; n < 2; ++n) // loop over both shapes to get all necessary projections
        {
            if (sqObj == colliders.at(key)) // Don't compare if the objects are the same
                break;
            if (sqObj->collisionPoints.find(val) != sqObj->collisionPoints.end()) // Don't compare if the pair has been checked already
                break;

            if (n > 0) // Switch rects for 2nd loop to get all necessary projections
            {
                sq1 = colliders.at(key);
                sq2 = sqObj;
            }

            coords1 = sq1->getCoordinates(withVelocity);
            coords2 = sq2->getCoordinates(withVelocity);
            for (int j = 0; j < coords1.size() / 2; ++j) // Loop over half of the points that make the rect
            {
                P = getProjectionVector(coords1.at(j + 1), coords1.at(j));

                // Find min-max projections from first & 2nd square
                proj1 = getProjections(coords1, P);
                proj2 = getProjections(coords2, P);

                sorted1 = sortProjections(proj1);
                sorted2 = sortProjections(proj2);

                if (std::abs(proj1[sorted1[sorted1.size() - 1]] - proj2[sorted2[0]]) <= 2)
                {
                    proj1OnLeft = true;
                }
                else if (std::abs(proj1[sorted1[0]] - proj2[sorted2[sorted2.size() - 1]]) <= 2)
                {
                    proj1OnLeft = false;
                }
                else
                {
                    continue;
                }

                collisionFound = true;
                coords.clear();

                // Gather points that are nearby each other (on the projection line) to coords
                if (proj1OnLeft)
                {
                    coords.emplace_back(coords1[sorted1[coords1.size() - 1]]);
                    coords.emplace_back(coords2[sorted2[0]]);
                    if (formCollidingLine(proj1[sorted1[coords1.size() - 1]], proj1[sorted1[coords1.size() - 2]]))
                        coords.emplace_back(coords1[sorted1[coords1.size() - 2]]);
                    if (formCollidingLine(proj2[sorted2[0]], proj2[sorted2[1]]))
                        coords.emplace_back(coords2[sorted2[1]]);
                }
                else
                {
                    coords.emplace_back(coords1[sorted1[0]]);
                    coords.emplace_back(coords2[sorted2[coords2.size() - 1]]);
                    if (formCollidingLine(proj1[sorted1[0]], proj1[sorted1[1]]))
                        coords.emplace_back(coords1[sorted1[1]]);
                    if (formCollidingLine(proj2[sorted2[coords2.size() - 1]], proj2[sorted2[coords2.size() - 2]]))
                        coords.emplace_back(coords2[sorted2[coords2.size() - 2]]);
                }

                // Define normal vector
                normal = P;
                normal.normalize();
                if (proj1OnLeft)
                {
                    normal.x = -normal.x;
                    normal.y = -normal.y;
                }
                // Rotate P 90 deg
                P.rotate(M_PI / 2);
                proj1 = getProjections(coords, P);
                sorted1 = sortProjections(proj1);
                for (int i = 1; i < proj1.size() - 1; ++i)
                {
                    collisionPoints.at("point").emplace_back(coords[sorted1[i]]);
                    collisionPoints.at("normal").emplace_back(normal);
                }

                // Save collision points and normals for both rects
                sq1->collisionPoints.emplace(sq2, collisionPoints);
                sq1->collision = true;
                for (int i = 0; i < collisionPoints.at("normal").size(); ++i)
                {
                    collisionPoints.at("normal")[i].x = -collisionPoints.at("normal")[i].x;
                    collisionPoints.at("normal")[i].y = -collisionPoints.at("normal")[i].y;
                }
                sq2->collisionPoints.emplace(sq1, collisionPoints);
                sq2->collision = true;

                if (collisionFound)
                    break;
            }
            if (collisionFound)
                break;
        }
    }
}

bool GlfwCollision::linesIntersect(float x0_1, float y0_1, float x0_2, float y0_2, float x1_1, float y1_1, float x1_2, float y1_2)
{
    return false;
}

Vector2d GlfwCollision::intersectionPoint(float x0_1, float y0_1, float x0_2, float y0_2, float x1_1, float y1_1, float x1_2, float y1_2)
{
    Vector2d vec;

    return vec;
}

void GlfwCollision::calculateImpulse(GlfwSquare *sqObj, std::unordered_map<GlfwSquare *, std::unordered_map<std::string, std::vector<Vector2d>>> collisionPoints, float restitution)
{
    float impulse = 0;
    Vector2d impulseVector(0, 0);
    Vector2d normal(0, 0);

    for (auto const &[key, sq] : collisionPoints)
    {
        normal = sq.at("normal")[0]; // Each normal == same for convex shapes

        impulse = -(1 + restitution) * ((sqObj->velocity - key->velocity).dot(normal)) / (sqObj->invMass + key->invMass);
        impulseVector.x = impulse * normal.x;
        impulseVector.y = impulse * normal.y;

        sqObj->impulseVector = sqObj->impulseVector + impulseVector;
        key->impulseVector = key->impulseVector + impulseVector * -1;

        key->collidingObjects.erase(sqObj->name);

        // These somewhere else?
        sqObj->applyImpulse();
        key->applyImpulse();
    }
}

void GlfwCollision::calculateForceMoment(GlfwSquare *sqObj, std::unordered_map<GlfwSquare *, std::unordered_map<std::string, std::vector<Vector2d>>> collisionPoints)
{
    Vector2d point(0, 0);
    Vector2d radVec(0, 0);
    int pointTotal = 0;
    for (auto const &[key, sq] : collisionPoints)
    {
        pointTotal = sq.at("point").size();
        for (int i = 0; i < pointTotal; ++i)
        {
            point = sq.at("point")[i];
            radVec = point - sqObj->position;

            sqObj->forceMoment += radVec.cross(sqObj->impulseVector) / pointTotal;

            radVec = point - key->position;
            key->forceMoment += radVec.cross(key->impulseVector) / pointTotal;
        }
        key->collisionPoints.erase(sqObj);
    }
}
/**
 * 
 *  PRIVATE 
 * 
 */
bool GlfwCollision::formCollidingLine(float a, float b)
{
    return std::abs(a - b) < 0.1;
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

    float temp1 = 0;
    int temp2 = 0;
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                temp1 = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp1;

                temp2 = sortedInds[j];
                sortedInds[j] = sortedInds[j + 1];
                sortedInds[j + 1] = temp2;
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

float GlfwCollision::decreaseAngularVelocity(float omega, float amount)
{
    if (omega > amount)
        return omega - amount;
    else if (omega < -amount)
        return omega + amount;
    else
        return 0;
}

Vector2d GlfwCollision::decreasePosition(Vector2d &pos, Vector2d &prevPos, Vector2d &vel)
{
    Vector2d out = pos;
    float angle = vel.getAngle();
    float speed = vel.getLength();
    float speedDecX;
    float speedDecY;
    if (speed > 0)
    {
        speedDecX = -vel.x * 1 / speed; // This needs to be optimized
        speedDecY = -vel.y * 1 / speed; // This needs to be optimized
    }
    else
        return prevPos;

    if (std::abs(pos.x - prevPos.x) > std::abs(speedDecX))
        out.x += speedDecX;
    else
        out.x = prevPos.x;

    if (std::abs(pos.y - prevPos.y) > std::abs(speedDecY))
        out.y += speedDecY;
    else
        out.y = prevPos.y;

    return out;
}

float GlfwCollision::decreaseAngle(float angle, float prevAngle, float omega)
{
    // Increase 0.1 if you see weird "lagging"
    if (omega > 0.1 && angle > prevAngle)
        return angle - 0.1;
    else if (omega < -0.1 && angle < prevAngle)
        return angle + 0.1;
    else
        return prevAngle;
}

std::unordered_map<std::string, std::vector<Vector2d>> GlfwCollision::getNearbyPoints(std::vector<float> proj1, std::vector<float> proj2)
{
    std::unordered_map<std::string, std::vector<Vector2d>> outMap;
    outMap.emplace("proj_1", NULL);
    outMap.emplace("proj_2", NULL);
    std::vector<int> sortedInds;
    bool proj1OnLeft = false;

    sortedInds = sortProjections(proj1);
    if (proj1[0] - proj2[0] < 0)
        proj1OnLeft = true;

    return outMap;
}