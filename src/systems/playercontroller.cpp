#include "playercontroller.h"

#include <components/boxcollider.h>
#include <components/playercontrolled.h>
#include <components/physics.h>
#include <systems/input.h>

using namespace arrakis::systems;

PlayerController::PlayerController(Input & inputSystem) :
    input_system(inputSystem)
{

}

void PlayerController::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{
    using namespace arrakis::components;

    entities.each<PlayerControlled, Physics, BoxCollider>([this, dt](entityx::Entity entity, PlayerControlled & actor, Physics & physics, BoxCollider & collider)
    {
        auto jump = input_system.is_player_doing(actor.controlled_by, Input::Action::JUMP);
        auto right = input_system.is_player_doing(actor.controlled_by, Input::Action::RIGHT);
        auto left = input_system.is_player_doing(actor.controlled_by, Input::Action::LEFT);

        if (!collider.airborn && jump)
        {
            physics.force.y = 0.008f;
            physics.velocity.y = 0.01;
        }

//        physics.velocity.x = 0.05;

//        if (right != left)
//        {
//            physics.velocity.x = right ? lateral_acceleration : -lateral_acceleration;
//        }
//        else
//        {
//            physics.velocity.x = 0.0f;
//        }
    });
}
