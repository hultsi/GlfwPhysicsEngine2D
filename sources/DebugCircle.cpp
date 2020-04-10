#include "./../headers/GlfwHeaders.h"
#include <cmath>
#include <iostream>

DebugCircle::DebugCircle(float x, float y, float radius, std::string name)
{
    this->name = name;

    float theta = 2 * M_PI / 16;
    for (int i = 0; i < 32; i += 4)
    {
        lineVertices[i] = x + radius * std::cos((i / 2) * theta);
        lineVertices[i + 1] = y + radius * std::sin((i / 2) * theta);

        lineVertices[i + 2] = x + radius * std::cos((i / 2 + 2) * theta);
        lineVertices[i + 3] = y + radius * std::sin((i / 2 + 2) * theta);
    }
    this->x = x;
    this->y = y;
    this->radius = radius;
}

void DebugCircle::draw()
{
    glColor3f(1.0f, 0.0f, 0.0f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, lineVertices);
    glDrawArrays(GL_LINES, 0, 16);
    glDisableClientState(GL_VERTEX_ARRAY);
}
