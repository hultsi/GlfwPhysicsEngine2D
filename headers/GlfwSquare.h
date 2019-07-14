#pragma once

class GlfwSquare
{
public:
    GlfwSquare(){};
    GlfwSquare(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY,
               bool isStatic = true, float mass = 999);

    float getWidth();
    float getHeight();

    void update();
    void draw();

    void move(float x, float y);
    void rotate(float rad);

private:
    void updateForces();
    void updateAcceleration();
    void updateVelocity();
    void updatePosition();

    float distanceFromCM(float &x, float &y);

    float x1, y1, x2, y2, x3, y3, x4, y4,
        CMx, CMy, D_CMx, D_CMy, DD_CMx, DD_CMy,
        CMFx, CMFy, rotation, D_rotation,
        width, height, radius, mass,
        largeCenterAngle, smallCenterAngle;
    int applyForce;
};