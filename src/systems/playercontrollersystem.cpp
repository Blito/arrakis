#include "playercontrollersystem.h"

#include <components/position.h>
#include <systems/inputsystem.h>

using namespace arrakis::systems;

PlayerControllerSystem::PlayerControllerSystem(InputSystem & inputSystem) :
    m_inputSystem(inputSystem)
{

}

void PlayerControllerSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{
    auto up_active = m_inputSystem.isActive(InputSystem::Action::UP);
    entities.each<components::Position>([dt, up_active](entityx::Entity entity, components::Position & position)
    {
        position.y = std::sin(position.y + dt);

        if (up_active)
        {
            position.y++;
        }
    });
}
