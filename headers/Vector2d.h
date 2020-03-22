#pragma once
#include "./../headers/GlfwHeaders.h"
#include <math.h>

class Vector2d
{
public:
    Vector2d();
    Vector2d(float x, float y);

    Vector2d operator+(const Vector2d &param);
    Vector2d operator-(const Vector2d &param);
    template <class T>
    Vector2d operator*(const T param)
    {
        Vector2d newVec;
        newVec.x = param * this->x;
        newVec.y = param * this->y;
        return newVec;
    }
    Vector2d operator*(const Vector2d &param);
    bool operator==(const Vector2d &param);
    bool operator!=(const Vector2d &param);

    float dot(const Vector2d &param) const;
    Vector2d cross(const Vector2d &param) const;

    void rotate(double radians);
    void normalize();
    float length() const;
    float angle() const;
    float angleDeg() const;

    float x, y;

private:
};
