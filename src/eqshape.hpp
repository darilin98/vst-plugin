//
// Created by Darek Rudiš on 04.08.2025.
//

#ifndef EQSHAPE_HPP
#define EQSHAPE_HPP

#include <cmath>

enum EqShapePreset  {
    Bell,
    Wave,
    LeftRiser,
    TwoHills,
    // More shapes to be added
    Count
};

namespace EqShape {
    constexpr EqShapePreset kDefaultEqShape = Bell;
}

inline float shapeFromPreset(const EqShapePreset preset, const float x) {
    switch (preset)
    {
        case LeftRiser:
            return 1.5f - powf(x, 2.0f);
        case Bell:
            return (expf(5.0f * x) - 1.0f) * (expf(5.0f * (1.0f - x)) - 1.0f)  / powf(expf(5.0f * 0.5f) - 1.0f, 2.0f);
        case Wave:
            return -powf(x, 4.0f) + powf(x, 3.0f) + powf(x, 2.0f) + 0.5f;
        case TwoHills:
            return -0.27f * powf(x, 4.0f) + 0.6f * powf(x, 3.0f) + 0.9f * powf(x, 2.0f) - x;
        default:
            return 0.0f;
    }
}
#endif //EQSHAPE_HPP
