//
// Created by Darek Rudiš on 04.07.2025.
//

#ifndef EQCONSTANTS_HPP
#define EQCONSTANTS_HPP

/**
 * @namespace EQ
 * @brief Contains defined value constants
 *
 * Provides a common start point for use by the @ref PluginController and @ref PluginProcessor.
 *
 * @see PluginController
 * @see PluginController
 */
namespace EQ {
    constexpr float minFreq = 20.0f;
    constexpr float maxFreq = 20000.0f;

    constexpr float dBMaxBoost = 12.0f;

    constexpr float kDefaultShift = 0.5f;
    constexpr float kDefaultIntensity = 0.5f;
    constexpr float kDefaultDirection = 0.0f;
    constexpr float kDefaultWidth = 0.5f;

}

#endif //EQCONSTANTS_HPP
