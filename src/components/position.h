#ifndef POSITION_H
#define POSITION_H

namespace arrakis
{

namespace components
{

struct Position
{
    Position(int x, int y) : x(x), y(y) {}

    int x, y;
};

} // end components
} // end arrakis

#endif // POSITION_H
