#ifndef SERVER_H
#define SERVER_H

#include <functional>
#include <unordered_map>

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

namespace arrakis
{

namespace core
{

class MessageReceiver;

/**
 * @brief The Server class represents a game server.
 *
 * The server handles basic networking setup, and delegates message
 * processing to other classes (like arrakis::systems::InputSystem).
 */
class Server
{
public:
    enum class MessageType { Input };
    struct Message
    {
        MessageType type;
        std::string payload;
    };

    /**
     * @brief Server Sets up the server listening at the given port.
     */
    Server(int port = 9002);

    /**
     * @brief run Starts the server.
     */
    void run();

    /**
     * @brief sendMessage Sends a message to all registered receivers that are
     * interested in msg.MessageType.
     */
    void sendMessage(const Message & msg);

    /**
     * @brief registerTo Registers a receiver to be notified when a message of
     * a specific type reaches the server.
     */
    void registerTo(MessageType msgType, MessageReceiver & receiver);

protected:
    using server = websocketpp::server<websocketpp::config::asio>;
    using client = websocketpp::connection_hdl;
    using clients = std::vector<client>;

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
    using listeners = std::unordered_multimap<MessageType, MessageReceiver &, EnumClassHash>;

    /**
     * @brief onMessage Parses incoming messages and dispatches it accordingly.
     */
    void onMessage(client hdl, server::message_ptr msg);

    /**
     * @brief parseMessage Parses an incoming message into a Message.
     */
    Message parseMessage(const server::message_ptr & msg);

    server m_server;          //< websocketpp server
    clients m_input_clients;  //< clients that send input messages
    clients m_output_clients; //< clients that wait for our output messages
    listeners m_listeners;    //< who should we notify in our game system
};

/**
 * @brief The MessageReceiver class is a simple Observer interface.
 * @sa arrakis::core::Server::registerTo(...)
 */
class MessageReceiver
{
public:
    virtual void notify(Server::Message msg) = 0;
};

} // end core
} // end arrakis

#endif // SERVER_H
