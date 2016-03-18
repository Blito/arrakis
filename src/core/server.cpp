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
    if (msg.type != MessageType::Output)
        return;

    for (auto client : m_output_clients)
    {
        if (!client.expired())
        {
            m_server.send(client, msg.payload, websocketpp::frame::opcode::TEXT);
        }
    }
}

void Server::registerTo(MessageType msgType, MessageReceiver & receiver)
{
    m_listeners.insert({ msgType, receiver });
}

void Server::onMessage(client hdl, server::message_ptr msg)
{
    auto parsed_msg = parseMessage(msg);

    switch (parsed_msg.first)
    {
    // If a new message asks for a new client, add it to client lists.
    case MessageType::NewClient:
    {
        auto client_type = std::string((*parsed_msg.second)["new-client"].GetString());
        //TODO: CHECK THAT IT IS A NEW CLIENT
        if (client_type == "InputClient")
        {
            std::cout << "New input client." << std::endl;
            m_input_clients.push_back(hdl);
        }
        else if (client_type == "OutputClient")
        {
            std::cout << "New output client." << std::endl;
            m_output_clients.push_back(hdl);
        }
    }
        break;

    // If incoming message could not be classified.
    case MessageType::ParseError:
        break;

    // If another type of message comes, forward it to interested objects inside our server.
    case MessageType::Input:
    {
        //TODO: CHECK THAT IT IS AN EXISTING CLIENT
        auto range = m_listeners.equal_range(parsed_msg.first);
        Message message {parsed_msg.first, msg->get_payload()};
        std::for_each(range.first, range.second, [message](auto listener)
        {
            listener.second.notify(message);
        });
    }
        break;
    }

}

/**
 * Current supported messages:
 *
 * NewClient type request messages:
 * {new-client:[InputClient|OutputClient]}
 *
 * Input type messages:
 * {action[-stopped]:[UP|DOWN|LEFT|RIGHT|JUMP|A|B]}
 */
std::pair<Server::MessageType, std::unique_ptr<rapidjson::Document>> Server::parseMessage(const server::message_ptr & msg)
{
    auto document = std::make_unique<rapidjson::Document>();

    if (document->Parse(msg->get_payload().c_str()).HasParseError())
        return { MessageType::ParseError, std::move(document) };

    if (document->HasMember("action") || document->HasMember("action-stopped"))
        return { MessageType::Input, std::move(document) };

    if (document->HasMember("new-client"))
        return { MessageType::NewClient, std::move(document) };

    return { MessageType::ParseError, std::move(document) };
}
