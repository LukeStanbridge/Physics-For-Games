#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Sphere.h"
#include "Box.h"
#include "Plane.h"
#include "Input.h"
#include "Spring.h"
#include "SoftBody.h"
#include <cstdlib>
#include <string>

Application2D::Application2D()
{
	
}

Application2D::~Application2D()
{

}

bool Application2D::startup()
{
	// increase the 2d line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();
	m_font = new aie::Font("./font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();

	m_physicsScene->setTimeStep(0.01f);

	ObjectTest(); // spawn game objects

	return true;
}

void Application2D::shutdown() {

	delete m_font;
	delete m_texture;
	delete m_shipTexture;
	delete m_2dRenderer;
}

void Application2D::update(float deltaTime)
{
	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_timer += deltaTime; // for increasing speed of box drops

	// circle click
	if (input->isMouseButtonDown(0))
	{
		int xScreen, yScreen;
		input->getMouseXY(&xScreen, &yScreen);
		glm::vec2 worldPos = screenToWorld(glm::vec2(xScreen, yScreen));

		aie::Gizmos::add2DCircle(worldPos, 5, 32, glm::vec4(1, 0, 1, 1));
	}

	// check for player off screen
	tooLow = m_player->getPosition();
	if (tooLow.y < playerMinY || tooLow.y > playerMaxY) scoreCollisions = 0;

	// game loop
	if (scoreCollisions > 0)
	{
		m_scoreTime += deltaTime; // for printing out game time to screen
		// increase frequency of boxes 
		if (m_timer >= 10)
		{
			if (m_boxSpawnTimerReset > 0.6f)
			{
				m_boxSpawnTimerReset -= 0.6f;
				m_timer = 0.0f;
			}
		}

		// drop green boxes
		m_boxSpawnTimer -= deltaTime;
		if (m_boxSpawnTimer <= 0)
		{
			int randPos = (rand() % (spawnXmax - spawnXmin + 1)) + spawnXmin;
			Box* box1 = new Box(glm::vec2(3, 3), glm::vec4(0, 1, 0, 1), glm::vec2(randPos, 60), glm::vec2(0), 5.0f, 1.0f, 0.3f, 0.3f, false);
			m_physicsScene->addActor(box1);
			m_boxSpawnTimer = m_boxSpawnTimerReset;
		}

		// drop blue boxes
		m_boxSpawnTimer1 -= deltaTime;
		if (m_boxSpawnTimer1 <= 0)
		{
			int randPos = (rand() % (spawnXmax - spawnXmin + 1)) + spawnXmin;
			Box* box2 = new Box(glm::vec2(3, 3), glm::vec4(0, 1, 1, 1), glm::vec2(randPos, 60), glm::vec2(0), 5.0f, 1.0f, 0.3f, 0.3f, false);
			m_physicsScene->addActor(box2);
			m_boxSpawnTimer1 = m_boxSpawnTimerReset;
		}

		// drop yellow boxes
		m_boxSpawnTimer2 -= deltaTime;
		if (m_boxSpawnTimer2 <= 0)
		{
			int randPos = (rand() % (spawnXmax - spawnXmin + 1)) + spawnXmin;
			Box* box3 = new Box(glm::vec2(3, 3), glm::vec4(1, 1, 0, 1), glm::vec2(randPos, 60), glm::vec2(0), 5.0f, 1.0f, 0.3f, 0.3f, false);
			m_physicsScene->addActor(box3);
			m_boxSpawnTimer2 = m_boxSpawnTimerReset;
		}

		// drop pink boxes
		m_boxSpawnTimer3 -= deltaTime;
		if (m_boxSpawnTimer3 <= 0)
		{
			int randPos = (rand() % (spawnXmax - spawnXmin + 1)) + spawnXmin;
			Box* box3 = new Box(glm::vec2(3, 3), glm::vec4(1, 0, 1, 1), glm::vec2(randPos, 60), glm::vec2(0), 5.0f, 1.0f, 0.3f, 0.3f, false);
			m_physicsScene->addActor(box3);
			m_boxSpawnTimer3 = m_boxSpawnTimerReset;
		}

		// drop white edge boxes
		m_sideBoxTimer -= deltaTime;
		if (m_sideBoxTimer <= 0)
		{
			Box* box4 = new Box(glm::vec2(5, 3), glm::vec4(1, 1, 1, 1), glm::vec2(spawnXmax, 60), glm::vec2(0), 5.0f, 1.0f, 0.3f, 0.3f, false);
			m_physicsScene->addActor(box4);
			Box* box5 = new Box(glm::vec2(5, 3), glm::vec4(1, 1, 1, 1), glm::vec2(spawnXmin, 60), glm::vec2(0), 5.0f, 1.0f, 0.3f, 0.3f, false);
			m_physicsScene->addActor(box5);
			m_sideBoxTimer = m_sideBoxTimerReset;
		}
	}
	else
	{
		m_player->setKinematic(true);
		m_softBodSpawnTimer -= deltaTime;
		if (m_softBodSpawnTimer <= 0)
		{
			Plane* plane = new Plane(glm::vec2(0, 1), -50, glm::vec4(1, 1, 0, 1)); //bottom
			m_physicsScene->addActor(plane);

			gameOver.push_back(".......................................................");
			gameOver.push_back("000000..........000000..000000..000000..00...00..000000");
			gameOver.push_back("000000..........000000..000000..000000..00..00...000000");
			gameOver.push_back("000000..0...0.......00..00..00..000000..00.00........00");
			gameOver.push_back("00.....000.000..000000..00..00..00......0000.....000000");
			gameOver.push_back("00......0...0...00......00..00..00......0000.....00....");
			gameOver.push_back("000000..........000000..00..00..000000..00.00....000000");
			gameOver.push_back("000000..........000000..00..00..000000..00..00...000000");
			gameOver.push_back(".......................................................");

			SoftBody::Build(m_physicsScene, glm::vec2(-70, 40), 4.0f, 10.0f, 1.0f, gameOver);
			m_softBodSpawnTimer = m_softBodSpawnTimerReset;
			m_gameOver = true;
		}
	}

	m_physicsScene->getFallingObjects(); // delete falling boxes after they are off screen

	// move player
	if (input->isKeyDown(87)) // Up
	{
		m_player->applyForce(glm::vec2(0, 80), m_player->getPosition());
	}
	if (input->isKeyDown(83)) // Down
	{
		m_player->applyForce(glm::vec2(0, -60), m_player->getPosition());
	}
	if (input->isKeyDown(65)) // Left
	{
		m_player->applyForce(glm::vec2(-40, 0), m_player->getPosition());
	}
	if (input->isKeyDown(68)) // Right
	{
		m_player->applyForce(glm::vec2(40, 0), m_player->getPosition());
	}

	m_physicsScene->update(deltaTime);
	m_physicsScene->draw();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
	
	// restart application
	if (m_gameOver)
	{
		if (input->isKeyDown(aie::INPUT_KEY_SPACE))
		{
			m_physicsScene->removeAllActors();
			gameOver.clear();
			if (gameOver.empty() && m_physicsScene->m_actors.empty())
			{
				ObjectTest();
				m_gameOver = false;
			}
		}
	}
}

void Application2D::draw()
{
	// wipe the screen to the background colour
	clearScreen();

	// set the camera position before we begin rendering 
	m_2dRenderer->setCameraPos(m_cameraX, m_cameraY);

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	aie::Gizmos::draw2D(glm::ortho<float>(-extents, extents, -extents / aspectRatio, extents / aspectRatio, -1.0f, 1.0f));

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// update score
	std::string s = std::to_string(scoreCollisions);
	char const* collisions = s.c_str();
	m_2dRenderer->drawText(m_font, collisions, 0, 40);
	m_2dRenderer->drawText(m_font, "collisons left", 40, 40);

	if (m_gameOver)
	{
		m_2dRenderer->drawText(m_font, "Press SPACE to restart", 750, 500);
	}

	// update timer
	std::string t = std::to_string(m_scoreTime);
	char const* timer = t.c_str();
	m_2dRenderer->drawText(m_font, timer, 0, 80);

	// done drawing sprites
	m_2dRenderer->end();
}

void Application2D::Rope(int num)
{
	/*m_physicsScene->setGravity(glm::vec2(0, -9.82f));*/

	Sphere* prev = nullptr;
	for (int i = 0; i < num; i++)
	{
		// spawn a sphere to the right and below the previous one, so that the whole rope acts under gravityand swings
			Sphere* sphere = new Sphere(glm::vec2(i * 3 - 30, 30 - i * 5), glm::vec2(0), 10, 2, glm::vec4(1,0, 0, 1), 4.0f, 0.3f, 0.3f, false);
		if (i == 0 || i == 3)
			sphere->setKinematic(true);
		m_physicsScene->addActor(sphere);
		if (prev)
			m_physicsScene->addActor(new Spring(sphere, prev, 400, 20, 7));
		prev = sphere;
	}

	// add a kinematic box at an angle for the rope to drape over 
	Box* box = new Box(glm::vec2(10, 1), glm::vec4(1, 0, 0, 1), glm::vec2(-20, 35), glm::vec2(0), 5.0f, 1.0f, 0.3f, 0.3f, false);
	/*box->setKinematic(true);*/
	m_physicsScene->addActor(box);
}

glm::vec2 Application2D::screenToWorld(glm::vec2 screenPos)
{
	glm::vec2 worldPos = screenPos;

	// move the centre of the screen to (0,0) 
	worldPos.x -= getWindowWidth() / 2;
	worldPos.y -= getWindowHeight() / 2;

	// scale according to our extents 
	worldPos.x *= 2.0f * extents / getWindowWidth();
	worldPos.y *= 2.0f * extents / (aspectRatio * getWindowHeight());

	return worldPos;
}

void Application2D::ObjectTest()
{
	m_physicsScene->setGravity(glm::vec2(0, m_gravity));

	// reset variables
	scoreCollisions = 3;
	m_scoreTime = 0;
	m_boxSpawnTimer = 3.0f;
	m_boxSpawnTimer1 = 4.5f;
	m_boxSpawnTimer2 = 1.5f;
	m_boxSpawnTimer3 = 6.0f;
	m_boxSpawnTimerReset = 3.0f;

	m_sideBoxTimer = 4.0f;
	m_sideBoxTimerReset = 4.0f;

	m_softBodSpawnTimer = 5.0f;
	m_softBodSpawnTimerReset = 100.0f;

	// spawn necessary objects
	Sphere* ball1 = new Sphere(glm::vec2(38, 0), glm::vec2(0), 100.0f, 4, glm::vec4(1, 0, 0, 1), 4.0f, 0.3f, 0.3f, false); //red
	Plane* plane3 = new Plane(glm::vec2(1, 0), -80, glm::vec4(1, 1, 0, 1)); // left
	Plane* plane4 = new Plane(glm::vec2(-1, 0), -80, glm::vec4(1, 1, 0, 1)); // right

	m_player = ball1;

	m_physicsScene->addActor(ball1);
	m_physicsScene->addActor(plane3);
	m_physicsScene->addActor(plane4);

	// check for collison with box
	ball1->collisionCallback = [=](PhysicsObject* other)
	{
		if (other != nullptr && scoreCollisions > 0)
		{
			scoreCollisions--;
		}
		return;
	};
}

//Ascii penis
/*std::vector<std::string> sb;
			sb.push_back(".......");
			sb.push_back("..000..");
			sb.push_back("..000..");
			sb.push_back("..000..");
			sb.push_back("..000..");
			sb.push_back("..000..");
			sb.push_back("000.000");
			sb.push_back("000.000");
			sb.push_back(".......");*/

// LBJ Sucks Ascii
			/*std::vector<std::string> sb;
				sb.push_back(".................................................................");
				sb.push_back("000000..000000..000000....000000..000000..000000..00...00..000000");
				sb.push_back("000000..00..00..000000....000000..000000..000000..00..00...000000");
				sb.push_back("00......00..00..00..00........00..00..00..000000..00.00........00");
				sb.push_back("00......00000.......00....000000..00..00..00......0000.....000000");
				sb.push_back("00......00..00......00....00......00..00..00......0000.....00....");
				sb.push_back("00......00..00......00....000000..00..00..000000..00.00....000000");
				sb.push_back("00......000000......00....000000..00..00..000000..00..00...000000");
				sb.push_back(".................................................................");



				SoftBody::Build(m_physicsScene, glm::vec2(-95, 0), 5.0f, 10.0f, 1.0f, sb);*/