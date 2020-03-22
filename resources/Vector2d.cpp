#include "./../headers/GlfwHeaders.h"

Vector2d::Vector2d()
{
}

Vector2d::Vector2d(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Vector2d::rotate(double radians)
{
    float xPrev = x;
    x = x * std::cos(radians) - y * std::sin(radians);
    y = xPrev * std::sin(radians) + y * std::cos(radians);
}

void Vector2d::normalize()
{
    x = x / std::sqrt(x * x + y * y);
    y = y / std::sqrt(x * x + y * y);
}

Vector2d Vector2d::operator+(const Vector2d &param)
{
    Vector2d newVec;
    newVec.x = this->x + param.x;
    newVec.y = this->y + param.y;
    return newVec;
}

Vector2d Vector2d::operator-(const Vector2d &param)
{
    Vector2d newVec;
    newVec.x = this->x - param.x;
    newVec.y = this->y - param.y;
    return newVec;
}

Vector2d Vector2d::operator*(const Vector2d &param)
{
    Vector2d newVec;
    newVec.x = this->x * param.x;
    newVec.y = this->y * param.y;
    return newVec;
}

bool Vector2d::operator==(const Vector2d &param)
{
    if (this->x == param.x && this->y == param.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Vector2d::operator!=(const Vector2d &param)
{
    if (this->x == param.x && this->y == param.y)
    {
        return false;
    }
    else
    {
        return true;
    }
}

float Vector2d::dot(const Vector2d &param) const
{
    float val;
    val = this->x * param.x + this->y * param.y;
    return val;
}

float Vector2d::length() const
{
    float val;
    val = std::sqrt(this->x * this->x + this->y * this->y);
    return val;
}

float Vector2d::angle() const
{
    float val;
    val = std::atan2(this->y, this->x);
    return (val > 0 ? val : (2 * M_PI + val));
}

float Vector2d::angleDeg() const
{
    float val;
    val = std::atan2(this->y, this->x);
    return (val > 0 ? val : (2 * M_PI + val)) * 360 / (2 * M_PI);
}

Vector2d Vector2d::cross(const Vector2d &param) const
{
    Vector2d vec;
    vec.x = this->x * param.y;
    vec.y = -this->y * param.x;
    return vec;
}