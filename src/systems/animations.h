#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <entityx/System.h>

namespace arrakis
{

namespace systems
{

// WARNING/DISCLAIMER: The whole animations system is incomplete.
//  - Curve IDs are not being considered/used.
//  - Only animation supported is turning a boolean property ON after X ms.
class Animations : public entityx::System<Animations>
{
public:
    void update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt) override;
};

} // end systems
} // end arrakis

#endif // ANIMATIONS_H
