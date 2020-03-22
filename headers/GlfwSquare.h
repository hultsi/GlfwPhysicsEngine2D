#pragma once
#include "./../headers/GlfwHeaders.h"
#include <vector>
#include <array>
#include <unordered_map>

class GlfwCollision;
class GlfwGameControl;

class GlfwSquare
{
public:
    std::array<Vector2d, 4> corners;
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
    std::unordered_map<GlfwSquare *, int> collisionWith; // -1 Not checked, 0 false, 1 true
    std::unordered_map<GlfwSquare *, std::vector<Vector2d>> collidingPoints;

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