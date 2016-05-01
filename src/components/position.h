#ifndef POSITION_H
#define POSITION_H

namespace arrakis
{

namespace components
{

struct Position
{
    Position(float x, float y) : x(x), y(y) {}

    float x, y;
};

} // end components
} // end arrakis

#endif // POSITION_H
