#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "entityx/Entity.h"

namespace arrakis
{

namespace components
{

struct Collision
{
    Collision(entityx::Entity & own_entity,
              const struct BoxCollider & other_collider,
              const struct Position & other_position,
              entityx::Entity & other_entity)
        : own_entity(own_entity),
          other_collider(other_collider),
          other_position(other_position),
          other_entity(other_entity) {}

    entityx::Entity & own_entity;
    const BoxCollider & other_collider;
    const Position & other_position;
    entityx::Entity & other_entity;
};

struct BoxCollider
{
    enum class Tag
    {
        PLAYER   = 0,
        ARROW    = 1,
        POWER_UP = 2,
        STATIC   = 3
    };
    constexpr static size_t tag_count = 4;

    BoxCollider(Tag tag, float x_min, float x_max, float y_min, float y_max,
                bool enabled_vertical = true, bool enabled_horizontal = true,
                bool is_static = false)
        : tag(tag),
          x_min(x_min),
          x_max(x_max),
          y_min(y_min),
          y_max(y_max),
          enabled_vertical(enabled_vertical),
          enabled_horizontal(enabled_horizontal),
          is_static(is_static) {}

    Tag tag;

    bool is_static, enabled_vertical, enabled_horizontal;
    float x_min, x_max, y_min, y_max;

    std::function<void(Collision)> on_collision = nullptr;

    bool airborn;
};

} // end components
} // end arrakis

#endif // BOXCOLLIDER_H
