#include "physics.h"

#include <components/boxcollider.h>
#include <components/position.h>
#include <components/physics.h>

#include <array>
#include <limits>

using namespace arrakis::systems;

void Physics::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{
    // Update position of entities with Physics component
    update_positions(entities, dt);

    check_collisions(entities, dt);
}

void Physics::update_positions(entityx::EntityManager &entities, entityx::TimeDelta dt)
{
    using namespace arrakis::components;
    entities.each<Position, components::Physics>([dt](entityx::Entity entity, Position & position, components::Physics & physics)
    {
        float _dt = dt;

        if (physics.mass == 0)
        {
            return;
        }

        // force -> acceleration
        physics.acceleration = physics.force / physics.mass;
        if (physics.has_gravity)
        {
            physics.acceleration.y += gravity;
        }
        physics.force = { 0.0f, 0.0f };

        // acceleration -> velocity
        physics.velocity.x = std::min(physics.acceleration.x * _dt + physics.velocity.x, physics.max_velocity.x);
        physics.velocity.y = std::min(physics.acceleration.y * _dt + physics.velocity.y, physics.max_velocity.y);

        // velocity -> position
        position.x += physics.velocity.x * _dt;
        position.y += physics.velocity.y * _dt;
    });
}

void Physics::check_collisions(entityx::EntityManager & entities, entityx::TimeDelta dt)
{
    using namespace arrakis::components;

    entityx::ComponentHandle<Position> position_1, position_2;
    entityx::ComponentHandle<BoxCollider> collider_1, collider_2;

    auto entities_1 = entities.entities_with_components(position_1, collider_1);
    auto entities_2 = entities.entities_with_components(position_2, collider_2);

    // Naive approach, O(n^2)
    for (entityx::Entity entity_1 : entities_1) {

        auto left_1  = position_1->x + collider_1->x_min;
        auto right_1 = position_1->x + collider_1->x_max;
        auto down_1  = position_1->y + collider_1->y_min;
        auto up_1    = position_1->y + collider_1->y_max;

        for (entityx::Entity entity_2 : entities_2) {
            if (entity_1 != entity_2)
            {
                auto left_2  = position_2->x + collider_2->x_min;
                auto right_2 = position_2->x + collider_2->x_max;
                auto down_2  = position_2->y + collider_2->y_min;
                auto up_2    = position_2->y + collider_2->y_max;

                bool inside_horizontal = left_1 < left_2  &&  left_2 < right_1 ||
                                         left_1 < right_2 && right_2 < right_1;

                bool inside_vertical = down_1 < up_2   &&   up_2 < up_1 ||
                                       down_1 < down_2 && down_2 < up_1;

                bool collided = inside_horizontal && inside_vertical;

                if (collided)
                {
                    solve_collision(entity_1, entity_2, position_1, position_2, collider_1, collider_2, inside_vertical, inside_horizontal);
                }
            }
        }
    }
}

void Physics::solve_collision(entityx::Entity & entity_1,
                              entityx::Entity & entity_2,
                              entityx::ComponentHandle<arrakis::components::Position> & position_1,
                              entityx::ComponentHandle<arrakis::components::Position> & position_2,
                              const entityx::ComponentHandle<arrakis::components::BoxCollider> & collider_1,
                              const entityx::ComponentHandle<arrakis::components::BoxCollider> & collider_2,
                              bool vertical_collision,
                              bool horizontal_collision)
{
    // Correct the offending object. If one of the objects is static, that should be the offended.

    auto & offending_collider = collider_2->is_static ? collider_1 : collider_2;
    auto & offending_position = collider_2->is_static ? position_1 : position_2;
    auto & offending_entity   = collider_2->is_static ? entity_1 : entity_2;

    auto & offended_collider = collider_2->is_static ? collider_2 : collider_1;
    auto & offended_position = collider_2->is_static ? position_2 : position_1;

    auto left_offended  = offended_position->x + offended_collider->x_min;
    auto right_offended = offended_position->x + offended_collider->x_max;
    auto down_offended  = offended_position->y + offended_collider->y_min;
    auto up_offended    = offended_position->y + offended_collider->y_max;

    auto left_offending  = offending_position->x + offending_collider->x_min;
    auto right_offending = offending_position->x + offending_collider->x_max;
    auto down_offending  = offending_position->y + offending_collider->y_min;
    auto up_offending    = offending_position->y + offending_collider->y_max;

    float horizontal_penetration = std::numeric_limits<float>::max();
    float vertical_penetration   = std::numeric_limits<float>::max();

    if (horizontal_collision && offending_collider->enabled_horizontal)
    {
        if (right_offending > right_offended) // penetrated from the right side
        {
            horizontal_penetration = right_offended - left_offending;
        }
        else if (left_offending < left_offended) // penetrated from the left side
        {
            horizontal_penetration = left_offended - right_offending;
        }
    }

    if (vertical_collision && offending_collider->enabled_vertical)
    {
        if (up_offending > up_offended) // penetrated from above
        {
            vertical_penetration = up_offended - down_offending;
        }
        else if (down_offending < down_offended) // penetrated from below  ( ͡° ͜ʖ ͡°)
        {
            vertical_penetration = down_offended - up_offending;
        }
    }

    if (std::abs(horizontal_penetration) < std::abs(vertical_penetration))
    {
        offending_position->x += horizontal_penetration;

        auto physics_comp = offending_entity.component<components::Physics>();
        if (physics_comp)
        {
            physics_comp->acceleration.x = 0.0f;
            physics_comp->velocity.x = 0.0f;
        }
    }
    else
    {
        offending_position->y += vertical_penetration;

        auto physics_comp = offending_entity.component<components::Physics>();
        if (physics_comp)
        {
            physics_comp->acceleration.y = 0.0f;
            physics_comp->velocity.y = 0.0f;
        }
    }


}

void Physics::keep_in_world_bounds(float & x, float & y, bool & collided)
{
    collided = false;
    if (x < world_bounds_x.min)
    {
        collided = true;
        x = world_bounds_x.min;
    }
    else if (x > world_bounds_x.max)
    {
        collided = true;
        x = world_bounds_x.max;
    }

    if (y < world_bounds_y.min)
    {
        y = world_bounds_y.min;
    }
    else if (y > world_bounds_y.max)
    {
        y = world_bounds_y.max;
    }
}

void Physics::round_to_static(float & magnitude, float threshold)
{
    if (std::abs(magnitude) < threshold)
    {
        magnitude = 0.0f;
    }
}
