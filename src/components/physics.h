#ifndef PHYSICS_H
#define PHYSICS_H

#include <utils/vec2.h>

namespace arrakis
{

namespace components
{

struct Physics
{
    Physics(float mass = 1.0f,
            bool has_gravity = true,
            utils::vec2f friction = {0.0f, 0.0f},
            utils::vec2f max_velocity = {1.0f, 1.0f},
            utils::vec2f velocity = {0.0f, 0.0f},
            utils::vec2f acceleration = {0.0f, 0.0f})
        : friction(friction), velocity(velocity), acceleration(acceleration), max_velocity(max_velocity), mass(mass), has_gravity(has_gravity) {}

    utils::vec2f friction, velocity, acceleration, max_velocity;
    float mass;
    bool has_gravity;
};

} // end components
} // end arrakis

#endif // PHYSICS_H
