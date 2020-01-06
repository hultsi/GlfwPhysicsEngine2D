#pragma once
#include "./../headers/GlfwHeaders.h"
#include <math.h>

class Vector2d
{
public:
    Vector2d();
    Vector2d(float x, float y);

    //TODO: ABSTRACTIFY TO .cpp
    template <class T>
    Vector2d operator*(const T param)
    {
        Vector2d newVec;
        newVec.x = param * this->x;
        newVec.y = param * this->y;
        return newVec;
    }

    Vector2d operator+(const Vector2d &param);
    Vector2d operator-(const Vector2d &param);
    bool operator==(const Vector2d &param);
    bool operator!=(const Vector2d &param);

    float dot(const Vector2d &param) const;
    Vector2d cross(const Vector2d &param) const;

    void rotate(double radians);
    void normalize();

    float x, y;

private:
};