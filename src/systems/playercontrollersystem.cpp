#include "playercontrollersystem.h"

using namespace arrakis::systems;

PlayerControllerSystem::PlayerControllerSystem(InputSystem & inputSystem) :
    m_inputSystem(inputSystem)
{

}

void PlayerControllerSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{

}
