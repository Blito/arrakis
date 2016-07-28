#ifndef PLAYERCONTROLLED_H
#define PLAYERCONTROLLED_H

#include <core/message.h> // core::Player

namespace arrakis
{

namespace components
{

struct PlayerControlled
{
    PlayerControlled(core::PlayerID controlled_by) : controlled_by(controlled_by) {}

    core::PlayerID controlled_by;
};

} // end components
} // end arrakis

#endif // PLAYERCONTROLLED_H
