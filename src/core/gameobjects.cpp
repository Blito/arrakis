#include "gameobjects.h"

#include <components/animation.h>
#include <components/boxcollider.h>
#include <components/physics.h>
#include <components/playercontrolled.h>
#include <components/position.h>
#include <components/rendering.h>

#include <utils/vec2.h>

using namespace arrakis;
using namespace arrakis::components;
using namespace arrakis::core::game_objects;

entityx::Entity player::create(entityx::EntityManager & entity_manager, core::PlayerID player,
                              float position_x, float position_y)
{
    auto new_player = entity_manager.create();

    new_player.assign<Position>(position_x, position_y);

    new_player.assign<Physics>(defaults::mass, true, utils::vec2f{defaults::max_velocity_x, defaults::max_velocity_y});

    new_player.assign<BoxCollider>(BoxCollider::Tag::PLAYER, -defaults::collider_half_width, defaults::collider_half_width,
                                                             -defaults::collider_half_height, defaults::collider_half_height);

    new_player.assign<Rendering>(Rendering::Tag::PLAYER);

    new_player.assign<PlayerControlled>(player);

    return new_player;
}

entityx::Entity arrow::create(entityx::EntityManager & entity_manager,
                              float position_x, float position_y,
                              float velocity_x, float velocity_y)
{
    auto arrow = entity_manager.create();

    arrow.assign<Position>(position_x, position_y);

    // Start with gravity OFF
    auto physics = arrow.assign<Physics>(defaults::mass, false, utils::vec2f{defaults::max_velocity_x, defaults::max_velocity_y});

    physics->velocity = utils::vec2f{velocity_x, velocity_y};

    auto collider = arrow.assign<BoxCollider>(BoxCollider::Tag::ARROW, -defaults::collider_half_width, defaults::collider_half_width,
                                                                       -defaults::collider_half_height, defaults::collider_half_height);

    arrow.assign<Rendering>(Rendering::Tag::ARROW);

    // Turn gravity ON after one second
    arrow.assign<Animation>(physics->has_gravity, 1);

    collider->on_collision = [] (Collision collision)
    {
        if (collision.other_collider.tag == BoxCollider::Tag::PLAYER ||
            collision.other_collider.tag == BoxCollider::Tag::STATIC)
        {
            collision.own_entity.destroy();
        }
    };

    return arrow;
}
