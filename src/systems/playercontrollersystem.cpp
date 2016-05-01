#include "playercontrollersystem.h"

#include <components/playercontrolled.h>
#include <components/physics.h>
#include <systems/inputsystem.h>

using namespace arrakis::systems;

PlayerControllerSystem::PlayerControllerSystem(InputSystem & inputSystem) :
    m_inputSystem(inputSystem)
{

}

void PlayerControllerSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{
    using namespace arrakis::components;

    entities.each<PlayerControlled, Physics>([this, dt](entityx::Entity entity, PlayerControlled & actor, Physics & physics)
    {
        auto jump = m_inputSystem.isPlayerUsing(actor.controlled_by, InputSystem::Action::JUMP);
        auto right = m_inputSystem.isPlayerUsing(actor.controlled_by, InputSystem::Action::RIGHT);
        auto left = m_inputSystem.isPlayerUsing(actor.controlled_by, InputSystem::Action::LEFT);

        if (jump && physics.velocity.y == 0.0f)
        {
            physics.acceleration.y = jump_acceleration;
        }
        else
        {
            physics.acceleration.y = 0.0f;
        }

        if (right != left)
        {
            physics.acceleration.x = right ? lateral_acceleration : -lateral_acceleration;
        }
        else
        {
            physics.acceleration.x = 0.0f;
        }
    });
}
