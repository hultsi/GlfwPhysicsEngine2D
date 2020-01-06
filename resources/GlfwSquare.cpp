#include "./../headers/GlfwHeaders.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <array>

GlfwSquare::GlfwSquare(float bottomLeftX, float bottomLeftY, float rectWidth, float rectHeight,
                       double rectRotation, bool isStatic, float mass)
{
    x1 = bottomLeftX;              // these are cm --> convert to meters when used
    y1 = bottomLeftY;              // these are cm --> convert to meters when used
    x3 = bottomLeftX + rectWidth;  // these are cm --> convert to meters when used
    y3 = bottomLeftY + rectHeight; // these are cm --> convert to meters when used

    x2 = x3;
    y2 = y1;
    x4 = x1;
    y4 = y3;

    width = x3 - x1;  // these are cm --> convert to meters when used
    height = y3 - y1; // these are cm --> convert to meters when used

    radius = std::sqrt(width * width / 4 + height * height / 4);
    largeCenterAngle = 2 * std::acos(height / 2 / radius);
    smallCenterAngle = M_PI - largeCenterAngle;

    position.x = x1 + width / 2;
    position.y = y1 + height / 2;

    force.x = 0;
    force.y = 0;

    velocity.x = 0;
    velocity.y = 0;

    acceleration.x = 0;
    acceleration.y = 0;

    rotation = 0;
    D_rotation = 0;
    DD_rotation = 0;

    rotate(rectRotation);

    mass = mass;
    invMass = 1 / mass;
    if (!isStatic)
    {
        applyForce = 1;
        // Add md^2 to moment of inertia to gain the inertia over some other axis than CoM
        // m = mass & d = distance between the axes
        // Multiply with 0.0001? 0.01^2 = conversion from cm to meters
        inertia = mass * (height * height + width * width) / 12;
    }
};

void GlfwSquare::setGameControl(GlfwGameControl *gameControl)
{
    gameControl = gameControl;
}

Coords GlfwSquare::getCoordinates(bool addVelocity)
{
    float velX = 0, velY = 0;
    if (addVelocity)
    {
        velX = velocity.x;
        velY = velocity.y;
    }
    Coords coordVec;
    Vector2d vec;
    vec.x = position.x + velX + radius * std::cos(rotation + D_rotation + largeCenterAngle + smallCenterAngle * 3 / 2);
    vec.y = position.y + velY + radius * std::sin(rotation + D_rotation + largeCenterAngle + smallCenterAngle * 3 / 2);
    coordVec.insert(coordVec.end(), vec);
    vec.x = position.x + velX + radius * std::cos(rotation + D_rotation + 2 * largeCenterAngle + smallCenterAngle * 3 / 2);
    vec.y = position.y + velY + radius * std::sin(rotation + D_rotation + 2 * largeCenterAngle + smallCenterAngle * 3 / 2);
    coordVec.insert(coordVec.end(), vec);
    vec.x = position.x + velX + radius * std::cos(rotation + D_rotation + smallCenterAngle / 2);
    vec.y = position.y + velY + radius * std::sin(rotation + D_rotation + smallCenterAngle / 2);
    coordVec.insert(coordVec.end(), vec);
    vec.x = position.x + velX + radius * std::cos(rotation + D_rotation + largeCenterAngle + smallCenterAngle / 2);
    vec.y = position.y + velY + radius * std::sin(rotation + D_rotation + largeCenterAngle + smallCenterAngle / 2);
    coordVec.insert(coordVec.end(), vec);
    return coordVec;
}

void GlfwSquare::move(float xInc, float yInc)
{
    position.x += xInc;
    position.y += yInc;

    x1 = position.x + radius * std::cos(rotation + largeCenterAngle + smallCenterAngle * 3 / 2);
    y1 = position.y + radius * std::sin(rotation + largeCenterAngle + smallCenterAngle * 3 / 2);

    x2 = position.x + radius * std::cos(rotation + 2 * largeCenterAngle + smallCenterAngle * 3 / 2);
    y2 = position.y + radius * std::sin(rotation + 2 * largeCenterAngle + smallCenterAngle * 3 / 2);

    x3 = position.x + radius * std::cos(rotation + smallCenterAngle / 2);
    y3 = position.y + radius * std::sin(rotation + smallCenterAngle / 2);

    x4 = position.x + radius * std::cos(rotation + largeCenterAngle + smallCenterAngle / 2);
    y4 = position.y + radius * std::sin(rotation + largeCenterAngle + smallCenterAngle / 2);
}

void GlfwSquare::rotate(float rad)
{
    rotation += rad;
}

