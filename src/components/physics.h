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
            utils::vec2f max_velocity = {0.01f, 0.01f})
        : mass(mass),
          force(force),
          acceleration(acceleration),
          velocity(velocity),
          max_velocity(max_velocity),
          has_gravity(has_gravity) {}


    float           mass;
    utils::vec2f    force = { 0.0f, 0.0f };
    utils::vec2f    acceleration = { 0.0f, 0.0f };
    utils::vec2f    velocity = { 0.0f, 0.0f };
    utils::vec2f    max_velocity;
    bool            has_gravity;
};

} // end components
} // end arrakis

#endif // PHYSICS_H
