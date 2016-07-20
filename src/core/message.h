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
    Input,
    Output,
    ParseError
};

struct Message
{
    MessageType type;
    std::string payload;
};

enum class Player : unsigned short
{
    ONE = 0,
    TWO = 1,
    NA = 2 // used as a flag for non-valid operations
};

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
    virtual void notify(Message msg, Player player) = 0;
};

} // end core
} // end arrakis

#endif // MESSAGE_H
