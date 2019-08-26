#include "./../headers/GlfwHeaders.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <array>

GlfwSquare::GlfwSquare(float bottomLeftX, float bottomLeftY, float rectWidth, float rectHeight,
                       double rectRotation, bool isStatic, float mass)
{
    x1 = bottomLeftX;              // these are cm --> convert to meters when used
    y1 = bottomLeftY;              // these are cm --> convert to meters when used
    x3 = bottomLeftX + rectWidth;  // these are cm --> convert to meters when used
    y3 = bottomLeftY + rectHeight; // these are cm --> convert to meters when used

    x2 = x3;
    y2 = y1;
    x4 = x1;
    y4 = y3;

    width = x3 - x1;  // these are cm --> convert to meters when used
    height = y3 - y1; // these are cm --> convert to meters when used

    radius = std::sqrt(width * width / 4 + height * height / 4);
    largeCenterAngle = 2 * std::acos(height / 2 / radius);
    smallCenterAngle = M_PI - largeCenterAngle;

    CMx = x1 + width / 2;
    CMy = y1 + height / 2;

    CMFx = 0;
    CMFy = 0;

    D_CMx = 0;
    D_CMy = 0;

    DD_CMx = 0;
    DD_CMy = 0;

    rotation = 0;
    D_rotation = 0;
    DD_rotation = 0;

    rotate(rectRotation);

    mass = mass;
    if (!isStatic)
    {
        applyForce = 1;
        // Add md^2 to moment of inertia to gain the inertia over some other axis than CoM
        // m = mass & d = distance between the axes
        // Multiply with 0.0001? 0.01^2 = conversion from cm to meters
        inertia = mass * (height * height + width * width) / 12;
    }
};

Coords GlfwSquare::getCoordinates(bool addVelocity)
{
    float velX = 0, velY = 0;
    if (addVelocity)
    {
        velX = D_CMx;
        velY = D_CMy;
    }
    Coords coordVec;
    Vector2d vec;
    vec.x = x1 + velX;
    vec.y = y1 + velY;
    coordVec.insert(coordVec.end(), vec);
    vec.x = x2 + velX;
    vec.y = y2 + velY;
    coordVec.insert(coordVec.end(), vec);
    vec.x = x3 + velX;
    vec.y = y3 + velY;
    coordVec.insert(coordVec.end(), vec);
    vec.x = x4 + velX;
    vec.y = y4 + velY;
    coordVec.insert(coordVec.end(), vec);
    return coordVec;
}

void GlfwSquare::move(float xInc, float yInc)
{
    CMx += xInc;
    CMy += yInc;

    x1 = CMx + radius * std::cos(rotation + largeCenterAngle + smallCenterAngle * 3 / 2);
    y1 = CMy + radius * std::sin(rotation + largeCenterAngle + smallCenterAngle * 3 / 2);

    x2 = CMx + radius * std::cos(rotation + 2 * largeCenterAngle + smallCenterAngle * 3 / 2);
    y2 = CMy + radius * std::sin(rotation + 2 * largeCenterAngle + smallCenterAngle * 3 / 2);

    x3 = CMx + radius * std::cos(rotation + smallCenterAngle / 2);
    y3 = CMy + radius * std::sin(rotation + smallCenterAngle / 2);

    x4 = CMx + radius * std::cos(rotation + largeCenterAngle + smallCenterAngle / 2);
    y4 = CMy + radius * std::sin(rotation + largeCenterAngle + smallCenterAngle / 2);
}

void GlfwSquare::rotate(float rad)
{
    rotation += rad;
}

//Happens BEFORE draw() and during every loop
void GlfwSquare::update(double dt)
{
    if (applyForce) {
        updateImpulse(dt);
        updateForces(dt);
        updateAcceleration(dt);
        updateVelocity(dt);
    }
    handleCollision();
    updatePosition(dt);
}
//Happens AFTER update() and during every loop
void GlfwSquare::draw()
{
    GLfloat lineVertices[] = {
        x1, y1,
        x2, y2,
        x2, y2,
        x3, y3,
        x3, y3,
        x4, y4,
        x4, y4,
        x1, y1};

    glColor3f(1.0f, 1.0f, 1.0f);
    if (collision)
        glColor3f(1.0f, 0.0f, 0.0f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, lineVertices);
    glDrawArrays(GL_LINES, 0, 8);
    glDisableClientState(GL_VERTEX_ARRAY);
}

/**
 * Private
 */
float GlfwSquare::distanceFromCM(float &x, float &y)
{
    return std::sqrt((CMx - x) * (CMx - x) + (CMy - y) * (CMy - y));
}

void GlfwSquare::updateImpulse(double dt)
{

}

void GlfwSquare::updateForces(double dt)
{
    CMFx = 0;

    CMFy = 0;
}

void GlfwSquare::updateAcceleration(double dt)
{
    DD_CMx = CMFx / mass;
    DD_CMy = CMFy / mass;
}

void GlfwSquare::updateVelocity(double dt)
{
    D_CMx += DD_CMx;
    D_CMy += DD_CMy;
}

void GlfwSquare::updateAngAcceleration(double dt)
{
    D_rotation = 0;
}

void GlfwSquare::updateAngVelocity(double dt)
{
    D_rotation = 0;
}

void GlfwSquare::updatePosition(double dt)
{
    CMx += D_CMx;
    CMy += D_CMy;
    rotation += D_rotation;

    move();
}

void GlfwSquare::pointCollisionControl(GlfwCollision *collisionObj)
{
    glfwCollision = collisionObj;
}

void GlfwSquare::handleCollision()
{
    if (glfwCollision->withSquare(this).size() != 0)
    {
        collision = true;
        std::vector<GlfwSquare *> squares = glfwCollision->preventPenetration(this);
        std::vector<Vector2d> points = glfwCollision->pointsOfCollision(this, squares);
    }
    else
    {
        collision = false;
    }
}

void GlfwSquare::collisionHandler()
{
    
}

void GlfwSquare::setSpdX(double spd, bool increase)
{
    if (increase)
    {
        D_CMx += spd;
    }
    else
    {
        D_CMx = spd;
    }
}

void GlfwSquare::setSpdY(double spd, bool increase)
{
    if (increase)
    {
        D_CMy += spd;
    }
    else
    {
        D_CMy = spd;
    }
}
