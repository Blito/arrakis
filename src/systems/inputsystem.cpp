#include "inputsystem.h"

#include <rapidjson/document.h>

#include <iostream>

using namespace arrakis::systems;

bool InputSystem::isActive(Action action) const
{
    return m_actions[static_cast<short>(action)];
}

void InputSystem::notify(core::Server::Message msg)
{
    using namespace rapidjson;
    auto change_action_status = [this] (const std::string & action, bool new_status)
    {
        if (action == "UP")
        {
            m_actions[static_cast<short>(Action::UP)] = new_status;
        }
        else if (action == "DOWN")
        {
            m_actions[static_cast<short>(Action::DOWN)] = new_status;
        }
        else if (action == "LEFT")
        {
            m_actions[static_cast<short>(Action::LEFT)] = new_status;
        }
        else if (action == "RIGHT")
        {
            m_actions[static_cast<short>(Action::RIGHT)] = new_status;
        }
        else if (action == "SPACE")
        {
            m_actions[static_cast<short>(Action::JUMP)] = new_status;
        }
        else if (action == "A")
        {
            m_actions[static_cast<short>(Action::A)] = new_status;
        }
        else if (action == "B")
        {
            m_actions[static_cast<short>(Action::B)] = new_status;
        }
        else
        {
            std::cout << "received " << action << ", but did not match any pattern." << std::endl;
        }
    };

    Document document;
    document.Parse(msg.payload.c_str()); //< The server handles parsing errors, so this is always valid JSON.

    if (document.HasMember("action"))
    {
        change_action_status(document["action"].GetString(), true);
    }
    else if (document.HasMember("action-stopped"))
    {
        change_action_status(document["action-stopped"].GetString(), false);
    }
}
