#include "animations.h"

#include <components/animation.h>

using namespace arrakis::systems;

void Animations::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{
    using namespace arrakis::components;
    entities.each<Animation>([dt, this](entityx::Entity entity, Animation & animation)
    {
        animation.ms_ellapsed += dt;

        if (animation.ms_ellapsed > 1000)
        {
            animation.property = true;
            entity.remove<Animation>();
        }
    });
}
