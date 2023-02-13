#pragma once
#include "glm/glm.hpp"

enum ShapeType
{
    JOINT = -1,
    PLANE = 0,
    SPHERE,
    BOX,
};

const int SHAPE_COUNT = 3;

class PhysicsObject
{
protected:
    PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}

public:
    virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
    virtual void draw() = 0;
    virtual void resetPosition() {};
    virtual float getEnergy() { return 0; }
    virtual float getElasticity() { return 0.3f; }
    virtual ShapeType getShapeID() { return m_shapeID; }
    virtual glm::vec2 getPosition() { return m_position; }

protected:
    ShapeType m_shapeID;
    float m_elasticity;
    glm::vec2 m_position;
};

