#include "./../headers/GlfwHeaders.h"
#include <cmath>
#include <iostream>

GlfwSquare::GlfwSquare(float bottomLeftX, float bottomLeftY, float width, float height,
                       bool isStatic, float mass)
{
    this->x1 = bottomLeftX;          // 2 * bottomLeftX / W_WIDTH - 1;
    this->y1 = bottomLeftY;          // 2 * bottomLeftY / W_HEIGHT - 1;
    this->x3 = bottomLeftX + width;  // 2 * (bottomLeftX + width) / W_WIDTH - 1;
    this->y3 = bottomLeftY + height; // 2 * (bottomLeftY + height) / W_HEIGHT - 1;

    this->x2 = this->x3;
    this->y2 = this->y1;
    this->x4 = this->x1;
    this->y4 = this->y3;

    this->width = x3 - x1;  // (topRightX - bottomLeftX);  // * W_WIDTH;
    this->height = y3 - y1; // (topRightY - bottomLeftY); // * W_HEIGHT;
    this->rotation = 0;

    this->radius = std::sqrt(this->width * this->width / 4 + this->height * this->height / 4);
    this->largeCenterAngle = 2 * std::acos(this->height / 2 / this->radius);
    this->smallCenterAngle = M_PI - this->largeCenterAngle;

    this->mass = mass;

    (isStatic ? this->applyForce = 0 : this->applyForce = 1);

    this->CMx = this->x1 + this->width / 2;
    this->CMy = this->y1 + this->height / 2;

    this->CMFx = 0;
    this->CMFy = 0;
};

float GlfwSquare::getWidth()
{
    return this->width;
}

float GlfwSquare::getHeight()
{
    return this->height;
}

void GlfwSquare::rotate(float rad)
{
    this->rotation += rad;

    x1 = CMx + this->radius * std::cos(this->rotation + this->largeCenterAngle + this->smallCenterAngle * 3 / 2);
    y1 = CMy + this->radius * std::sin(this->rotation + this->largeCenterAngle + this->smallCenterAngle * 3 / 2);

    x2 = CMx + this->radius * std::cos(this->rotation + 2 * this->largeCenterAngle + this->smallCenterAngle * 3 / 2);
    y2 = CMy + this->radius * std::sin(this->rotation + 2 * this->largeCenterAngle + this->smallCenterAngle * 3 / 2);

    x3 = CMx + this->radius * std::cos(this->rotation + this->smallCenterAngle / 2);
    y3 = CMy + this->radius * std::sin(this->rotation + this->smallCenterAngle / 2);

    x4 = CMx + this->radius * std::cos(this->rotation + this->largeCenterAngle + this->smallCenterAngle / 2);
    y4 = CMy + this->radius * std::sin(this->rotation + this->largeCenterAngle + this->smallCenterAngle / 2);
}

//Happens BEFORE draw() and during every loop
void GlfwSquare::update()
{
    this->updateForces();
    this->updateAcceleration();
    //this->updateVelocity();
    //this->updatePosition();
    this->rotate(0.01);
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

void GlfwSquare::updateForces()
{
    this->CMFy = -GlfwForces::gravity(GRAVITY, this->mass);

    this->CMFy *= this->applyForce;
}

void GlfwSquare::updateAcceleration()
{
    this->DD_CMx = this->CMFx / this->mass;
    this->DD_CMy = this->CMFy / this->mass;
}

void GlfwSquare::updateVelocity()
{
    this->D_CMx = DD_CMx;
    this->D_CMy = DD_CMy;
    //std::cout << D_CMy << std::endl;
}
/*
void GlfwSquare::updatePosition()
{
    this->CMx += D_CMx;
    this->CMy += D_CMy;
    //TODO: DO BETTER
    this->rotate(0);
}
*/