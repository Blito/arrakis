#ifndef RENDERING_H
#define RENDERING_H

namespace arrakis
{

namespace components
{

struct Rendering
{
    Rendering(bool enabled) : enabled(enabled) {}

    bool enabled;
};

} // end components
} // end arrakis

#endif // RENDERING_H
