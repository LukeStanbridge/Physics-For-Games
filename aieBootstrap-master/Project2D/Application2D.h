#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include <glm\ext.hpp>
#include <Gizmos.h>

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

protected:

	aie::Renderer2D* m_2dRenderer;
	aie::Texture* m_texture;
	aie::Texture* m_shipTexture;
	aie::Font* m_font;

	PhysicsScene* m_physicsScene;
	float m_timer;
	float m_cameraX = getWindowWidth();
	float m_cameraY = getWindowHeight();
	const float extents = 100;
	const float aspectRatio = 16.0f / 9.0f;
};