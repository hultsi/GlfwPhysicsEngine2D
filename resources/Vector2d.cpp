#include "./../headers/GlfwHeaders.h"

Vector2d::Vector2d()
{
}

Vector2d::Vector2d(float x, float y)
{
    this->x = x;
    this->y = y;
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

Vector2d Vector2d::cross(const Vector2d &param) const
{
    Vector2d vec;
    vec.x = this->x * param.y;
    vec.y = -this->y * param.x;
    return vec;
}