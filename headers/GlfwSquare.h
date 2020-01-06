#pragma once
#include "./../headers/GlfwHeaders.h"
#include <vector>
#include <array>

class GlfwCollision;
class GlfwGameControl;

class GlfwSquare
{
public:
    //TODO maybe: Change coords to std::vector or std::array
    //TODO: Change to public
    float x1, y1, x2, y2, x3, y3, x4, y4;
    float width, height;
    float radius;
    float mass, invMass;
    float largeCenterAngle, smallCenterAngle;
    float impulse;
    Vector2d position;
    Vector2d velocity;
    Vector2d acceleration;
    Vector2d force;
    Vector2d previousVelocity;
    double rotation, D_rotation, DD_rotation;
    double inertia = 0, momentum = 0, KE = 0, PE = 0;
    int applyForce = 0;
    bool collision = false;

    GlfwSquare(){};
    GlfwSquare(float topLeftX, float topLeftY, float rectWidth, float rectHeight,
               double rectRotation = 0, bool isStatic = true, float mass = 999);

    Coords getCoordinates(bool addVelocity = false);

    void update(double SPF = 1); // Seconds Per Frame
    void draw();

    void move(float xInc = 0, float yInc = 0);
    void rotate(float rad);

    void pointCollisionControl(GlfwCollision *collisionObj); // Used to assign GlfwCollision object pointer

    void setGameControl(GlfwGameControl *gameControl);

private:
    void calculateImpulse(std::vector<GlfwSquare *> squares, std::vector<Vector2d> points);
    void updateForces(double dt);
    void updateAcceleration(double dt);
    void updateVelocity(double dt);
    void updateAngAcceleration(double dt);
    void updateAngVelocity(double dt);
    void updatePosition(double dt);
    void handleCollision();

    void collisionHandler();

    GlfwGameControl *gameControl;
    GlfwCollision *glfwCollision;
};