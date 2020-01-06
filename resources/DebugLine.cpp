#include "./../headers/GlfwHeaders.h"
#include <cmath>
#include <iostream>

DebugLine::DebugLine(float x1, float y1, float x2, float y2)
{
    lineVertices[0] = x1;
    lineVertices[1] = y1;
    lineVertices[2] = x2;
    lineVertices[3] = y2;
}

void DebugLine::draw()
{
    glColor3f(1.0f, 0.0f, 0.0f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, lineVertices);
    glDrawArrays(GL_LINES, 0, 2);
    glDisableClientState(GL_VERTEX_ARRAY);
}
