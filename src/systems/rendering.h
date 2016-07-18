#ifndef RENDERINGSYSTEM_H
#define RENDERINGSYSTEM_H

#include <entityx/System.h>

namespace arrakis
{

namespace systems
{

class Networking;

class Rendering : public entityx::System<Rendering>
{
public:
    Rendering(Networking & server);

    void update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt) override;

protected:
    Networking & m_server;
};

} // end systems
} // end arrakis

#endif // RENDERINGSYSTEM_H
