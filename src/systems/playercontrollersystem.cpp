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
    auto up_active = m_inputSystem.isPlayerUsing(core::Player::ONE, InputSystem::Action::UP);

    auto right_active = m_inputSystem.isPlayerUsing(core::Player::TWO, InputSystem::Action::RIGHT);

    entities.each<components::Position>([dt, up_active, right_active](entityx::Entity entity, components::Position & position)
    {
        if (up_active)
        {
            position.y++;
        }
        else
        {
            if (position.y > 0)
            {
                position.y--;
            }
        }

        if (right_active)
        {
            position.x++;
        }
    });
}
