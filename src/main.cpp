#include <entityx/entityx.h>

#include <iostream>
#include <exception>

#include <core/server.h>

int main()
{
    entityx::EntityX ex;
    entityx::Entity entity = ex.entities.create();
    entity.destroy();

    try
    {
        arrakis::core::Server server;
    }
    catch (std::exception ex)
    {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}
