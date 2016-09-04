#ifndef RENDERING_H
#define RENDERING_H

namespace arrakis
{

namespace components
{

struct Rendering
{
    enum class Tag
    {
        PLAYER,
        ARROW,
        ARROW_POWER_UP
    };

    Rendering(Tag tag, bool enabled = true) : tag(tag), enabled(enabled) {}

    bool enabled;
    Tag tag;
};

} // end components
} // end arrakis

#endif // RENDERING_H
