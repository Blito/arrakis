#ifndef GAME_H
#define GAME_H

#include <thread>

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

    entityx::EntityManager m_entityManager;

    entityx::EventManager m_eventManager;

    entityx::SystemManager m_systemsManager;

    systems::Networking m_server;

    systems::Input m_input;

    std::thread m_serverThread;
};

} // end core
} // end arrakis

#endif // GAME_H
