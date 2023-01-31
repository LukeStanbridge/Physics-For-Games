#include "Rigidbody.h"
#include <iostream>
#include <cmath>

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass) : PhysicsObject(shapeID)
{
	m_shapeID = shapeID;
	m_position = position;
	m_velocity = velocity;
	m_orientation = orientation;
	m_mass = mass;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	applyForce(gravity * m_mass * timeStep);
	m_position += m_velocity * timeStep;
}

void Rigidbody::applyForce(glm::vec2 force)
{
	m_velocity += force / m_mass;
}

void Rigidbody::applyForceToActor(Rigidbody* actor2, glm::vec2 force)
{
	actor2->applyForce(force);
	applyForce(-force);
}

void Rigidbody::resolveCollision(Rigidbody* actor2)
{
	glm::vec2 normal = glm::normalize(actor2->getPosition() - m_position);
	glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;

	float elasticity = 1;
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
		((1 / m_mass) + (1 / actor2->getMass()));

	glm::vec2 force = normal * j;

	float kePre = getKineticEnergy() + actor2->getKineticEnergy();

	applyForceToActor(actor2, force);

	float kePost = getKineticEnergy() + actor2->getKineticEnergy();

	float deltaKE = kePost - kePre;
	if (deltaKE > kePost * 0.01f)
		std::cout << "Kinetic Energy discrepancy greater than 1% detected!!";
}

float Rigidbody::getKineticEnergy()
{
	return (0.5 * m_mass * glm::dot(m_velocity, m_velocity));  // KE = 1/2 * m * v(to the power of 2)
}

float Rigidbody::getPotentialEnergy()
{
	return -getMass() * glm::dot(PhysicsScene::getGravity(), getPosition());
}
