#include "Rigidbody.h"
#include <iostream>
#include <cmath>
#include "Gizmos.h"
#include "PhysicsScene.h"
#include "glm/glm.hpp"

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass, float angularVelocity, float linearDrag, float angularDrag, bool iskinematic) : PhysicsObject(shapeID)
{
	m_shapeID = shapeID;
	m_position = position;
	m_velocity = velocity;
	m_orientation = orientation;
	m_mass = mass;
	m_angularVelocity = angularVelocity;
	m_linearDrag = linearDrag;
	m_angularDrag = angularDrag;
	m_isKinematic = iskinematic;
}

Rigidbody::~Rigidbody()
{
}


void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	//store the local axes 
	float cs = cosf(m_orientation);
	float sn = sinf(m_orientation);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));

	if (m_isKinematic)
	{
		m_velocity = glm::vec2(0);
		m_angularVelocity = 0;
		return;
	}

	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

	if (length(m_velocity) < MIN_LINEAR_THRESHOLD) 
	{
		m_velocity = glm::vec2(0, 0);
	}
	if (abs(m_angularVelocity) < MIN_ANGULAR_THRESHOLD) 
	{
		m_angularVelocity = 0;
	}

	m_position += m_velocity * timeStep;
	applyForce(gravity * getMass() * timeStep, glm::vec2(0, 0));

	m_orientation += m_angularVelocity * timeStep;
}

void Rigidbody::applyForce(glm::vec2 force, glm::vec2 pos)
{
	// use getMass() and getMoment() here in case we ever get it to do something more than just return mass...
	m_velocity += force / getMass();
	m_angularVelocity += (force.y * pos.x - force.x * pos.y) / getMoment();
}

void Rigidbody::resolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal, float pen)
{
	// find the vector between their centres, or use the provided direction of force, and make sure it's normalised 
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal : actor2->m_position - m_position);
	// get the vector perpendicular to the collision normal 
	glm::vec2  perp(normal.y, -normal.x);

	// determine the total velocity of the contact points for the two objects, for both linear and rotational     

	 // 'r' is the radius from axis to application of force 
	float r1 = glm::dot(contact - m_position, -perp);
	float r2 = glm::dot(contact - actor2->m_position, perp);
	// velocity of the contact point on this object  
	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	// velocity of contact point on actor2 
	float v2 = glm::dot(actor2->m_velocity, normal) + r2 * actor2->m_angularVelocity;

	if (v1 > v2) // they're moving closer 
	{
		// calculate the effective mass at contact point for each object 
		// ie how much the contact point will move due to the force applied. 
		float mass1 = 1.0f / (1.0f / getMass() + (r1 * r1) / getMoment());
		float mass2 = 1.0f / (1.0f / actor2->getMass() + (r2 * r2) / actor2->getMoment());

		float elasticity = (getElasticity() + actor2->getElasticity()) / 2.0f;

		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;

		//apply equal and opposite forces 
		applyForce(-force, contact - m_position);
		actor2->applyForce(force, contact - actor2->m_position);

		if (collisionCallback != nullptr)
			collisionCallback(actor2);
		if (actor2->collisionCallback)
			actor2->collisionCallback(this);
	}

	if (pen > 0) PhysicsScene::ApplyContactForces(this, actor2, normal, pen);
}

float Rigidbody::getKineticEnergy()
{
	return 0.5f * (getMass() * glm::dot(m_velocity, m_velocity) + getMoment() * m_angularVelocity * m_angularVelocity);
}

float Rigidbody::getPotentialEnergy()
{
	return -getMass() * glm::dot(PhysicsScene::getGravity(), getPosition());
}