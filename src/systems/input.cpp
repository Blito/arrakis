#include "input.h"

#include <rapidjson/document.h>

#include <type_traits>
#include <algorithm>
#include <iostream>
#include <assert.h>

using namespace arrakis::systems;

bool Input::is_player_doing(core::Player player, Action action) const
{
    return is_playing(player) && players_actions[player][core::enum_index(action)];
}

bool Input::is_playing(core::Player player) const
{
    return enabled_players[player];
}

bool Input::is_room_for_new_player() const
{
    return playing_count < core::max_players_count;
}

arrakis::core::Player Input::create_new_player()
{
    // TODO: Class interface is not idiot-proof!
    assert(is_room_for_new_player());

    // Guaranteed to find something
    for (unsigned int player_number = 0; player_number < enabled_players.size(); player_number++)
    {
        if (!enabled_players[player_number])
        {
            enabled_players[player_number] = true;
            return player_number;
        }
    }

    return enabled_players.size(); //< program should not reach this
}

void Input::notify(core::Message msg, core::Player player)
{
    using namespace rapidjson;
    using namespace core;

    auto change_action_status = [this] (const std::string & action, core::Player player, bool new_status)
    {
        if (action == "UP")
        {
            players_actions[player][enum_index(Action::UP)] = new_status;
        }
        else if (action == "DOWN")
        {
            players_actions[player][enum_index(Action::DOWN)] = new_status;
        }
        else if (action == "LEFT")
        {
            players_actions[player][enum_index(Action::LEFT)] = new_status;
        }
        else if (action == "RIGHT")
        {
            players_actions[player][enum_index(Action::RIGHT)] = new_status;
        }
        else if (action == "JUMP")
        {
            players_actions[player][enum_index(Action::JUMP)] = new_status;
        }
        else if (action == "A")
        {
            players_actions[player][enum_index(Action::A)] = new_status;
        }
        else if (action == "B")
        {
            players_actions[player][enum_index(Action::B)] = new_status;
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
