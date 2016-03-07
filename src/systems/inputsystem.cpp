#include "inputsystem.h"

#include <iostream>

using namespace arrakis::systems;

InputSystem::InputSystem()
{

}

void InputSystem::notify(core::MessageReceiver::Message msg)
{
    std::cout << "InputSystem: " << msg << std::endl;
}
