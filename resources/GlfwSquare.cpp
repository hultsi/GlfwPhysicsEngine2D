#include "./../headers/glHeaders.h"
#include <cmath>
#include <iostream>

GlfwSquare::GlfwSquare(float bottomLeftX, float bottomLeftY, float width, float height,
                       bool isStatic)
//: x1(bottomLeftX), y1(bottomLeftY), x3(bottomLeftX + width), y3(bottomLeftY + height)
{
    this->x1 = 2 * bottomLeftX / W_WIDTH - 1;
    this->y1 = 2 * bottomLeftY / W_HEIGHT - 1;
    this->x3 = 2 * (bottomLeftX + width) / W_WIDTH - 1;
    this->y3 = 2 * (bottomLeftY + height) / W_HEIGHT - 1;

    this->width = width;   // (topRightX - bottomLeftX);  // * W_WIDTH;
    this->height = height; // (topRightY - bottomLeftY); // * W_HEIGHT;
    this->rotation = 0;

    this->radius = std::sqrt(this->width * this->width / 4 + this->height * this->height / 4);
    this->largeCenterAngle = 2 * std::acos(this->height / 2 / this->radius);
    this->smallCenterAngle = M_PI - this->largeCenterAngle;

    this->CMx = (this->x1 + this->x3) / 2;
    this->CMy = (this->y1 + this->y3) / 2;

    this->x2 = this->x3;
    this->y2 = this->y1;
    this->x4 = this->x1;
    this->y4 = this->y3;
};

float GlfwSquare::getWidth()
{
    return this->width;
}

float GlfwSquare::getHeight()
{
    return this->height;
}

void GlfwSquare::move(float x, float y)
{
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

int GlfwSquare::draw()
{
    float x, y;
    glBegin(GL_LINES);

    glVertex2f(x1, y1);
    glVertex2f(x2, y2);

    glVertex2f(x2, y2);
    glVertex2f(x3, y3);

    glVertex2f(x3, y3);
    glVertex2f(x4, y4);

    glVertex2f(x4, y4);
    glVertex2f(x1, y1);

    glEnd();

    return 1;
}

/**
 * Private
 */
float GlfwSquare::distanceFromCM(float &x, float &y)
{
    return std::sqrt((this->CMx - x) * (this->CMx - x) + (this->CMy - y) * (this->CMy - y));
}