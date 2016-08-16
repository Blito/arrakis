#ifndef PLAYERCONTROLLERSYSTEM_H
#define PLAYERCONTROLLERSYSTEM_H

#include <entityx/System.h>

namespace arrakis
{

namespace systems
{

class Input;

// The PlayerControllerSystem transforms input registered in the InputSystem
// to actions applied to Components owned by the player.
class PlayerController : public entityx::System<PlayerController>
{
public:
    PlayerController(Input & inputSystem);

    void update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt) override;

protected:
    static constexpr float jump_acceleration = 0.009f;
    static constexpr float lateral_velocity = 0.05f;

    Input & input_system; //< who should we ask for input
};

} // end systems
} // end arrakis

#endif // PLAYERCONTROLLERSYSTEM_H
