#include "./../headers/GlfwHeaders.h"

GlfwForces::GlfwForces()
{
}

float GlfwForces::gravity(float mass, float gravity)
{
    return mass * gravity;
}