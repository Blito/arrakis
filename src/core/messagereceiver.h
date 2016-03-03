#ifndef MESSAGERECEIVER_H
#define MESSAGERECEIVER_H

#include <string>

namespace arrakis
{

namespace  core
{

/**
 * @brief The MessageReceiver class is a simple Observer interface.
 * @sa arrakis::core::Server::registerTo(...)
 */
class MessageReceiver
{
public:
    using Message = std::string;

    virtual void notify(Message msg) = 0;
};

} // end core
} // end arrakis

#endif // MESSAGERECEIVER_H
