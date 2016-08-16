#include "input.h"

#include <rapidjson/document.h>

#include <type_traits>
#include <algorithm>
#include <iostream>
#include <assert.h>

using namespace arrakis::systems;

bool Input::is_player_doing(core::PlayerID player, Action action) const
{
    return is_playing(player) && players_actions[player][core::enum_index(action)];
}

bool Input::is_anyone_doing(Action action) const
{
    return std::any_of(players_actions.begin(), players_actions.end(),
                       [&action](auto & actions) -> bool { return actions[core::enum_index(action)]; });
}

bool Input::is_playing(core::PlayerID player) const
{
    return enabled_players[player];
}

bool Input::is_room_for_new_player() const
{
    return playing_count < core::max_players_count;
}

arrakis::core::PlayerID Input::create_new_player()
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

void Input::drop_player(core::PlayerID player)
{
    auto && player_actions = players_actions[player];
    for (unsigned int i = 0; i < player_actions.size(); i++)
    {
        player_actions[i] = false;
    }

    enabled_players[player] = false;
}

void Input::notify(core::Message msg, core::PlayerID player)
{
    using namespace rapidjson;
    using namespace core;

    switch (msg.type)
    {

    case core::MessageType::ClientDisconnected:
    {
        drop_player(player);
    }
        break;

    case core::MessageType::Input:
    {
        auto change_action_status = [this] (const std::string & action, core::PlayerID player, bool new_status)
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
            else if (action == "AIM")
            {
                players_actions[player][enum_index(Action::AIM)] = new_status;
            }
            else if (action == "DASH")
            {
                players_actions[player][enum_index(Action::DASH)] = new_status;
            }
            else if (action == "PAUSE")
            {
                players_actions[player][enum_index(Action::PAUSE)] = new_status;
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
        break;

    }
}
