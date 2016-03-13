#include "inputsystem.h"

#include <iostream>

using namespace arrakis::systems;

InputSystem::InputSystem()
{

}

bool InputSystem::isActive(Action action) const
{
    return m_actions[static_cast<short>(action)];
}

void InputSystem::notify(core::Server::Message msg)
{
    std::cout << "InputSystem: " << msg.payload << std::endl;

    const std::string limit_chars = ",}";

    auto change_action_status =
            [&limit_chars, this] (const std::string & action_tag, const std::string & msg, bool new_status)
            {
                auto action_begin_pos = msg.find(action_tag);
                if (action_begin_pos == std::string::npos)
                {
                    return;
                }
                action_begin_pos += action_tag.size();
                auto action_end_pos = msg.find_first_of(limit_chars, action_begin_pos);
                if (action_end_pos == std::string::npos)
                {
                    return;
                }

                std::string action = msg.substr(action_begin_pos, action_end_pos - action_begin_pos);

                std::cout << action << std::endl;

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
                    std::cout << "received " << msg << ", but did not match any pattern." << std::endl;
                }
            };

    change_action_status("action:", msg.payload, true);
    change_action_status("action-stopped:", msg.payload, false);
}
