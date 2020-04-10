#include "./../headers/GlfwHeaders.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <array>

GlfwSquare::GlfwSquare(float bottomLeftX, float bottomLeftY, float rectWidth, float rectHeight,
                       double rectRotation, bool isStatic, std::string rectName, float mass)
{
    name = rectName;

    corners[0].x = bottomLeftX;              // these are cm --> convert to meters when used
    corners[0].y = bottomLeftY;              // these are cm --> convert to meters when used
    corners[2].x = bottomLeftX + rectWidth;  // these are cm --> convert to meters when used
    corners[2].y = bottomLeftY + rectHeight; // these are cm --> convert to meters when used

    corners[1].x = corners[2].x;
    corners[1].y = corners[0].y;
    corners[3].x = corners[0].x;
    corners[3].y = corners[2].y;

    width = corners[2].x - corners[0].x;  // these are cm --> convert to meters when used
    height = corners[2].y - corners[0].y; // these are cm --> convert to meters when used

    radius = std::sqrt(width * width / 4 + height * height / 4);
    largeCenterAngle = 2 * std::acos(height / 2 / radius);
    smallCenterAngle = M_PI - largeCenterAngle;

    position.x = corners[0].x + width / 2;
    position.y = corners[0].y + height / 2;

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
    this->gameControl = gameControl;
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

    corners[0].x = position.x + radius * std::cos(rotation + largeCenterAngle + smallCenterAngle * 3 / 2);
    corners[0].y = position.y + radius * std::sin(rotation + largeCenterAngle + smallCenterAngle * 3 / 2);

    corners[1].x = position.x + radius * std::cos(rotation + 2 * largeCenterAngle + smallCenterAngle * 3 / 2);
    corners[1].y = position.y + radius * std::sin(rotation + 2 * largeCenterAngle + smallCenterAngle * 3 / 2);

    corners[2].x = position.x + radius * std::cos(rotation + smallCenterAngle / 2);
    corners[2].y = position.y + radius * std::sin(rotation + smallCenterAngle / 2);

    corners[3].x = position.x + radius * std::cos(rotation + largeCenterAngle + smallCenterAngle / 2);
    corners[3].y = position.y + radius * std::sin(rotation + largeCenterAngle + smallCenterAngle / 2);
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
        updateForces(dt);
        updateAcceleration(dt);
        updateVelocity(dt);
    }
    handleCollision();
    updatePosition(dt);
}
//Happens AFTER update() and during every loop
void GlfwSquare::draw()
{
    GLfloat lineVertices[] = {
        corners[0].x, corners[0].y,
        corners[1].x, corners[1].y,
        corners[1].x, corners[1].y,
        corners[2].x, corners[2].y,
        corners[2].x, corners[2].y,
        corners[3].x, corners[3].y,
        corners[3].x, corners[3].y,
        corners[0].x, corners[0].y};

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
    collision = false;
    std::unordered_map<std::string, GlfwSquare> colliders = glfwCollision->withConvex(this, *gameControl->getSquares());

    if (colliders.size() != 0)
    {
        colliders = glfwCollision->preventPenetration(this, colliders);
        glfwCollision->pointsOfCollision(this, colliders);
        /*
        collision = true;
        // Save velocity here. Function preventPenetration() changes it.
        // pointsOfCollision() Also returns normal at points[points.size()-1]
        std::vector<Vector2d> points = glfwCollision->pointsOfCollision(this, squares);
        // std::cout << "points: " << points.size() << "\n";
        gameControl->createObject(DebugLine(200, 200, 100, 100));
        calculateImpulse(squares, points);
        */
    }
}

void GlfwSquare::collisionHandler()
{
}