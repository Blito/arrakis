#ifndef SERVER_H
#define SERVER_H

#include <functional>
#include <unordered_map>
#include <memory>

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

#include <rapidjson/document.h>

#include <core/message.h>
#include <systems/input.h>

namespace arrakis
{

namespace core
{
class MessageReceiver;
}

namespace systems
{

/**
 * @brief The Server class represents a game server.
 *
 * The server handles basic networking setup, and delegates message
 * processing to other classes (like arrakis::systems::InputSystem).
 */
class Networking
{
public:
    /**
     * @brief Server Sets up the server listening at the given port.
     */
    Networking(systems::Input & input_system, int port = 9002);

    /**
     * @brief run Starts the server.
     */
    void run();

    /**
     * @brief sendMessage Sends a message to all registered receivers that are
     * interested in msg.MessageType.
     */
    void sendMessage(const core::Message & msg);

    /**
     * @brief registerTo Registers a receiver to be notified when a message of
     * a specific type reaches the server.
     */
    void registerTo(core::MessageType msgType, core::MessageReceiver & receiver);

protected:
    using server = websocketpp::server<websocketpp::config::asio>;
    using client = websocketpp::connection_hdl;
    using clients = std::map<client, core::Player, std::owner_less<websocketpp::connection_hdl>>;

    /**
     * We need this to store enum classes as unordered_map keys.
     */
    struct EnumClassHash
    {
        template <typename T>
        std::size_t operator()(T t) const
        {
            return static_cast<std::size_t>(t);
        }
    };
    using listeners = std::unordered_multimap<core::MessageType, core::MessageReceiver &, EnumClassHash>;

    /**
     * @brief onMessage Parses incoming messages and dispatches it accordingly.
     */
    void onMessage(client hdl, server::message_ptr msg);

    /**
     * @brief parseMessage Parses an incoming message into a Message.
     */
    std::pair<core::MessageType, std::unique_ptr<rapidjson::Document>> parseMessage(const server::message_ptr & msg);

    server m_server;          //< websocketpp server
    clients m_input_clients;  //< clients that send input messages
    clients m_output_clients; //< clients that wait for our output messages
    listeners m_listeners;    //< who should we notify in our game system
    systems::Input & m_input_system;
};

} // end core
} // end arrakis

#endif // SERVER_H
