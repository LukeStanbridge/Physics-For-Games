#pragma once
#include "glm/glm.hpp"

enum ShapeType
{
    PLANE = 0,
    SPHERE,
    /*BOX,*/
    SHAPE_COUNT
};

class PhysicsObject
{
protected:
    PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}

public:
    virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
    virtual void draw() = 0;
    virtual void resetPosition() {};
    virtual float getEnergy() { return 0; }
    virtual ShapeType getShapeID() { return m_shapeID; }

protected:
    ShapeType m_shapeID;
};