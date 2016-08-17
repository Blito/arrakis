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
        // Movement
        auto right  = input_system.is_player_doing(actor.controlled_by, Input::Action::RIGHT);
        auto left   = input_system.is_player_doing(actor.controlled_by, Input::Action::LEFT);
        auto down   = input_system.is_player_doing(actor.controlled_by, Input::Action::DOWN);
        auto up     = input_system.is_player_doing(actor.controlled_by, Input::Action::UP);

        // Actions
        auto jump   = input_system.is_player_doing(actor.controlled_by, Input::Action::JUMP);
        auto aiming = input_system.is_player_doing(actor.controlled_by, Input::Action::AIM);

        actor.status = PlayerControlled::Status::IDLE;

        if (collider.airborn)
        {
            actor.status = PlayerControlled::Status::AIRBORN;
        }
        else
        {
            // Player is on the floor
            if (jump)
            {
                physics.force.y = jump_acceleration;
            }

            if (down)
            {
                actor.status = PlayerControlled::Status::DUCKING;
            }
        }

        if (!aiming)
        {
            actor.aim_direction = PlayerControlled::Direction::NONE;

            if (right != left)
            {
                actor.status = PlayerControlled::Status::WALKING;

                physics.velocity.x = right ? lateral_velocity: -lateral_velocity;
            }
            else
            {
                physics.velocity.x = 0.0f;
            }
        }
        else
        {
            if (!((up && down) || (right && left)))
            {
                if (up)
                {
                    if (right)
                    {
                        actor.aim_direction = PlayerControlled::Direction::NE;
                    }
                    else if (left)
                    {
                        actor.aim_direction = PlayerControlled::Direction::NW;
                    }
                    else
                    {
                        actor.aim_direction = PlayerControlled::Direction::N;
                    }
                }
                else if (down)
                {
                    if (right)
                    {
                        actor.aim_direction = PlayerControlled::Direction::SE;
                    }
                    else if (left)
                    {
                        actor.aim_direction = PlayerControlled::Direction::SW;
                    }
                    else
                    {
                        actor.aim_direction = PlayerControlled::Direction::S;
                    }
                }
                else if (right)
                {
                    actor.aim_direction = PlayerControlled::Direction::E;
                }
                else if (left)
                {
                    actor.aim_direction = PlayerControlled::Direction::W;
                }
            }
        }
    });
}
