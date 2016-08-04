#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

namespace arrakis
{

namespace components
{

struct BoxCollider
{
    BoxCollider(float x_min, float x_max, float y_min, float y_max,
                bool enabled_vertical = true, bool enabled_horizontal = true,
                bool is_static = false)
        : x_min(x_min),
          x_max(x_max),
          y_min(y_min),
          y_max(y_max),
          enabled_vertical(enabled_vertical),
          enabled_horizontal(enabled_horizontal),
          is_static(is_static) {}

    bool is_static, enabled_vertical, enabled_horizontal;
    float x_min, x_max, y_min, y_max;
};

} // end components
} // end arrakis

#endif // BOXCOLLIDER_H
