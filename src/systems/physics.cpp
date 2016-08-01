#include "physics.h"

#include <components/position.h>
#include <components/physics.h>

using namespace arrakis::systems;

void Physics::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{
    // Update position of entities with Physics component
    update_positions(entities, dt);
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
