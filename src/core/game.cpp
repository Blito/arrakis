#include "game.h"

#include <iostream>

#include <core/constants.h>
#include <core/message.h>
#include <components/physics.h>
#include <components/playercontrolled.h>
#include <components/position.h>
#include <components/rendering.h>
#include <systems/inputsystem.h>
#include <systems/physicssystem.h>
#include <systems/playercontrollersystem.h>
#include <systems/renderingsystem.h>

using namespace arrakis::core;

Game::Game(int server_port) :
    m_entityManager(m_eventManager),
    m_systemsManager(m_entityManager, m_eventManager),
    m_server(m_input, server_port)
{
    m_systemsManager.add<systems::PlayerControllerSystem>(m_input);
    m_systemsManager.add<systems::PhysicsSystem>();
    m_systemsManager.add<systems::RenderingSystem>(m_server);
    m_systemsManager.configure();

    try
    {
        m_server.registerTo(MessageType::Input, m_input);
        m_server.registerTo(MessageType::NewClient, *this);
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

    while (true)
    {
        using namespace std::literals::chrono_literals;

        m_systemsManager.update<systems::PlayerControllerSystem>(50.0f);
        m_systemsManager.update<systems::PhysicsSystem>(50.0f);
        m_systemsManager.update<systems::RenderingSystem>(50.0f);
        std::this_thread::sleep_for(50ms);
    }
}

void Game::notify(Message msg, Player player)
{
    auto new_entity = m_entityManager.create();

    new_entity.assign<components::Position>(10, 5);
    new_entity.assign<components::Physics>(5.0f, true, utils::vec2f{1.1f, 1.5f}, utils::vec2f{400.0f, 700.0f});
    new_entity.assign<components::Rendering>(true);
    new_entity.assign<components::PlayerControlled>(player);
}
