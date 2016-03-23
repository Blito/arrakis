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

/**
 * @brief The MessageReceiver class is a simple Observer interface.
 * @sa arrakis::core::Server::registerTo(...)
 */
class MessageReceiver
{
public:
    virtual void notify(Message msg) = 0;
};

} // end core
} // end arrakis

#endif // MESSAGE_H
