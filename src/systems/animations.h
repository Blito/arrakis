#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <entityx/System.h>

namespace arrakis
{

namespace systems
{

class Animations : public entityx::System<Animations>
{
public:
    void update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt) override;
};

} // end systems
} // end arrakis

#endif // ANIMATIONS_H
