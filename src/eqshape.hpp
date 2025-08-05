//
// Created by Darek Rudiš on 04.08.2025.
//

#ifndef EQSHAPE_HPP
#define EQSHAPE_HPP

#include <cmath>

enum EqShapePreset  {
    Bell,
    LeftRiser,
    Wave,
    // More shapes to be added
    Count
};

namespace EqShape {
    constexpr EqShapePreset kDefaultEqShape = Bell;
}

inline float shapeFromPreset(const EqShapePreset preset, const float x) {
    switch (preset)
    {
        case Bell:
            return 1.0f - powf(x, 2.0f);
        case LeftRiser:
            // TODO Design shape
                break;
        case Wave:
            return -powf(x, 4.0f) + powf(x, 3.0f) + powf(x, 2.0f) + 0.5f;
        default:
            return 0.0f;
    }
}
#endif //EQSHAPE_HPP
