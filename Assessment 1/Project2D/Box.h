#pragma once
#include "Rigidbody.h"

class Box : public Rigidbody
{
public:
	Box(glm::vec2 extents, glm::vec4 colour, glm::vec2 position, glm::vec2 velocity, float mass, float angularVelocity, float linearDrag, float angularDrag, bool iskinematic);
	~Box();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void draw();

	bool checkBoxCorners(Box& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal);

	glm::vec2 getExtents() { return m_extents; }
	float getWidth() { return m_extents.x * 2; }
	float getHeight() { return m_extents.y * 2; }
	float getAngularVelocity() { return m_angularVelocity; }

protected:
	glm::vec2 m_extents;   // the halfedge lengths  
	glm::vec4 m_colour;
	glm::vec2 m_normal;
};

