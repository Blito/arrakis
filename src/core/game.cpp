#include "game.h"

#include <iostream>

#include <components/position.h>
#include <systems/inputsystem.h>
#include <systems/playercontrollersystem.h>

using namespace arrakis::core;

Game::Game() :
    m_entityManager(m_eventManager),
    m_systemsManager(m_entityManager, m_eventManager),
    m_player(m_entityManager.create())
{
    m_systemsManager.add<systems::PlayerControllerSystem>(m_input);
    m_systemsManager.configure();

    m_player.assign<components::Position>(10, 100);

    try
    {
        m_server.registerTo(Server::MessageType::Input, m_input);
        m_serverThread = std::thread([this] { m_server.run(); });
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
        std::this_thread::sleep_for(100ms);
    }
}
