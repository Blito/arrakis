#ifndef PLAYERCONTROLLED_H
#define PLAYERCONTROLLED_H

#include <core/message.h> // core::Player

namespace arrakis
{

namespace components
{

struct PlayerControlled
{
    PlayerControlled(core::Player controlled_by) : controlled_by(controlled_by) {}

    core::Player controlled_by;
};

} // end components
} // end arrakis

#endif // PLAYERCONTROLLED_H
