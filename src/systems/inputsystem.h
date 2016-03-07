#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <core/messagereceiver.h>

namespace arrakis
{

namespace systems
{

/**
 * @brief The InputSystem is the source for input events and input state.
 *
 * The InputSystem interprets events coming from a core::Server through InputSystem::notify,
 * and updates its internal state.
 */
class InputSystem : public core::MessageReceiver
{
public:
    InputSystem();

protected:
    /**
     * @brief notify Gets notified of Input events coming from the core::Server.
     *
     * @example User presses a key in browser, sends JSON { key:P } to the server,
     * the server redirects it to InputSystem, InputSystem updates its status (P
     * key is now pressed).
     */
    virtual void notify(core::MessageReceiver::Message msg) override;
};

} // end systems
} // end arrakis

#endif // INPUTSYSTEM_H
