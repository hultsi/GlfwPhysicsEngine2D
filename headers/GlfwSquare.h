#pragma once
#include "./../headers/GlfwHeaders.h"
#include <vector>
#include <array>

class GlfwCollision;

class GlfwSquare
{
public:
    //TODO maybe: Change coords to std::vector or std::array
    //TODO: Change to public
    float x1, y1, x2, y2, x3, y3, x4, y4;
    float width, height;
    float radius;
    float mass;
    float largeCenterAngle, smallCenterAngle;
    float impulse;
    double CMx, CMy, CMFx, CMFy;
    double D_CMx, D_CMy;
    double DD_CMx, DD_CMy;
    double rotation, D_rotation, DD_rotation;
    double inertia = 0, momentum = 0, KE = 0, PE = 0;
    int applyForce = 0;
    bool collision = false;

    GlfwSquare(){};
    GlfwSquare(float topLeftX, float topLeftY, float rectWidth, float rectHeight,
               double rectRotation = 0, bool isStatic = true, float mass = 999);

    void setSpdX(double spd, bool increase = false);
    void setSpdY(double spd, bool increase = false);

    Coords getCoordinates(bool addVelocity = false);

    void update(double SPF = 1); // Seconds Per Frame
    void draw();

    void move(float xInc = 0, float yInc = 0);
    void rotate(float rad);

    void pointCollisionControl(GlfwCollision *collisionObj); // Used to assign GlfwCollision object pointer

private:
    void updateImpulse(double dt);
    void updateForces(double dt);
    void updateAcceleration(double dt);
    void updateVelocity(double dt);
    void updateAngAcceleration(double dt);
    void updateAngVelocity(double dt);
    void updatePosition(double dt);
    void handleCollision();

    void collisionHandler();

    float distanceFromCM(float &x, float &y);

    GlfwCollision *glfwCollision;
};