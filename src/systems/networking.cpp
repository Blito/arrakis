#include "networking.h"

#include <algorithm>

using namespace arrakis::systems;

Networking::Networking(systems::Input & input_system, int port) :
    input_system(input_system)
{
    auto message_handler = [this] (auto hdl, auto msg) { on_message(hdl, msg); };
    auto close_handler = [this](auto hdl){ on_disconnect(hdl); };

    server.set_message_handler(message_handler);
    server.set_close_handler(close_handler);

    server.set_error_channels(websocketpp::log::elevel::all);
    server.set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload);

    server.init_asio();
    server.listen(port);
    server.start_accept();
}

Networking::~Networking()
{
    stop_server();
}

void Networking::start_server()
{
    server_thread = std::thread([this] { server.run(); });
}

void Networking::stop_server()
{

}

void Networking::send_message(const core::Message & msg)
{
    assert(msg.type == core::MessageType::Output);

    for (auto client : output_clients)
    {
        if (!client.first.expired())
        {
            server.send(client.first, msg.payload, websocketpp::frame::opcode::TEXT);
        }
    }
}

void Networking::register_to(core::MessageType msgType, core::MessageReceiver & receiver)
{
    listeners.insert({ msgType, receiver });
}

void Networking::on_message(client hdl, ws_server::message_ptr msg)
{
    auto parsed_msg = parseMessage(msg);

    switch (parsed_msg.first)
    {
    // If a message asks for a new client, add it to client lists.
    case core::MessageType::NewClient:
    {
        auto client_type = std::string((*parsed_msg.second)["new-client"].GetString());

        if (client_type == CLIENT_TYPE_INPUT)
        {            
            // Check that client is actually a new client.
            if (input_clients.find(hdl) != input_clients.end())
            {
                return;
            }

            if (input_system.is_room_for_new_player())
            {
                auto && player_id = input_system.create_new_player();
                input_clients.insert({hdl, player_id});

                auto input_listeners = listeners.equal_range(core::MessageType::NewClient);
                core::Message message {parsed_msg.first, msg->get_payload()};
                std::for_each(input_listeners.first, input_listeners.second, [message, player_id](auto listener)
                {
                    listener.second.notify(message, player_id);
                });

                std::cout << "New input client." << std::endl;
            }
            else
            {
                std::cout << "New input client requested but could not be created. Server full." << std::endl;
            }
        }
        else if (client_type == CLIENT_TYPE_OUTPUT)
        {
            // Check that client does not exist
            if (output_clients.find(hdl) != output_clients.end())
            {
                return;
            }

            std::cout << "New output client." << std::endl;

            // TODO: outpus clients may or may not be tied to a player!
            output_clients.insert({hdl, core::max_players_count});
        }
        else
        {
            std::cout << "Not a valid new client request." << std::endl;
        }
    }
        break;

    // If incoming message could not be classified.
    case core::MessageType::ParseError:
        break;

    // If another type of message comes, forward it to interested objects inside our server.
    case core::MessageType::Input:
    {
        // Check that client exists
        auto client = input_clients.find(hdl);
        if (client == input_clients.end())
        {
            return;
        }

        auto input_listeners = listeners.equal_range(core::MessageType::Input);
        core::Message message {parsed_msg.first, msg->get_payload()};
        std::for_each(input_listeners.first, input_listeners.second, [message, &client](auto listener)
        {
            listener.second.notify(message, (*client).second);
        });
    }
        break;
    }

}

void Networking::on_disconnect(client hdl)
{
    core::PlayerID disconnected_player = core::max_players_count;

    // Check if hdl is an input_client
    auto input_client = input_clients.find(hdl);
    if (input_client != input_clients.end())
    {
        disconnected_player = (*input_client).second;
        input_clients.erase(input_client);

        // Notify interested parties
        auto input_listeners = listeners.equal_range(core::MessageType::ClientDisconnected);
        core::Message message {core::MessageType::ClientDisconnected, std::string()};
        std::for_each(input_listeners.first, input_listeners.second, [&message, &disconnected_player](auto listener)
        {
            listener.second.notify(message, disconnected_player);
        });

        std::cout << "Player " << disconnected_player << " disconnected." << std::endl;
    }

    // Check if hdl is an output client
    auto output_client = output_clients.find(hdl);
    if (output_client != output_clients.end())
    {
        // TODO: At the moment, output clients are not tied to players.
        //disconnected_player = *output_client.second;
        output_clients.erase(output_client);
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
std::pair<arrakis::core::MessageType, std::unique_ptr<rapidjson::Document>> Networking::parseMessage(const ws_server::message_ptr & msg)
{
    auto document = std::make_unique<rapidjson::Document>();

    if (document->Parse(msg->get_payload().c_str()).HasParseError())
        return { core::MessageType::ParseError, std::move(document) };

    if (document->HasMember("action") || document->HasMember("action-stopped"))
        return { core::MessageType::Input, std::move(document) };

    if (document->HasMember("new-client"))
        return { core::MessageType::NewClient, std::move(document) };

    return { core::MessageType::ParseError, std::move(document) };
}
