#ifndef GAME_H
#define GAME_H

#include <entityx/Entity.h>
#include <entityx/System.h>

#include <core/message.h>
#include <systems/input.h>
#include <systems/networking.h>

namespace arrakis
{

namespace core
{

class Game : public MessageReceiver
{
public:
    Game(int server_port);

    void run();

protected:
    void notify(Message msg, Player player) override;

    entityx::EntityManager entity_manager;

    entityx::EventManager event_manager;

    entityx::SystemManager systems_manager;

    systems::Networking networking_system;

    systems::Input input_system;
};

} // end core
} // end arrakis

#endif // GAME_H
