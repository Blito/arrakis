#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <core/message.h>
#include <array>

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
    enum class Action : unsigned short
    {
        UP    = 0,
        LEFT  = 1,
        RIGHT = 2,
        DOWN  = 3,
        JUMP  = 4,
        A     = 5,
        B     = 6
    };

    enum class Player : unsigned short
    {
        ONE = 0,
        TWO = 1
    };

    /**
     * @brief isPlayerUsing Checks if a player is playing and if he/she's using an Action
     */
    bool isPlayerUsing(Player player, Action action) const;

    bool isPlaying(Player player) const;

    bool isRoomForNewPlayer() const;

    /**
     * @brief createNewPlayer Creates a new player.
     * IMPORTANT: does not check if this is possible, use isRoomForNewPlayer() first.
     * @return The new player handle
     * @sa isRoomForNewPlayer()
     */
    Player createNewPlayer();

protected:
    /**
     * @brief notify Gets notified of Input events coming from the core::Server.
     *
     * @example User presses a key in browser, sends JSON { key:P } to the server,
     * the server redirects it to InputSystem, InputSystem updates its status (P
     * key is now pressed).
     */
    virtual void notify(core::Message msg) override;

    static constexpr size_t actions_count = 7;
    static constexpr size_t max_players_count = 2;
    using Actions = std::array<bool, actions_count>; // array with active actions
    using Players = std::array<Actions, max_players_count>; // array of Actions, one for each player

    Players m_players { { { { false } } } };

    std::array<bool, max_players_count> m_enabled_players { {false} };

    template<class T>
    inline auto enum_index(T elem) const noexcept
    {
        return static_cast<std::underlying_type_t<decltype(elem)>>(elem);
    }

};

} // end systems
} // end arrakis

#endif // INPUTSYSTEM_H
