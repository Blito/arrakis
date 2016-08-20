#include "playercontroller.h"

#include <components/boxcollider.h>
#include <components/playercontrolled.h>
#include <components/physics.h>
#include <components/position.h>
#include <components/rendering.h>
#include <systems/input.h>

using namespace arrakis::systems;
using namespace arrakis::components;

PlayerController::PlayerController(Input & inputSystem) :
    input_system(inputSystem)
{

}

void PlayerController::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{
    entities.each<PlayerControlled, Physics, BoxCollider>([this, dt, &entities](entityx::Entity entity, PlayerControlled & character, Physics & physics, BoxCollider & collider)
    {
        using Status = PlayerControlled::Status;
        using Direction = PlayerControlled::Direction;

        // Movement
        auto right  = input_system.is_player_doing(character.controlled_by, Input::Action::RIGHT);
        auto left   = input_system.is_player_doing(character.controlled_by, Input::Action::LEFT);
        auto down   = input_system.is_player_doing(character.controlled_by, Input::Action::DOWN);
        auto up     = input_system.is_player_doing(character.controlled_by, Input::Action::UP);

        // Actions
        auto jump   = input_system.is_player_doing(character.controlled_by, Input::Action::JUMP);
        auto aiming = input_system.is_player_doing(character.controlled_by, Input::Action::AIM);

        // If the player released the AIM command and has ammo, fire.
        auto should_fire = !aiming && character.aim_direction != Direction::NONE && character.ammo > 0;
        if (should_fire)
        {
            spawn_arrow(entities, *entity.component<Position>().get(), character);
        }

        character.status = Status::IDLE;

        if (collider.airborn)
        {
            character.status = Status::AIRBORN;
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
                character.status = Status::DUCKING;
            }
        }

        if (!aiming)
        {
            character.aim_direction = Direction::NONE;

            if (right != left)
            {
                if (character.status != Status::AIRBORN)
                {
                    character.status = Status::WALKING;
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
                        character.aim_direction = Direction::NE;
                    }
                    else if (left)
                    {
                        character.aim_direction = Direction::NW;
                    }
                    else
                    {
                        character.aim_direction = Direction::N;
                    }
                }
                else if (down)
                {
                    if (right)
                    {
                        character.aim_direction = Direction::SE;
                    }
                    else if (left)
                    {
                        character.aim_direction = Direction::SW;
                    }
                    else
                    {
                        character.aim_direction = character.status == Status::DUCKING ? Direction::NONE : Direction::S;
                    }
                }
                else if (right)
                {
                    character.aim_direction = Direction::E;
                }
                else if (left)
                {
                    character.aim_direction = Direction::W;
                }
            }
        }
    });
}

void PlayerController::spawn_arrow(entityx::EntityManager & entity_manager, Position & player_position, PlayerControlled & player_info) const
{
    auto arrow = entity_manager.create();

    arrow.assign<Position>(player_position.x+10, player_position.y+5);
    auto physics = arrow.assign<Physics>(1.0f, true, utils::vec2f{0.6f, 0.6f});
    physics->velocity = utils::vec2f{0.005f, 0.01f};
    arrow.assign<BoxCollider>(-4, 4, -4, 4);
    arrow.assign<Rendering>(Rendering::Tag::ARROW);
}
