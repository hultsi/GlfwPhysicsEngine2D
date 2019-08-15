#pragma once
#include "./../headers/GlfwHeaders.h"

class DebugCircle {
    public:
        float x, y, radius;

        GLfloat lineVertices[32];
        
        DebugCircle(float x, float y, float radius);

        void draw();
};
