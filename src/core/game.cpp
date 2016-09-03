#include "game.h"

#include <iostream>
#include <algorithm>
#include <chrono>

#include <core/constants.h>
#include <core/gameobjects.h>
#include <core/message.h>
#include <components/boxcollider.h>
#include <components/physics.h>
#include <components/playercontrolled.h>
#include <components/position.h>
#include <components/rendering.h>
#include <systems/animations.h>
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
    systems_manager.add<systems::Animations>();
    systems_manager.configure();

    init_scene();

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
        using namespace std::chrono;
        using namespace std::literals::chrono_literals;

        auto start_time = std::chrono::high_resolution_clock::now();

        float tick_length_ms = is_paused() ? 0.0f : tick_length.count();

        systems_manager.update<systems::PlayerController>(tick_length_ms);
        systems_manager.update<systems::Physics>(tick_length_ms);
        systems_manager.update<systems::Rendering>(tick_length_ms);
        systems_manager.update<systems::Animations>(tick_length_ms);

        auto time_elapsed = high_resolution_clock::now() - start_time;

        if (time_elapsed > tick_length)
        {
            std::cout << "Tick length exceeded!" << std::endl;
        }
        else
        {
            std::this_thread::sleep_for(tick_length - time_elapsed);
        }
    }
}

void Game::init_scene()
{
    auto floor = entity_manager.create();
    floor.assign<components::Position>(250, 15);
    floor.assign<components::BoxCollider>(components::BoxCollider::Tag::STATIC, -250, 250, -15, 15, true, true, true);

    auto left_wall = entity_manager.create();
    left_wall.assign<components::Position>(10, 60);
    left_wall.assign<components::BoxCollider>(components::BoxCollider::Tag::STATIC, -10, 10, -30, 30, true, true, true);
}

bool Game::is_paused() const
{
    return input_system.is_anyone_doing(systems::Input::Action::PAUSE);
}

void Game::notify(Message msg, PlayerID player)
{
    game_objects::player::create(entity_manager, player, 50.0, 50.0f);
}
