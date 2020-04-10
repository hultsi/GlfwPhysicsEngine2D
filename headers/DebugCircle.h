#pragma once
#include "./../headers/GlfwHeaders.h"
#include <string>

class DebugCircle
{
public:
    float x = 0;
    float y = 0;
    float radius = 0;
    std::string name;

    GLfloat lineVertices[32];

    DebugCircle(float x, float y, float radius, std::string name = "");

    void draw();
};
