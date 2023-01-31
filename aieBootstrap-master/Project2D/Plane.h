#pragma once
#include "Application2D.h"
#include "Rigidbody.h"
#include <Gizmos.h>

class Plane : public PhysicsObject
{
public:
    Plane(glm::vec2 normal, float distance, glm::vec4 colour);
    ~Plane();

    virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
    virtual void draw();
    virtual void resetPosition();
    void resolveCollision(Rigidbody* actor);

    glm::vec2 getNormal() { return m_normal; }
    float getDistance() { return m_distanceToOrigin; }

protected:
    glm::vec2 m_normal;
    glm::vec4 m_colour;
    float m_distanceToOrigin;
};

