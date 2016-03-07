#ifndef PLAYERCONTROLLERSYSTEM_H
#define PLAYERCONTROLLERSYSTEM_H

#include <entityx/System.h>

namespace arrakis
{

namespace systems
{

class InputSystem;

/**
 * @brief The PlayerControllerSystem transforms input registered in the InputSystem
 * to actions applied to Components owned by the player.
 */
class PlayerControllerSystem : public entityx::System<PlayerControllerSystem>
{
public:
    PlayerControllerSystem(InputSystem & inputSystem);

    void update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt) override;

protected:
    InputSystem & m_inputSystem; //< who should we ask for input
};

} // end systems
} // end arrakis

#endif // PLAYERCONTROLLERSYSTEM_H
