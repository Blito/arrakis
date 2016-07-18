#ifndef PLAYERCONTROLLERSYSTEM_H
#define PLAYERCONTROLLERSYSTEM_H

#include <entityx/System.h>

namespace arrakis
{

namespace systems
{

class Input;

/**
 * @brief The PlayerControllerSystem transforms input registered in the InputSystem
 * to actions applied to Components owned by the player.
 */
class PlayerController : public entityx::System<PlayerController>
{
public:
    PlayerController(Input & inputSystem);

    void update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt) override;

protected:
    static constexpr float jump_acceleration = 800.0f;
    static constexpr float lateral_acceleration = 70.0f;

    Input & m_inputSystem; //< who should we ask for input
};

} // end systems
} // end arrakis

#endif // PLAYERCONTROLLERSYSTEM_H
