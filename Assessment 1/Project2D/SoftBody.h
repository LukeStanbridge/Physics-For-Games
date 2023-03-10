#pragma once
#include "PhysicsScene.h"
#include <string>

class SoftBody
{
public:
	static void Build(PhysicsScene* scene, glm::vec2 position, float spacing, float springForce, float damping, std::vector<std::string>& strings);

protected:
	glm::vec2 m_position;
};