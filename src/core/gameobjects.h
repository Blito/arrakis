#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <entityx/Entity.h>

#include <core/message.h> // core::PlayerID

namespace arrakis
{

namespace core
{

namespace game_objects
{

namespace player
{
    namespace defaults
    {
        constexpr float mass = 5.0f;
        constexpr float max_velocity_x = 0.5f;
        constexpr float max_velocity_y = 0.5f;
        constexpr float collider_half_width = 10.0f;
        constexpr float collider_half_height = 10.0f;
    }

    entityx::Entity create(entityx::EntityManager & entity_manager, PlayerID player,
                           float position_x, float position_y);
}

namespace arrow
{

    namespace defaults
    {
        constexpr float mass = 1.0f;
        constexpr float max_velocity_x = 0.6f;
        constexpr float max_velocity_y = 0.6f;
        constexpr float collider_half_width = 4.0f;
        constexpr float collider_half_height = 4.0f;
    }

    entityx::Entity create(entityx::EntityManager & entity_manager,
                           float position_x, float position_y,
                           float velocity_x, float velocity_y);
}

namespace powerup
{

    namespace defaults
    {
        constexpr float mass = 5.0f;
        constexpr float collider_half_width = 10.0f;
        constexpr float collider_half_height = 10.0f;
    }

    entityx::Entity create(entityx::EntityManager & entity_manager,
                           float position_x, float position_y);

}

} // end game_objects

} // end core
} // end arrakis

#endif // GAMEOBJECTS_H
