#include "playercontrollersystem.h"

#include <components/playercontrolled.h>
#include <components/position.h>
#include <systems/inputsystem.h>

using namespace arrakis::systems;

PlayerControllerSystem::PlayerControllerSystem(InputSystem & inputSystem) :
    m_inputSystem(inputSystem)
{

}

void PlayerControllerSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{
    using namespace arrakis::components;

    entities.each<PlayerControlled, Position>([this, dt](entityx::Entity entity, PlayerControlled & actor, Position & position)
    {
        auto up_active = m_inputSystem.isPlayerUsing(actor.controlled_by, InputSystem::Action::UP);
        auto right_active = m_inputSystem.isPlayerUsing(actor.controlled_by, InputSystem::Action::RIGHT);

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
