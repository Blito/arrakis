#include "server.h"

#include <algorithm>

using namespace arrakis::core;

Server::Server(int port)
{
    auto message_handler = [this] (auto hdl, auto msg) { onMessage(hdl, msg); };

    m_server.set_message_handler(message_handler);

    m_server.init_asio();
    m_server.listen(port);
    m_server.start_accept();
}

void Server::run()
{
    m_server.run();
}

void Server::sendMessage(const Message & msg)
{
}

void Server::registerTo(MessageType msgType, MessageReceiver & receiver)
{
    m_listeners.insert({ msgType, receiver });
}

void Server::onMessage(client hdl, server::message_ptr msg)
{
    std::cout << "Server: " << msg->get_payload() << std::endl;

    for (auto & msgType_hdlr : m_listeners)
    {
        msgType_hdlr.second.notify(msg->get_payload());
    }
}

Server::Message Server::parseMessage(const server::message_ptr & msg)
{

}
