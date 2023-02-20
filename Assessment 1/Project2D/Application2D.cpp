#include "Application2D.h"

Application2D::Application2D() { }

Application2D::~Application2D() { }

bool Application2D::startup()
{
	// increase the 2d line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	m_2dRenderer = new aie::Renderer2D();
	m_font = new aie::Font("./font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setTimeStep(0.01f);
	initiateGame(); // spawn game objects and start game

	return true;
}

void Application2D::shutdown() 
{
	delete m_font;
	delete m_texture;
	delete m_shipTexture;
	delete m_2dRenderer;
}

void Application2D::update(float deltaTime)
{
	aie::Input* input = aie::Input::getInstance(); // get input
	aie::Gizmos::clear();

	m_timer += deltaTime; // for increasing speed of box drops
	 
	tooLow = m_player->getPosition(); // check for player off screen and end game
	if (tooLow.y < playerMinY || tooLow.y > playerMaxY) scoreCollisions = 0;

	if (scoreCollisions > 0) // game loop
	{
		m_scoreTime += deltaTime; // for printing out game run time to screen
		if (m_timer >= 10) // increase frequency of boxes every 10 seconds
		{
			if (m_boxSpawnTimerReset > 0.6f)
			{
				m_boxSpawnTimerReset -= 0.6f;
				m_timer = 0.0f;
			}
		}
		boxDrop(deltaTime);// drop boxes to dodge with increasing frequency
	}
	else
	{	
		endGame(deltaTime);// pause game and initiate game over screen
	}

	m_physicsScene->getFallingObjects(); // delete falling boxes after they are off screen
	controlPlayer(input); // control player movement
	m_physicsScene->update(deltaTime);
	m_physicsScene->draw();
	
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE)) quit(); // exit the application
	if (m_gameOver) // trigger restart
	{
		if (input->isKeyDown(aie::INPUT_KEY_SPACE)) restartGame = true;
	}
	if (restartGame) // clear screen and re-initialise game
	{
		m_physicsScene->removeAllActors();
		gameOver.clear();
		if (gameOver.empty() && m_physicsScene->m_actors.empty())
		{
			initiateGame();
			m_gameOver = false;
			restartGame = false;
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

	// update collison count display
	std::string s = std::to_string(scoreCollisions);
	char const* collisions = s.c_str();
	m_2dRenderer->drawText(m_font, collisions, 0, 40);
	m_2dRenderer->drawText(m_font, "collisons left", 40, 40);

	// prompt to restart the game
	if (m_gameOver)	m_2dRenderer->drawText(m_font, "Press SPACE to restart", 750, 500);

	// update timer display
	std::string t = std::to_string(m_scoreTime);
	char const* timer = t.c_str();
	m_2dRenderer->drawText(m_font, timer, 0, 80);
	m_2dRenderer->drawText(m_font, "W-A-S-D Controls", 0, 120);

	// done drawing sprites
	m_2dRenderer->end();
}

void Application2D::initiateGame()
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
	m_physicsScene->addActor(ball1);
	m_physicsScene->addActor(plane3);
	m_physicsScene->addActor(plane4);

	m_player = ball1;

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

void Application2D::endGame(float deltaTime)
{
	m_player->setKinematic(true); // pause player movement
	m_softBodSpawnTimer -= deltaTime;
	if (m_softBodSpawnTimer <= 0)
	{
		Plane* plane = new Plane(glm::vec2(0, 1), -50, glm::vec4(1, 1, 0, 1)); //bottom
		m_physicsScene->addActor(plane);

		gameOver.push_back("...........................................................");
		gameOver.push_back("...00.....000000..000000....000000..000000..000000..00...00");
		gameOver.push_back("...00.....000000..000000....000000..000000..000000..00..00.");
		gameOver.push_back("...00.....00..00..00..00........00..00..00..000000..00.00..");
		gameOver.push_back("...00.....00..00..00..00....000000..00..00..00......0000...");
		gameOver.push_back("..0000....00..00..00..00....00......00..00..00......0000...");
		gameOver.push_back(".00..00...000000..00..00....000000..00..00..000000..00.00..");
		gameOver.push_back("00....00..000000..00..00....000000..00..00..000000..00..00.");
		gameOver.push_back("...........................................................");

		SoftBody::Build(m_physicsScene, glm::vec2(-70, 40), 4.0f, 10.0f, 1.0f, gameOver);
		m_softBodSpawnTimer = m_softBodSpawnTimerReset;
		m_gameOver = true;
	}
}

void Application2D::boxDrop(float deltaTime)
{
	// drop green boxes
	m_boxSpawnTimer -= deltaTime;
	if (m_boxSpawnTimer <= 0)
	{
		spawnBoxes(glm::vec4(0, 1, 0, 1));
		m_boxSpawnTimer = m_boxSpawnTimerReset;
	}
	// drop blue boxes
	m_boxSpawnTimer1 -= deltaTime;
	if (m_boxSpawnTimer1 <= 0)
	{
		spawnBoxes(glm::vec4(0, 1, 1, 1));
		m_boxSpawnTimer1 = m_boxSpawnTimerReset;
	}
	// drop yellow boxes
	m_boxSpawnTimer2 -= deltaTime;
	if (m_boxSpawnTimer2 <= 0)
	{
		spawnBoxes(glm::vec4(1, 1, 0, 1));
		m_boxSpawnTimer2 = m_boxSpawnTimerReset;
	}
	// drop pink boxes
	m_boxSpawnTimer3 -= deltaTime;
	if (m_boxSpawnTimer3 <= 0)
	{
		spawnBoxes(glm::vec4(1, 0, 1, 1));
		m_boxSpawnTimer3 = m_boxSpawnTimerReset;
	}
	// drop white edge boxes
	m_sideBoxTimer -= deltaTime;
	if (m_sideBoxTimer <= 0)
	{
		Box* box1 = new Box(glm::vec2(5, 3), glm::vec4(1, 1, 1, 1), glm::vec2(spawnXmax, 60), glm::vec2(0), 5.0f, 1.0f, 0.3f, 0.3f, false);
		m_physicsScene->addActor(box1);
		Box* box2 = new Box(glm::vec2(5, 3), glm::vec4(1, 1, 1, 1), glm::vec2(spawnXmin, 60), glm::vec2(0), 5.0f, 1.0f, 0.3f, 0.3f, false);
		m_physicsScene->addActor(box2);
		m_sideBoxTimer = m_sideBoxTimerReset;
	}
}

void Application2D::spawnBoxes(glm::vec4 colour)
{
	int randPos = (rand() % (spawnXmax - spawnXmin + 1)) + spawnXmin;
	Box* box = new Box(glm::vec2(3, 3), colour, glm::vec2(randPos, 60), glm::vec2(0), 5.0f, 1.0f, 0.3f, 0.3f, false);
	m_physicsScene->addActor(box);
}

void Application2D::controlPlayer(aie::Input* input)
{
	if (input->isKeyDown(87)) // Up
	{
		m_player->applyForce(glm::vec2(0, 100), m_player->getPosition());
	}
	if (input->isKeyDown(83)) // Down
	{
		m_player->applyForce(glm::vec2(0, -60), m_player->getPosition());
	}
	if (input->isKeyDown(65)) // Left
	{
		m_player->applyForce(glm::vec2(-60, 0), m_player->getPosition());
	}
	if (input->isKeyDown(68)) // Right
	{
		m_player->applyForce(glm::vec2(60, 0), m_player->getPosition());
	}
}

//create a rope
/*void Application2D::Rope(int num)
{
	Sphere* prev = nullptr;
	for (int i = 0; i < num; i++)
	{
		// spawn a sphere to the right and below the previous one, so that the whole rope acts under gravityand swings
		Sphere* sphere = new Sphere(glm::vec2(i * 3 - 30, 30 - i * 5), glm::vec2(0), 10, 2, glm::vec4(1, 0, 0, 1), 4.0f, 0.3f, 0.3f, false);
		if (i == 0 || i == 3)
			sphere->setKinematic(true);
		m_physicsScene->addActor(sphere);
		if (prev)
			m_physicsScene->addActor(new Spring(sphere, prev, 400, 20, 7));
		prev = sphere;
	}

	// add a kinematic box at an angle for the rope to drape over 
	Box* box = new Box(glm::vec2(10, 1), glm::vec4(1, 0, 0, 1), glm::vec2(-20, 35), glm::vec2(0), 5.0f, 1.0f, 0.3f, 0.3f, false);
	//box->setKinematic(true);
	m_physicsScene->addActor(box);
}*/

//screen to world pos
/*glm::vec2 Application2D::screenToWorld(glm::vec2 screenPos)
{
	glm::vec2 worldPos = screenPos;

	// move the centre of the screen to (0,0) 
	worldPos.x -= getWindowWidth() / 2;
	worldPos.y -= getWindowHeight() / 2;

	// scale according to our extents 
	worldPos.x *= 2.0f * extents / getWindowWidth();
	worldPos.y *= 2.0f * extents / (aspectRatio * getWindowHeight());

	return worldPos;
}*/

//Penis Ascii
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

// C++ sucks Ascii
	/*gameOver.push_back(".......................................................");
	//gameOver.push_back("000000..........000000..000000..000000..00...00..000000");
	//gameOver.push_back("000000..........000000..000000..000000..00..00...000000");
	//gameOver.push_back("000000..0...0.......00..00..00..000000..00.00........00");
	//gameOver.push_back("00.....000.000..000000..00..00..00......0000.....000000");
	//gameOver.push_back("00......0...0...00......00..00..00......0000.....00....");
	//gameOver.push_back("000000..........000000..00..00..000000..00.00....000000");
	//gameOver.push_back("000000..........000000..00..00..000000..00..00...000000");
	//gameOver.push_back(".......................................................");*/

// diplay circle on click
	/*if (input->isMouseButtonDown(0))
	{
		int xScreen, yScreen;
		input->getMouseXY(&xScreen, &yScreen);
		glm::vec2 worldPos = screenToWorld(glm::vec2(xScreen, yScreen));
	
		aie::Gizmos::add2DCircle(worldPos, 5, 32, glm::vec4(1, 0, 1, 1));
	}*/