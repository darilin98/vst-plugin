//
// Created by Darek Rudiš on 04.08.2025.
//

#ifndef EQSHAPE_HPP
#define EQSHAPE_HPP

#include <cmath>

/**
 * @brief Lists presets for the shape of an EQ curve
 */
enum EqShapePreset  {
    SoftNotch,
    Wave,
    LeftRiser,
    Valley,
    // More shapes to be added
    Count ///< Indicates number of presets defined.
};

/**
 * @brief Defines the default selected EQ curve shape in @ref PresetMenu
 */
namespace EqShape {
    constexpr EqShapePreset kDefaultEqShape = SoftNotch;
}

/**
 * @brief Converts selected preset from @ref EqShapePreset to a value for the purpose of drawing an EQ curve.
 * @param preset Selected preset option to-be converted to a value
 * @param x Point on the x-axis of the plot of the curve
 * @return Immediate curve value in the point of x
 * @see EqShapePreset
 */
inline float shapeFromPreset(const EqShapePreset preset, const float x) {
    switch (preset)
    {
        case LeftRiser:
            return 1.5f - powf(x, 2.0f);
        case SoftNotch:
            return (expf(5.0f * x) - 1.0f) * (expf(5.0f * (1.0f - x)) - 1.0f)  / powf(expf(5.0f * 0.5f) - 1.0f, 2.0f);
        case Wave:
            return -powf(x, 4.0f) + powf(x, 3.0f) + powf(x, 2.0f) + 0.5f;
        case Valley:
            return -0.27f * powf(x, 4.0f) + 0.6f * powf(x, 3.0f) + 0.6f * powf(x, 2.0f) - x;
        default:
            return 0.0f;
    }
}
#endif //EQSHAPE_HPP
