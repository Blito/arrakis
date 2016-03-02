#include "server.h"

using namespace arrakis::core;

Server::Server(int port)
{
    m_server.set_message_handler(&onMessage);

    m_server.init_asio();
    m_server.listen(port);
    m_server.start_accept();
}

void Server::run()
{
    m_server.run();
}

void Server::registerTo(MessageType msgType, MessageHandler msgHdlr)
{
    m_listeners[msgType].push_back(msgHdlr);
}

void Server::onMessage(websocketpp::connection_hdl hdl, server::message_ptr msg)
{
    std::cout << msg->get_payload() << std::endl;
}
