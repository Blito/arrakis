#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

namespace arrakis
{

namespace core
{

enum class MessageType
{
    NewClient,
    ClientDisconnected,
    Input,
    Output,
    ParseError
};

struct Message
{
    MessageType type;
    std::string payload;
};

typedef unsigned short PlayerID;

// Example: enum_index(Player::ONE) == 0
template<class T>
inline auto enum_index(T elem) noexcept
{
    return static_cast<std::underlying_type_t<decltype(elem)>>(elem);
}

// Simple Observer interface.
class MessageReceiver
{
public:
    virtual void notify(Message msg, PlayerID player) = 0;
};

} // end core
} // end arrakis

#endif // MESSAGE_H
