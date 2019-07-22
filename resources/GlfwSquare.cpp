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

    this->rotation = 0;
    this->D_rotation = 0;
    this->DD_rotation = 0;

    this->rotate(rotation);

    this->mass = mass;
    if (!isStatic)
    {
        this->applyForce = 1;
        // Add md^2 to moment of inertia to gain the inertia over some other axis than CoM
        // m = mass & d = distance between the axes
        // 0.0001 is = 0.01^2 = conversion from cm to meters
        this->inertia = 0.0001 * mass * (height * height + width * width) / 12;
    }
};

void GlfwSquare::setVelocity(double xVel, double yVel)
{
    this->D_CMx = xVel;
    this->D_CMy = yVel;
}
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

void GlfwSquare::updateAngAcceleration()
{
    this->D_rotation = 0;
}

void GlfwSquare::updateAngVelocity()
{
    this->D_rotation = 0;
}

void GlfwSquare::updatePosition()
{
    this->CMx += D_CMx;
    this->CMy += D_CMy;
    this->rotation += D_rotation;

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
        std::vector<GlfwSquare *> squares = glfwCollision->preventPenetration(this);
    }
    else
    {
        collision = false;
    }
}

void GlfwSquare::collisionHandler1()
{
    std::vector<GlfwSquare *> squares = glfwCollision->withSquare(this);
    float m1 = this->mass;
    float m2 = squares.at(0)->getMass();
    double v1 = this->D_CMx;
    double v2 = squares.at(0)->getSpdX();
    double v1FinalX = (m1 * v1 - m2 * (v1 - 2 * v2)) / (m1 + m2);
    double D_CMx2 = (m2 * v2 + m1 * (2 * v1 - v2)) / (m1 + m2);
    v1 = this->D_CMy;
    v2 = squares.at(0)->getSpdY();
    double v1FinalY = (m1 * v1 - m2 * (v1 - 2 * v2)) / (m1 + m2);
    double D_CMy2 = (m2 * v2 + m1 * (2 * v1 - v2)) / (m1 + m2);
    while (squares.size() != 0 && D_CMx != 0)
    {
        double v = std::sqrt(D_CMx * D_CMx + D_CMy + D_CMy);
        double theta = std::atan(D_CMy / D_CMx);
        v -= 1;
        D_CMx = v * std::cos(theta);
        D_CMy = v * std::sin(theta);
        squares = glfwCollision->withSquare(this);
        std::cout << D_CMx << std::endl;
        std::cout << D_CMy << std::endl;
    }
    D_CMx = v1FinalX;
    D_CMy = v1FinalY;
    squares.at(0)->setVelocity(D_CMx2, D_CMy2);
}

void GlfwSquare::collisionHandler2()
{
    std::vector<GlfwSquare *> squares = glfwCollision->withSquare(this);
    float m1 = this->mass;
    float m2 = squares.at(0)->getMass();
    double v1 = std::sqrt(this->D_CMx * this->D_CMx + this->D_CMy * this->D_CMy);
    double v2 = std::sqrt(squares.at(0)->getSpdX() * squares.at(0)->getSpdX() +
                          squares.at(0)->getSpdY() * squares.at(0)->getSpdY());
    double omega1 = this->D_rotation;
    double omega2 = squares.at(0)->getAngVelocity();
    double I1 = this->inertia;
    double I2 = squares.at(0)->getInertia();
}

void GlfwSquare::setSpdX(double spd, bool increase)
{
    if (increase)
    {
        this->D_CMx += spd;
    }
    else
    {
        this->D_CMx = spd;
    }
}

void GlfwSquare::setSpdY(double spd, bool increase)
{
    if (increase)
    {
        this->D_CMy += spd;
    }
    else
    {
        this->D_CMy = spd;
    }
}

double GlfwSquare::getSpdX()
{
    return this->D_CMx;
}

double GlfwSquare::getSpdY()
{
    return this->D_CMy;
}

double GlfwSquare::getAngVelocity()
{
    return this->D_rotation;
}

double GlfwSquare::getInertia()
{
    return this->inertia;
}

float GlfwSquare::getMass()
{
    return this->mass;
}