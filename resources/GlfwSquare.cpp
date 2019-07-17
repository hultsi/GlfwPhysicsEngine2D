#include "./../headers/GlfwHeaders.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <array>

GlfwSquare::GlfwSquare(float bottomLeftX, float bottomLeftY, float width, float height,
                       double rotation, bool isStatic, float mass)
{
    this->x1 = bottomLeftX;          // these are cm --> convert to meters when used
    this->y1 = bottomLeftY;          // these are cm --> convert to meters when used
    this->x3 = bottomLeftX + width;  // these are cm --> convert to meters when used
    this->y3 = bottomLeftY + height; // these are cm --> convert to meters when used

    this->x2 = this->x3;
    this->y2 = this->y1;
    this->x4 = this->x1;
    this->y4 = this->y3;

    this->width = x3 - x1;  // these are cm --> convert to meters when used
    this->height = y3 - y1; // these are cm --> convert to meters when used
    this->rotation = 0;

    this->radius = std::sqrt(this->width * this->width / 4 + this->height * this->height / 4);
    this->largeCenterAngle = 2 * std::acos(this->height / 2 / this->radius);
    this->smallCenterAngle = M_PI - this->largeCenterAngle;

    this->CMx = this->x1 + this->width / 2;
    this->CMy = this->y1 + this->height / 2;

    this->CMFx = 0;
    this->CMFy = 0;

    this->D_CMx = 0;
    this->D_CMy = 0;

    this->DD_CMx = 0;
    this->DD_CMy = 0;

    this->rotate(rotation);

    this->mass = mass;
    if (!isStatic)
    {
        this->applyForce = 1;
        // Add md^2 to moment of inertia to gain the inertia over some other axis than CoM
        // m = mass & d = distance between the axes
        // 0.0001 is = 0.01^2 = conversion from cm to meters
        this->momentOfInertia = 0.0001 * mass * (height * height + width * width) / 12;
    }
};

float GlfwSquare::getWidth()
{
    return this->width;
}

float GlfwSquare::getHeight()
{
    return this->height;
}

Coords GlfwSquare::getCoordinates(bool addVelocity)
{
    float velX = 0, velY = 0;
    if (addVelocity)
    {
        velX = this->D_CMx;
        velY = this->D_CMy;
    }
    Coords coordVec;
    Vector2d vec;
    vec.x = this->x1 + velX;
    vec.y = this->y1 + velY;
    coordVec.insert(coordVec.end(), vec);
    vec.x = this->x2 + velX;
    vec.y = this->y2 + velY;
    coordVec.insert(coordVec.end(), vec);
    vec.x = this->x3 + velX;
    vec.y = this->y3 + velY;
    coordVec.insert(coordVec.end(), vec);
    vec.x = this->x4 + velX;
    vec.y = this->y4 + velY;
    coordVec.insert(coordVec.end(), vec);
    return coordVec;
}

void GlfwSquare::move(float xInc, float yInc)
{
    this->CMx += xInc;
    this->CMy += yInc;

    x1 = this->CMx + this->radius * std::cos(this->rotation + this->largeCenterAngle + this->smallCenterAngle * 3 / 2);
    y1 = this->CMy + this->radius * std::sin(this->rotation + this->largeCenterAngle + this->smallCenterAngle * 3 / 2);

    x2 = this->CMx + this->radius * std::cos(this->rotation + 2 * this->largeCenterAngle + this->smallCenterAngle * 3 / 2);
    y2 = this->CMy + this->radius * std::sin(this->rotation + 2 * this->largeCenterAngle + this->smallCenterAngle * 3 / 2);

    x3 = this->CMx + this->radius * std::cos(this->rotation + this->smallCenterAngle / 2);
    y3 = this->CMy + this->radius * std::sin(this->rotation + this->smallCenterAngle / 2);

    x4 = this->CMx + this->radius * std::cos(this->rotation + this->largeCenterAngle + this->smallCenterAngle / 2);
    y4 = this->CMy + this->radius * std::sin(this->rotation + this->largeCenterAngle + this->smallCenterAngle / 2);
}

void GlfwSquare::rotate(float rad)
{
    this->rotation += rad;
    this->move();
}

//Happens BEFORE draw() and during every loop
void GlfwSquare::update(double SPF)
{
    this->updateForces(SPF);
    this->updateAcceleration();
    this->updateVelocity();
    this->handleCollision();
    this->updatePosition();
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
    return std::sqrt((this->CMx - x) * (this->CMx - x) + (this->CMy - y) * (this->CMy - y));
}

void GlfwSquare::updateForces(double SPF)
{
    this->CMFx = 0;
    this->CMFx *= (this->applyForce) / 30;

    this->CMFy = 0 * (this->applyForce) / 30;
}

void GlfwSquare::updateAcceleration()
{
    this->DD_CMx = this->CMFx / this->mass;
    this->DD_CMy = this->CMFy / this->mass;
}

void GlfwSquare::updateVelocity()
{
    this->D_CMx += DD_CMx;
    this->D_CMy += DD_CMy;
}

void GlfwSquare::updatePosition()
{
    this->CMx += D_CMx;
    this->CMy += D_CMy;
    //TODO: DO BETTER
    this->move();
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
    }
    else
    {
        collision = false;
    }
}