#ifndef RENDERINGSYSTEM_H
#define RENDERINGSYSTEM_H

#include <entityx/System.h>

namespace arrakis
{

namespace core // forward declarations
{
    class Server;
}

namespace systems
{

class RenderingSystem : public entityx::System<RenderingSystem>
{
public:
    RenderingSystem(core::Server & server);

    void update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt) override;

protected:
    core::Server & m_server;
};

} // end systems
} // end arrakis

#endif // RENDERINGSYSTEM_H
