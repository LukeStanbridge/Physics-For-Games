#pragma once
#include "Rigidbody.h"

class Sphere : public Rigidbody
{
public:
    Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour, float angularVelocity, float linearDrag, float angularDrag, bool iskinematic);
    ~Sphere();

    virtual void draw();
    virtual bool IsInside(glm::vec2 point);

    float getRadius() { return m_radius; }
    float getMass() { return m_mass; }
    float getMoment() { return m_moment; }
    glm::vec4 getColour() { return m_colour; }

protected:
    float m_radius;
    glm::vec4 m_colour;
};

