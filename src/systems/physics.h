#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <entityx/System.h>

#include <components/physics.h>

#include <utils/vec2.h>

namespace arrakis
{

namespace systems
{

/**
 * @brief The PhysicsSystem updates entities' position in the world using a
 * basic homemade physics system.
 */
class Physics : public entityx::System<Physics>
{
public:
    void update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt) override;

protected:
    // Adds friction based on the current acceleration
    void updateAcceleration(components::Physics & physics, entityx::TimeDelta dt);

    // Updates velocity using updated acceleration
    void updateVelocity(components::Physics & physics, float dt);

    // Keeps x and y between world_bounds_x and world_bounds_y
    void keep_in_world_bounds(float & x, float & y, bool &collided);

    // Rounds magnitude to 0 of lower than abs(static_threshold)
    void round_to_static(float & magnitude, float threshold);

    static constexpr float gravity = -9.8f;
    static constexpr float static_threshold = 0.000001f;

    struct { float min, max; } world_bounds_x {0, 500}, world_bounds_y {0, 500};
};

} // end systems
} // end arrakis

#endif // PHYSICSSYSTEM_H
