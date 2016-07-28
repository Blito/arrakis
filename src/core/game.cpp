#include "game.h"

#include <iostream>
#include <algorithm>

#include <core/constants.h>
#include <core/message.h>
#include <components/physics.h>
#include <components/playercontrolled.h>
#include <components/position.h>
#include <components/rendering.h>
#include <systems/input.h>
#include <systems/physics.h>
#include <systems/playercontroller.h>
#include <systems/rendering.h>

using namespace arrakis::core;

Game::Game(int server_port) :
    entity_manager(event_manager),
    systems_manager(entity_manager, event_manager),
    networking_system(input_system, server_port)
{
    systems_manager.add<systems::PlayerController>(input_system);
    systems_manager.add<systems::Physics>();
    systems_manager.add<systems::Rendering>(networking_system);
    systems_manager.configure();

    try
    {
        networking_system.register_to(MessageType::Input, input_system);
        networking_system.register_to(MessageType::ClientDisconnected, input_system);
        networking_system.register_to(MessageType::NewClient, *this);
        networking_system.start_server();
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

        float time_elapsed = is_paused() ? 0.0 : 50.0f;

        if (is_paused()) std::cout << "Game paused." << std::endl;

        systems_manager.update<systems::PlayerController>(time_elapsed);
        systems_manager.update<systems::Physics>(time_elapsed);
        systems_manager.update<systems::Rendering>(time_elapsed);
        std::this_thread::sleep_for(50ms);
    }
}

bool Game::is_paused() const
{
    return input_system.is_anyone_doing(systems::Input::Action::PAUSE);
}

void Game::notify(Message msg, PlayerID player)
{
    auto new_entity = entity_manager.create();

    new_entity.assign<components::Position>(10, 5);
    new_entity.assign<components::Physics>(5.0f, true, utils::vec2f{1.1f, 1.5f}, utils::vec2f{400.0f, 700.0f});
    new_entity.assign<components::Rendering>(true);
    new_entity.assign<components::PlayerControlled>(player);
}