//Happens BEFORE draw() and during every loop
void GlfwSquare::update(double dt)
{
    if (applyForce)
    {
        // std::cout << this << "\n";
        updateForces(dt);
        updateAcceleration(dt);
        updateVelocity(dt);
    }
    collision = true;
    while (collision)
        handleCollision();
    updatePosition(dt);
}
//Happens AFTER update() and during every loop
void GlfwSquare::draw()
{
    GLfloat lineVertices[] = {
        x1, y1,
        x2, y2,
        x2, y2,
        x3, y3,
        x3, y3,
        x4, y4,
        x4, y4,
        x1, y1};

    glColor3f(1.0f, 1.0f, 1.0f);
    if (collision)
        glColor3f(1.0f, 0.0f, 0.0f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, lineVertices);
    glDrawArrays(GL_LINES, 0, 8);
    glDisableClientState(GL_VERTEX_ARRAY);
}

/**
 * Private
 */
void GlfwSquare::calculateImpulse(std::vector<GlfwSquare *> squares, std::vector<Vector2d> points)
{
    float restitution = 0.7;
    float theta = 0;
    float hyp = 0;
    Vector2d normal = points[points.size() - 1];
    Vector2d impulseVector;
    Vector2d P;

    for (int i = 0; i < squares.size(); ++i)
    {
        impulse = -(1 + restitution) * ((previousVelocity - squares[i]->previousVelocity).dot(normal)) / (invMass + squares[i]->invMass);
        // Apply impulse
        impulseVector.x = impulse * normal.x;
        impulseVector.y = impulse * normal.y;

        squares[i]->velocity.x = squares[i]->previousVelocity.x - squares[i]->invMass * impulseVector.x;
        squares[i]->velocity.y = squares[i]->previousVelocity.y - squares[i]->invMass * impulseVector.y;

        velocity.x = previousVelocity.x + invMass * impulseVector.x;
        velocity.y = previousVelocity.y + invMass * impulseVector.y;

        //TODO: DO BETTER
        //TODO FIRSST: FIX INFINITE LOOP???
        P = normal;
        P.rotate(-M_PI / 2);
        float d1 = position.dot(P);
        float d2 = points[0].dot(P);
        float d = (d2 - d1);
        if (P.y * d < 0)
            d = -1 * d;
        //std::cout << P.y * d << "\n";
        D_rotation += d * std::sqrt(impulseVector.dot(impulseVector)) * invMass * invMass * 0.003;

        P.rotate(rotation - squares[i]->rotation);
        d1 = squares[i]->position.dot(P);
        d2 = points[0].dot(P);
        d = (d2 - d1);
        if (P.y * d < 0)
            d = -1 * d;

        /*float ang = M_PI / 2 - (rotation - squares[i]->rotation);
        impulseVector = impulseVector * std::sin(ang);
        squares[i]->D_rotation += d * std::sqrt(impulseVector.dot(impulseVector)) * squares[i]->invMass * squares[i]->invMass * 0.003;
        P.rotate(rotation - squares[i]->rotation - M_PI / 2);
        d1 = squares[i]->position.dot(P);
        d2 = points[0].dot(P);
        d = (d2 - d1);
        if (P.y * d < 0)
            d = -1 * d;
        squares[i]->D_rotation -= d * std::sqrt(impulseVector.dot(impulseVector)) * squares[i]->invMass * squares[i]->invMass * 0.003;
        std::cout << squares[i]->D_rotation << "\n";*/
        std::cout << D_rotation << "\n";
        std::cout << velocity.x << "\n";
        std::cout << velocity.y << "\n";
        std::cout << squares[i]->velocity.x << "\n";
        std::cout << squares[i]->velocity.y << "\n";
        float a;
        std::cin >> a;
        if (a != 1)
            D_rotation = a;
    }
}

void GlfwSquare::updateForces(double dt)
{
}

void GlfwSquare::updateAcceleration(double dt)
{
}

void GlfwSquare::updateVelocity(double dt)
{
    previousVelocity = velocity;
}

void GlfwSquare::updateAngAcceleration(double dt)
{
    D_rotation = 0;
}

void GlfwSquare::updateAngVelocity(double dt)
{
    D_rotation = 0;
}

void GlfwSquare::updatePosition(double dt)
{
    position.x += velocity.x;
    position.y += velocity.y;
    rotation += D_rotation;

    move();
}

void GlfwSquare::pointCollisionControl(GlfwCollision *collisionObj)
{
    glfwCollision = collisionObj;
}

void GlfwSquare::handleCollision()
{
    std::vector<GlfwSquare *> squares = glfwCollision->withSquare(this);
    if (glfwCollision->withSquare(this).size() != 0)
    {
        collision = true;
        // Save velocity here. Function preventPenetration() changes it.
        glfwCollision->preventPenetration(this);
        // pointsOfCollision() Also returns normal at points[points.size()-1]
        std::vector<Vector2d> points = glfwCollision->pointsOfCollision(this, squares);
        // std::cout << "points: " << points.size() << "\n";
        gameControl->createObject(DebugLine(200, 200, 100, 100));
        calculateImpulse(squares, points);
    }
    else
    {
        collision = false;
    }
}

void GlfwSquare::collisionHandler()
{
}