#ifndef RENDERINGSYSTEM_H
#define RENDERINGSYSTEM_H

#include <entityx/System.h>

namespace arrakis
{

namespace systems
{

class Networking;

// The Rendering system calculates what should the output clients see and
// sends that to them through the Networking system.
class Rendering : public entityx::System<Rendering>
{
public:
    Rendering(Networking & networking_system);

    void update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt) override;

protected:
    Networking & networking_system;
};

} // end systems
} // end arrakis

#endif // RENDERINGSYSTEM_H
