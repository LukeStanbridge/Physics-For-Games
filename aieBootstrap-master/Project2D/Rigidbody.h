#pragma once
#include "PhysicsObject.h"
#include <functional>
#include <vector>

class Rigidbody : public PhysicsObject
{
public:
    Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass, float angularVelocity, float linearDrag, float angularDrag, bool iskinematic);
    ~Rigidbody();

    virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
    void applyForce(glm::vec2 force, glm::vec2 pos);
    void resolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr, float pen = 0);

    void setKinematic(bool state) { m_isKinematic = state; }
    bool isKinematic() { return m_isKinematic; }

    glm::vec2 getPosition() const { return m_position; }
    void setPosition(glm::vec2 position) { m_position = position; }

    glm::vec2 toWorld(glm::vec2 worldCoordinates) { return worldCoordinates + getLocalX() * worldCoordinates.x + getLocalY() * worldCoordinates.y; }

    float getOrientatation() { return m_orientation; }
    glm::vec2 getVelocity() { return m_velocity; }
    float getMass() { return m_isKinematic ? INT_MAX : m_mass; }
    float getAngularVelocity() { return m_angularVelocity; }
    float getMoment() { return m_isKinematic ? INT_MAX : m_moment; }

    glm::vec2 getLocalX() { return m_localX; }
    glm::vec2 getLocalY() { return m_localY; }

    glm::vec2 setVelocity(glm::vec2 newVelocity) { return m_velocity = newVelocity; }

    std::function<void(PhysicsObject*)> collisionCallback;

protected:
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    float m_mass;
    float m_orientation;    //2D so we only need a single float to represent our orientation
    float m_angularVelocity;
    float m_moment = 0.0f;
    float m_linearDrag;
    float m_angularDrag;
    bool m_isKinematic;

    // store the local x,y axes of the box based on its angle of rotation 
    glm::vec2 m_localX = { 0, 0 };
    glm::vec2 m_localY = { 0, 0 };

    const float MIN_LINEAR_THRESHOLD = 0.1f;
    const float MIN_ANGULAR_THRESHOLD = 0.1f;

    std::vector<Rigidbody*> m_collided;
};