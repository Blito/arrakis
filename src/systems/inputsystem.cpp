#include "inputsystem.h"

#include <rapidjson/document.h>

#include <type_traits>
#include <algorithm>
#include <iostream>

using namespace arrakis::systems;

bool InputSystem::isPlayerUsing(core::Player player, Action action) const
{
    return isPlaying(player) && m_players[enum_index(player)][enum_index(action)];
}

bool InputSystem::isPlaying(core::Player player) const
{
    return m_enabled_players[enum_index(player)];
}

bool InputSystem::isRoomForNewPlayer() const
{
    return std::any_of(m_enabled_players.begin(), m_enabled_players.end(),
                       [](bool enabled) { return !enabled; });
}

arrakis::core::Player InputSystem::createNewPlayer()
{
    // This should be better, I don't like that the user should call isRoomForNewPlayer() first.
    // Wait for std::optional to be a reality

    if (!m_enabled_players[0])
    {
        m_enabled_players[0] = true;
        return core::Player::ONE;
    }
    else
    {
        m_enabled_players[1] = true;
        return core::Player::TWO;
    }
}

void InputSystem::notify(core::Message msg, core::Player player)
{
    using namespace rapidjson;
    auto change_action_status = [this] (const std::string & action, core::Player player, bool new_status)
    {
        if (action == "UP")
        {
            m_players[enum_index(player)][enum_index(Action::UP)] = new_status;
        }
        else if (action == "DOWN")
        {
            m_players[enum_index(player)][enum_index(Action::DOWN)] = new_status;
        }
        else if (action == "LEFT")
        {
            m_players[enum_index(player)][enum_index(Action::LEFT)] = new_status;
        }
        else if (action == "RIGHT")
        {
            m_players[enum_index(player)][enum_index(Action::RIGHT)] = new_status;
        }
        else if (action == "SPACE")
        {
            m_players[enum_index(player)][enum_index(Action::JUMP)] = new_status;
        }
        else if (action == "A")
        {
            m_players[enum_index(player)][enum_index(Action::A)] = new_status;
        }
        else if (action == "B")
        {
            m_players[enum_index(player)][enum_index(Action::B)] = new_status;
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
        change_action_status(document["action"].GetString(), player, true);
    }
    else if (document.HasMember("action-stopped"))
    {
        change_action_status(document["action-stopped"].GetString(), player, false);
    }
}
