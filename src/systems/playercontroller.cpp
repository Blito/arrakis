#include "playercontroller.h"

#include <components/animation.h>
#include <components/boxcollider.h>
#include <components/playercontrolled.h>
#include <components/physics.h>
#include <components/position.h>
#include <components/rendering.h>

#include <core/gameobjects.h>

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
            character.ammo--;
        }

        character.status = Status::IDLE;

        if (right != left)
        {
            character.facing_left = left;
        }

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
            // If aiming and not free-falling, stop the player
            if (character.status != Status::AIRBORN)
            {
                physics.velocity.x = 0.0f;
            }

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
                else
                {
                    // no key being pressed, fire at facing direction
                    character.aim_direction = character.facing_left ? Direction::W : Direction::E;
                }
            }
        }
    });
}

void PlayerController::spawn_arrow(entityx::EntityManager & entity_manager, Position & player_position, PlayerControlled & player_info) const
{
    constexpr float full_speed = 0.2f;
    constexpr float diag_speed = full_speed * std::sin(3.14159 / 4.0);

    utils::vec2f arrow_velocity;
    float starting_x = player_position.x, starting_y = player_position.y, starting_delta = 15;
    auto& direction = player_info.aim_direction;
    switch (direction)
    {
        case PlayerControlled::Direction::N:
            arrow_velocity = {0.0f, full_speed};
            starting_y += starting_delta;
        break;
        case PlayerControlled::Direction::NE:
            arrow_velocity = {diag_speed, diag_speed};
            starting_y += starting_delta;
            starting_x += starting_delta;
        break;
        case PlayerControlled::Direction::E:
            arrow_velocity = {full_speed, 0.0f};
            starting_x += starting_delta;
        break;
        case PlayerControlled::Direction::SE:
            arrow_velocity = {diag_speed, -diag_speed};
            starting_y -= starting_delta;
            starting_x += starting_delta;
        break;
        case PlayerControlled::Direction::S:
            arrow_velocity = {0.0f, -full_speed};
            starting_y -= starting_delta;
        break;
        case PlayerControlled::Direction::SW:
            arrow_velocity = {-diag_speed, -diag_speed};
            starting_y -= starting_delta;
            starting_x -= starting_delta;
        break;
        case PlayerControlled::Direction::W:
            arrow_velocity = {-full_speed, 0.0f};
            starting_x -= starting_delta;
        break;
        case PlayerControlled::Direction::NW:
            arrow_velocity = {-diag_speed, diag_speed};
            starting_y += starting_delta;
            starting_x -= starting_delta;
        break;
    }

    core::game_objects::arrow::create(entity_manager, starting_x, starting_y, arrow_velocity.x, arrow_velocity.y);
}
