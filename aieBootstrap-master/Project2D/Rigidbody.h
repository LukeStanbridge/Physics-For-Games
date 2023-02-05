#pragma once
#include "Application2D.h"

class Rigidbody : public PhysicsObject
{
public:
    Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass, float angularVelocity, float moment);
    ~Rigidbody();

    virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
    void applyForce(glm::vec2 force, glm::vec2 pos);
    void resolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);
    virtual float getKineticEnergy();
    virtual float getPotentialEnergy();

    glm::vec2 getPosition() { return m_position; }
    float getOrientatation() { return m_orientation; }
    glm::vec2 getVelocity() { return m_velocity; }
    float getMass() { return m_mass; }
    float getMoment() { return m_moment; }
    float getAngularVelocity() { return m_angularVelocity; }

    glm::vec2 setVelocity(glm::vec2 newVelocity) { return m_velocity = newVelocity; }

protected:
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    float m_mass;
    float m_orientation;    //2D so we only need a single float to represent our orientation
    float m_angularVelocity;
    float m_moment;
};