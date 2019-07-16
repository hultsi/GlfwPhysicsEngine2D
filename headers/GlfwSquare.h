#pragma once
#include "./../headers/GlfwHeaders.h"
#include <vector>
#include <array>

class GlfwCollision;

class GlfwSquare
{
public:
    GlfwSquare(){};
    GlfwSquare(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY,
               double rotation = 0, bool isStatic = true, float mass = 999);

    float getWidth();
    float getHeight();
    Coords getCoordinates();

    void update(double SPF = 1); // Seconds Per Frame
    void draw();

    void move(float x, float y);
    void rotate(float rad);

    void pointCollisionControl(GlfwCollision *collisionObj);

private:
    void updateForces(double SPF = 1);
    void updateAcceleration();
    void updateVelocity();
    void updatePosition();

    float distanceFromCM(float &x, float &y);

    //TODO maybe: Change coords to std::vector or std::array
    float x1, y1, x2, y2, x3, y3, x4, y4,
        width, height, radius, mass,
        largeCenterAngle, smallCenterAngle;
    double CMx, CMy, CMFx, CMFy, D_CMx, D_CMy, DD_CMx, DD_CMy, rotation, D_rotation;
    int applyForce;
    bool collision = false;

    GlfwCollision *glfwCollision;
};