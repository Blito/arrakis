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
        using Status = PlayerControlled::Status;
        using Direction = PlayerControlled::Direction;

        // Movement
        auto right  = input_system.is_player_doing(actor.controlled_by, Input::Action::RIGHT);
        auto left   = input_system.is_player_doing(actor.controlled_by, Input::Action::LEFT);
        auto down   = input_system.is_player_doing(actor.controlled_by, Input::Action::DOWN);
        auto up     = input_system.is_player_doing(actor.controlled_by, Input::Action::UP);

        // Actions
        auto jump   = input_system.is_player_doing(actor.controlled_by, Input::Action::JUMP);
        auto aiming = input_system.is_player_doing(actor.controlled_by, Input::Action::AIM);

        actor.status = Status::IDLE;

        if (collider.airborn)
        {
            actor.status = Status::AIRBORN;
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
                actor.status = Status::DUCKING;
            }
        }

        if (!aiming)
        {
            actor.aim_direction = Direction::NONE;

            if (right != left)
            {
                if (actor.status != Status::AIRBORN)
                {
                    actor.status = Status::WALKING;
                }

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
                        actor.aim_direction = Direction::NE;
                    }
                    else if (left)
                    {
                        actor.aim_direction = Direction::NW;
                    }
                    else
                    {
                        actor.aim_direction = Direction::N;
                    }
                }
                else if (down)
                {
                    if (right)
                    {
                        actor.aim_direction = Direction::SE;
                    }
                    else if (left)
                    {
                        actor.aim_direction = Direction::SW;
                    }
                    else
                    {
                        actor.aim_direction = actor.status == Status::DUCKING ? Direction::NONE : Direction::S;
                    }
                }
                else if (right)
                {
                    actor.aim_direction = Direction::E;
                }
                else if (left)
                {
                    actor.aim_direction = Direction::W;
                }
            }
        }
    });
}
