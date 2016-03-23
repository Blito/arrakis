#include "game.h"

#include <iostream>

#include <core/message.h>
#include <components/position.h>
#include <components/rendering.h>
#include <systems/inputsystem.h>
#include <systems/playercontrollersystem.h>
#include <systems/renderingsystem.h>

using namespace arrakis::core;

Game::Game(int server_port) :
    m_entityManager(m_eventManager),
    m_systemsManager(m_entityManager, m_eventManager),
    m_player(m_entityManager.create()),
    m_server(m_input, server_port)
{
    m_systemsManager.add<systems::PlayerControllerSystem>(m_input);
    m_systemsManager.add<systems::RenderingSystem>(m_server);
    m_systemsManager.configure();

    m_player.assign<components::Position>(10, 100);
    m_player.assign<components::Rendering>(true);

    try
    {
        m_server.registerTo(MessageType::Input, m_input);
        m_serverThread = std::thread([this] { m_server.run(); });
        std::cout << "Server started in port " << server_port << std::endl;
    }
    catch (std::exception ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

void Game::run()
{
    std::cout << "Game running..." << std::endl;

    unsigned int i = 0;
    while (true)
    {
        using namespace std::literals::chrono_literals;

        m_systemsManager.update<systems::PlayerControllerSystem>(10.0f);
        m_systemsManager.update<systems::RenderingSystem>(10.0f);
        std::this_thread::sleep_for(100ms);
    }
}
