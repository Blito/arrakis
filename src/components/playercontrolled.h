#ifndef PLAYERCONTROLLED_H
#define PLAYERCONTROLLED_H

#include <core/message.h> // core::Player

namespace arrakis
{

namespace components
{

struct PlayerControlled
{
    enum class Status
    {
        IDLE,
        WALKING,
        DUCKING,
        AIRBORN
    };

    enum class Direction
    {
        NONE, N, NE, E, SE, S, SW, W, NW
    };

    PlayerControlled(core::PlayerID controlled_by)
        : controlled_by(controlled_by),
          status(Status::IDLE),
          aim_direction(Direction::NONE),
          ammo(5) {}

    core::PlayerID controlled_by;

    Status status;

    Direction aim_direction;

    unsigned int ammo;
};

} // end components
} // end arrakis

#endif // PLAYERCONTROLLED_H
