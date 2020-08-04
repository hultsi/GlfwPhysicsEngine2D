#include "./../headers/GlfwHeaders.h"

Vector2d::Vector2d()
{
}

Vector2d::Vector2d(float x, float y)
{
    this->x = x;
    this->y = y;
    this->angle = std::atan2(this->y, this->x);
    (this->angle > 0 ? this->angle : (2 * M_PI + this->angle));
}

void Vector2d::rotate(double radians)
{
    this->angle += radians;
    this->x = getLength() * std::cos(this->angle);
    this->y = getLength() * std::sin(this->angle);
    //float xPrev = this->x;
    //this->x = this->x * std::cos(this->angle) - this->y * std::sin(this->angle);
    //this->y = xPrev * std::sin(this->angle) + this->y * std::cos(this->angle);
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

void Vector2d::setLength(float len)
{
    float angle = getAngle();
    float speed = len;

    this->x = speed * std::cos(angle);
    this->y = speed * std::sin(angle);
}

float Vector2d::getLength() const
{
    float val;
    val = std::sqrt(this->x * this->x + this->y * this->y);
    return val;
}

float Vector2d::getAngle() const
{
    return this->angle;
}

float Vector2d::getAngleDeg() const
{
    return this->angle * 360 / (2 * M_PI);
}

float Vector2d::cross(const Vector2d &param) const
{
    float out;
    out = this->x * param.y - this->y * param.x;
    return out;
}

/*
Vector2d Vector2d::cross(const Vector2d &param) const
{
    Vector2d vec;
    vec.x = this->x * param.y;
    vec.y = -this->y * param.x;
    return vec;
}
*/