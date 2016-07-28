#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <core/constants.h>
#include <core/message.h>
#include <array>

namespace arrakis
{

namespace systems
{

// The Input system is the source for input events and input state.
//
// Inputs come from the Networking system mostly and are tied to a
// particular Player (or input client, which should be the same).

class Input : public core::MessageReceiver
{
public:
    enum class Action : unsigned short
    {
        UP    = 0,
        LEFT  = 1,
        RIGHT = 2,
        DOWN  = 3,
        JUMP  = 4,
        A     = 5,
        B     = 6,
        PAUSE = 7
    }; // see actions_count

    bool is_player_doing(core::PlayerID player, Action action) const;

    bool is_anyone_doing(Action action) const;

    bool is_playing(core::PlayerID player) const;

    bool is_room_for_new_player() const;

    unsigned int active_players_count() const { return playing_count; }

    // IMPORTANT: Check if there is_room_for_new_player() first.
    core::PlayerID create_new_player();

    void drop_player(core::PlayerID player);

protected:
    // inherited from MessageReceiver
    virtual void notify(core::Message msg, core::PlayerID player) override;

    static constexpr size_t actions_count = 8;
    using Actions = std::array<bool, actions_count>; //< array with active actions
    using Players = std::array<Actions, core::max_players_count>; //< array of Actions, one for each player

    Players players_actions { { { { false } } } }; //< all players with their actions set as not-active

    std::array<bool, core::max_players_count> enabled_players { {false} };

    unsigned int playing_count = 0;

};

} // end systems
} // end arrakis

#endif // INPUTSYSTEM_H
