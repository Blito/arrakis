#include "physicssystem.h"

#include <components/position.h>
#include <components/physics.h>

using namespace arrakis::systems;

void PhysicsSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{
    using namespace arrakis::components;

    entities.each<Position, Physics>([this, dt](entityx::Entity entity, Position & position, Physics & physics)
    {
        float _dt = dt / 1000.0f;

        updateVelocity(physics, _dt);

        // update position
        position.x += physics.velocity.x * _dt;
        position.y += physics.velocity.y * _dt;

        bool collided;
        keep_in_world_bounds(position.x, position.y, collided);

        if (collided)
        {
            physics.velocity.x = 0.0f;
        }

        // if it collided with ground
        if(position.y == world_bounds_y.min)
        {
            physics.velocity.y = 0.0f;
        }
    });
}

void PhysicsSystem::updateAcceleration(components::Physics & physics, entityx::TimeDelta dt)
{
}

void PhysicsSystem::updateVelocity(components::Physics & physics, entityx::TimeDelta dt)
{
    auto keep_in_bounds = [](float & magnitude, float abs_bound)
    {
        if (magnitude < -abs_bound)
        {
            magnitude = -abs_bound;
        }
        else if (magnitude > abs_bound)
        {
            magnitude = abs_bound;
        }
    };

    physics.velocity += physics.acceleration * dt;
    if (physics.has_gravity)
    {
        physics.velocity.y += gravity * dt;
    }

    keep_in_bounds(physics.velocity.x, physics.max_velocity.x);
    keep_in_bounds(physics.velocity.y, physics.max_velocity.y);
    round_to_static(physics.velocity.x, 0.00001f);
    round_to_static(physics.velocity.y, 0.00001f);

    if (physics.velocity.x != 0)
    {
        physics.velocity.x /= physics.friction.x;
    }
}

void PhysicsSystem::keep_in_world_bounds(float & x, float & y, bool & collided)
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

void PhysicsSystem::round_to_static(float & magnitude, float threshold)
{
    if (std::abs(magnitude) < threshold)
    {
        magnitude = 0.0f;
    }
}
