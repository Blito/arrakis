#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <iostream>

#include <core/messagereceiver.h>

namespace arrakis
{

namespace systems
{

class InputSystem : public core::MessageReceiver
{
public:
    InputSystem();

protected:
    virtual void notify(core::MessageReceiver::Message msg) override
    {
        std::cout << "InputSystem: " << msg << std::endl;
    }
};

} // end systems
} // end arrakis

#endif // INPUTSYSTEM_H
