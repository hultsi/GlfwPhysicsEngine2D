#pragma once
#include "./../headers/GlfwHeaders.h"

class DebugCircle
{
public:
    float x = 0;
    float y = 0;
    float radius = 0;

    GLfloat lineVertices[32];

    DebugCircle(float x, float y, float radius);

    void draw();
};
