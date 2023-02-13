#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "Sphere.h"
#include <glm\ext.hpp>
#include <Gizmos.h>
#include <iostream>

class Application2D : public aie::Application
{
public:

	Application2D();
	virtual ~Application2D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	virtual void Rope(int num);

	virtual glm::vec2 Application2D::screenToWorld(glm::vec2 screenPos);

	void ObjectTest();

protected:

	aie::Renderer2D* m_2dRenderer;
	aie::Texture* m_texture;
	aie::Texture* m_shipTexture;
	aie::Font* m_font;

	PhysicsScene* m_physicsScene;
	float m_scoreTime = 0.0f;
	float m_timer = 0.0f;
	float m_cameraX = getWindowWidth();
	float m_cameraY = getWindowHeight();

	float m_boxSpawnTimer = 3.0f;
	float m_boxSpawnTimer1 = 4.5f;
	float m_boxSpawnTimer2 = 1.5f;
	float m_boxSpawnTimer3 = 6.0f;
	float m_boxSpawnTimerReset = 3.0f;

	float m_sideBoxTimer = 4.0f;
	float m_sideBoxTimerReset = 4.0f;

	float m_softBodSpawnTimer = 5.0f;
	float m_softBodSpawnTimerReset = 100.0f;

	const float extents = 100;
	const float aspectRatio = 16.0f / 9.0f;
	const float m_gravity = -19.82f;
	const int spawnXmin = -70;
	const int spawnXmax = 70;
	const int playerMinY = -60;
	const int playerMaxY = 60;

	Sphere* m_player;
	int scoreCollisions = 3;
	bool m_gameOver = false;
	std::vector<std::string> gameOver;
	glm::vec2 tooLow;

};