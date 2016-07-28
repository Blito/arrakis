#ifndef NETWORKINGSYSTEM_H
#define NETWORKINGSYSTEM_H

#include <functional>
#include <unordered_map>
#include <memory>
#include <string>
#include <thread>

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


// The Networking system is responsible for direct communication with
// both input and output clients.
//
// Handles incoming messages and dispatches them through the MessageReceiver
// interface.
//
// The current implementation involves a JSON over WebSockets protocol.
class Networking
{
public:
    Networking(systems::Input & input_system, int port = 9002);
    ~Networking();

    void start_server();
    void stop_server();

    // send to output clients
    void send_message(const core::Message & msg);

    // register to input messages
    void register_to(core::MessageType msgType, core::MessageReceiver & receiver);

protected:
    using ws_server = websocketpp::server<websocketpp::config::asio>;

    // client handle used internally
    using client = websocketpp::connection_hdl;

    // map: client -> player
    using clients = std::map<client, core::PlayerID, std::owner_less<websocketpp::connection_hdl>>;

    // We need this to store enum classes as unordered_map keys.
    struct EnumClassHash
    {
        template <typename T>
        std::size_t operator()(T t) const
        {
            return static_cast<std::size_t>(t);
        }
    };
    using message_listeners = std::unordered_multimap<core::MessageType, core::MessageReceiver &, EnumClassHash>;

    // Parses incoming messages and dispatches it accordingly.
    void on_message(client hdl, ws_server::message_ptr msg);

    // Cleans up structures on client disconnection.
    void on_disconnect(client hdl);

    // Parses an incoming message into a Message.
    std::pair<core::MessageType, std::unique_ptr<rapidjson::Document>> parseMessage(const ws_server::message_ptr & msg);

    clients input_clients;
    clients output_clients;
    message_listeners listeners;  //< who should we notify in our game system
    systems::Input & input_system;

    ws_server server;
    std::thread server_thread;

    // Protocol constants
    const std::string CLIENT_TYPE_INPUT = "InputClient";
    const std::string CLIENT_TYPE_OUTPUT = "OutputClient";

};

} // end core
} // end arrakis

#endif // NETWORKINGSYSTEM_H
