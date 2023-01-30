#include "PhysicsScene.h"
#include "Sphere.h"

PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0, 0))
{
}

PhysicsScene::~PhysicsScene()
{
    for (auto pActor : m_actors)
    {
        delete pActor;
    }
}

void PhysicsScene::addActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::removeActor(PhysicsObject* actor)
{
    // check to see if the actor is in m_actors
    std::vector<PhysicsObject*>::iterator iterator;
    iterator = std::find(m_actors.begin(), m_actors.end(), actor);

    if (iterator != m_actors.end())
    {
        m_actors.erase(remove(m_actors.begin(), m_actors.end(), actor), m_actors.end());
    }
}

void PhysicsScene::update(float dt)
{
    // update physics at a fixed time step

    static float accumulatedTime = 0.0f;
    accumulatedTime += dt;

    while (accumulatedTime >= m_timeStep)
    {
        for (auto pActor : m_actors)
        {
            pActor->fixedUpdate(m_gravity, m_timeStep);
        }

        accumulatedTime -= m_timeStep;
    }

    // check for collisions (ideally you'd want to have some sort of 
        // scene management in place)
    int actorCount = m_actors.size();

    // need to check for collisions against all objects except this one.
    for (int outer = 0; outer < actorCount - 1; outer++)
    {
        for (int inner = outer + 1; inner < actorCount; inner++)
        {
            PhysicsObject* object1 = m_actors[outer];
            PhysicsObject* object2 = m_actors[inner];

            // for now we can assume both shapes are spheres, 
            // since that is all we’ve implemented for now.
            sphere2Sphere(object1, object2);
        }
    }
}

void PhysicsScene::draw()
{
    for (auto pActor : m_actors) {
        pActor->draw();
    }
}

bool PhysicsScene::sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
    // try to cast objects to sphere and sphere
    Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
    Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);
    // if we are successful then test for collision
    if (sphere1 != nullptr && sphere2 != nullptr)
    {
        // TODO do the necessary maths in here
        // TODO if the spheres touch, set their velocities to zero for now
        glm::distance(sphere1, sphere2);

        return true;
    }

    return  false;
}
