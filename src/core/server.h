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

    /**
     * @brief Server Sets up the server listening at the given port.
     */
    Server(int port = 9002);

    /**
     * @brief run Starts the server.
     */
    void run();

    void registerTo(MessageType msgType, MessageReceiver & receiver);

protected:
    using server = websocketpp::server<websocketpp::config::asio>;

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
    void onMessage(websocketpp::connection_hdl hdl, server::message_ptr msg);

    server m_server; //< websocketpp server
    listeners m_listeners; //< who should we notify
};

} // end core
} // end arrakis

#endif // SERVER_H
