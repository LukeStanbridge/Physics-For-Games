#include "Spring.h"
#include "Gizmos.h"

Spring::Spring(Rigidbody* body1, Rigidbody* body2, float springCoefficient, float damping, float restLength, glm::vec2 contact1, glm::vec2 contact2) : PhysicsObject(JOINT)
{
	m_body1 = body1;
	m_body2 = body2;
	m_springCoefficient = springCoefficient;
	m_damping = damping;
	m_restLength = restLength;
	m_contact1 = contact1;
	m_contact2 = contact2;
}

Spring::~Spring() { }

void Spring::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	// get the world coordinates of the ends of the springs 
	glm::vec2 p1 = getContact1();
	glm::vec2 p2 = getContact2();

	float length = glm::distance(p1, p2);
	glm::vec2 direction = glm::normalize(p2 - p1);

	// apply damping 
	glm::vec2 relativeVelocity = m_body2->getVelocity() - m_body1->getVelocity();

	// F = -kX - bv 
	glm::vec2 force = direction * m_springCoefficient * (m_restLength - length) - m_damping * relativeVelocity;

	// cap the spring force to 1000 N to prevent numerical instability 
	const float threshold = 1000.0f;
	float forceMag = glm::length(force);
	if (forceMag > threshold)
		force *= threshold / forceMag;

	m_body1->applyForce(-force * timeStep, p1 - m_body1->getPosition());
	m_body2->applyForce(force * timeStep, p2 - m_body2->getPosition());
}

void Spring::draw()
{
	aie::Gizmos::add2DLine(getContact1(), getContact2(), glm::vec4(1, 1, 1, 1));
}

glm::vec2 Spring::getContact1()
{
	return m_body1 ? m_body1->getPosition() : m_contact1;
}

glm::vec2 Spring::getContact2()
{
	return m_body2 ? m_body2->getPosition() : m_contact2;
}