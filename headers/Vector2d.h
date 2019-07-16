#pragma once
#include "./../headers/GlfwHeaders.h"

class Vector2d
{
public:
    Vector2d();
    Vector2d(float x, float y);

    Vector2d operator+(const Vector2d &param);
    Vector2d operator-(const Vector2d &param);
    bool operator==(const Vector2d &param);
    bool operator!=(const Vector2d &param);

    float dot(Vector2d &param);
    Vector2d cross(Vector2d &param);

    float x, y;

private:
};