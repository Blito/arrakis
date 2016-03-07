#include "playercontrollersystem.h"

#include <components/position.h>

using namespace arrakis::systems;

PlayerControllerSystem::PlayerControllerSystem(InputSystem & inputSystem) :
    m_inputSystem(inputSystem)
{

}

void PlayerControllerSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{
    entities.each<components::Position>([dt](entityx::Entity entity, components::Position & position)
    {
        position.y = std::sin(position.y + dt);
    });
}
