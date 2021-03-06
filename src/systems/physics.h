#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <entityx/System.h>

#include <components/boxcollider.h>
#include <components/physics.h>
#include <components/position.h>

#include <utils/vec2.h>

#include <array>

namespace arrakis
{

namespace systems
{

// The Physics system updates entities' position in the world using a
// basic homemade physics system.
class Physics : public entityx::System<Physics>
{
public:
    void update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt) override;

protected:
    void update_positions(entityx::EntityManager & entities, entityx::TimeDelta dt);

    void check_collisions(entityx::EntityManager & entities, entityx::TimeDelta dt);

    void solve_collision(entityx::Entity & entity_1,
                         entityx::Entity & entity_2,
                         entityx::ComponentHandle<components::Position> & position_1,
                         entityx::ComponentHandle<components::Position> & position_2,
                         const entityx::ComponentHandle<components::BoxCollider> & collider_1,
                         const entityx::ComponentHandle<components::BoxCollider> & collider_2,
                         bool vertical_collision,
                         bool horizontal_collision);

    // if out of world bounds, loop elements over it.
    void loop_over_world(components::Position & position) const;

    bool types_collide(arrakis::components::BoxCollider::Tag type_1,
                       arrakis::components::BoxCollider::Tag type_2) const;

    static constexpr float gravity = -0.000098f; // [pixels / ms]
    static constexpr float static_threshold = 0.000001f;

    struct { float min, max; } world_bounds_x {0, 500}, world_bounds_y {0, 500};

    bool collision_matrix[arrakis::components::BoxCollider::tag_count][arrakis::components::BoxCollider::tag_count] =
    {
    /*              PLAYER  ARROW  POWERUP  STATIC */
    /* PLAYER  */ { false,  true,   true,    true},
    /* ARROW   */ { true,   false,  false,   true},
    /* POWERUP */ { true,   false,  false,   false},
    /* STATIC  */ { true,   true,   false,   false}
    };
};

} // end systems
} // end arrakis

#endif // PHYSICSSYSTEM_H
