#pragma once
#include "./GlfwHeaders.h"

class GlfwForces
{
public:
    GlfwForces();

    static float gravity(float mass, float gravity);
};