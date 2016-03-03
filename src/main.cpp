#include <entityx/entityx.h>

#include <iostream>
#include <exception>

#include <core/server.h>
#include <systems/inputsystem.h>

int main()
{
    entityx::EntityX ex;
    entityx::Entity entity = ex.entities.create();
    entity.destroy();

    try
    {
        using namespace arrakis::core;
        using namespace arrakis::systems;

        Server server;
        InputSystem input;

        server.registerTo(Server::MessageType::Input, input);
        server.run();
    }
    catch (std::exception ex)
    {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}
