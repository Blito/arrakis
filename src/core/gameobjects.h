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
        constexpr float max_velocity_x = 0.5;
        constexpr float max_velocity_y = 0.5;
        constexpr float collider_half_width = 10;
        constexpr float collider_half_height = 10;
    }

    entityx::Entity create(entityx::EntityManager & entity_manager, PlayerID player,
                           float position_x, float position_y);
}

namespace arrow
{

    namespace defaults {
        constexpr float mass = 1.0f;
        constexpr float max_velocity_x = 0.6;
        constexpr float max_velocity_y = 0.6;
        constexpr float collider_half_width = 4;
        constexpr float collider_half_height = 4;
    }

    entityx::Entity create(entityx::EntityManager & entity_manager,
                           float position_x, float position_y,
                           float velocity_x, float velocity_y);
}

}

} // end core
} // end arrakis

#endif // GAMEOBJECTS_H
