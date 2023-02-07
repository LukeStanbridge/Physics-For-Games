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
	//m_physicsScene->setGravity(glm::vec2(0, 0));  // turn off gravity
	m_physicsScene->setGravity(glm::vec2(0, -19.82f));
	m_physicsScene->setTimeStep(0.01f);

	Sphere* ball1 = new Sphere(glm::vec2(38, 0), glm::vec2(0), 100.0f, 4, glm::vec4(1, 0, 0, 1), 4.0f, 0.3f, 0.3f, false); //red
	Sphere* ball2 = new Sphere(glm::vec2(19, 40), glm::vec2(0), 100.0f, 4, glm::vec4(0, 1, 0, 1), 1.0f, 0.3f, 0.3f, true); // green
	Sphere* ball3 = new Sphere(glm::vec2(-15, -20), glm::vec2(0), 100.0f, 8, glm::vec4(0, 1, 1, 1), 10.0f, 0.3f, 0.3f, false); //blue
	Plane* plane = new Plane(glm::vec2(0, 1), -50, glm::vec4(1, 1, 0, 1));
	Plane* plane2 = new Plane(glm::vec2(0, -1), -50, glm::vec4(1, 1, 0, 1));
	Plane* plane3 = new Plane(glm::vec2(1, 0), -80, glm::vec4(1, 1, 0, 1));
	Plane* plane4 = new Plane(glm::vec2(-1, 0), -80, glm::vec4(1, 1, 0, 1));

	Spring* spring1 = new Spring(ball1, ball2, 500.0f, 1.0f, 30.0f);
	Spring* spring2 = new Spring(ball1, ball3, 500.0f, 1.0f, 30.0f);

	Box* box1 = new Box(glm::vec2(10, 1), glm::vec4(1, 0, 0, 1), glm::vec2(-50, 40), glm::vec2(0), 5.0f, 1.0f, 0.3f, 0.3f, true);
	Box* box2 = new Box(glm::vec2(15, 4), glm::vec4(1, 0, 0, 1), glm::vec2(40, 30), glm::vec2(0), 5.0f, 1.0f, 0.3f, 0.3f, false);
	/*Box* box3 = new Box(glm::vec2(40, 1), glm::vec4(1, 0, 0, 1), glm::vec2(0, -40), glm::vec2(0), 5.0f, 1.0f, 0.3f, 0.3f, true);*/

	m_physicsScene->addActor(ball1);
	m_physicsScene->addActor(ball2);
	m_physicsScene->addActor(ball3);
	m_physicsScene->addActor(plane);
	m_physicsScene->addActor(plane2);
	m_physicsScene->addActor(plane3);
	m_physicsScene->addActor(plane4);
	m_physicsScene->addActor(box1);
	m_physicsScene->addActor(box2);
	/*m_physicsScene->addActor(box3);*/
	m_physicsScene->addActor(spring1);
	m_physicsScene->addActor(spring2);

	Rope(7);

	/*ball1->applyForce(glm::vec2(0, -10), ball1->getPosition());
	ball2->applyForce(glm::vec2(0, -10), ball2->getPosition());
	ball3->applyForce(glm::vec2(0, -10), ball3->getPosition());*/

	/*std::vector<std::string> sb;
	sb.push_back(".......................................................");
	sb.push_back("000000..........000000..000000..000000..00...00..000000");
	sb.push_back("000000..........000000..000000..000000..00..00...000000");
	sb.push_back("000000..0...0.......00..00..00..000000..00.00........00");
	sb.push_back("00.....000.000..000000..00..00..00......0000.....000000");
	sb.push_back("00......0...0...00......00..00..00......0000.....00....");
	sb.push_back("000000..........000000..00..00..000000..00.00....000000");
	sb.push_back("000000..........000000..00..00..000000..00..00...000000");
	sb.push_back(".......................................................");*/

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

	if (input->isMouseButtonDown(0))
	{
		int xScreen, yScreen;
		input->getMouseXY(&xScreen, &yScreen);
		glm::vec2 worldPos = screenToWorld(glm::vec2(xScreen, yScreen));

		aie::Gizmos::add2DCircle(worldPos, 5, 32, glm::vec4(1, 0, 1, 1));
	}

	/*if (input->isMouseButtonDown(1))
	{
		int xScreen, yScreen;
		input->getMouseXY(&xScreen, &yScreen);
		glm::vec2 worldPos = screenToWorld(glm::vec2(xScreen, yScreen));

		glm::vec2 p1 = worldPos - worldPos.x * 3 - worldPos.y * 3;
		glm::vec2 p2 = worldPos + worldPos.x * 3 - worldPos.y * 3;
		glm::vec2 p3 = worldPos - worldPos.x * 3 + worldPos.y * 3;
		glm::vec2 p4 = worldPos + worldPos.x * 3 + worldPos.y * 3;
		aie::Gizmos::add2DTri(p1, p2, p4, glm::vec4(1, 0, 1, 1));
		aie::Gizmos::add2DTri(p1, p4, p3, glm::vec4(1, 0, 1, 1));
	}*/

	m_physicsScene->update(deltaTime);
	m_physicsScene->draw();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
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