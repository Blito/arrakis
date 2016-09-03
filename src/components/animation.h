#ifndef ANIMATION_H
#define ANIMATION_H

namespace arrakis
{

namespace components
{

struct Animation
{
    Animation(bool & property, unsigned int curve_id, float starting_phase = 0.0f)
        : property(property),
          curve_id(curve_id),
          ms_ellapsed(0.0f) {}

    bool & property; // TODO: change this to a variadic?
    unsigned int curve_id; // this id should match an id in the Animations system.
    float ms_ellapsed;
};

} // end components
} // end arrakis

#endif // ANIMATION_H
