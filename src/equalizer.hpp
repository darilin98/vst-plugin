//
// Created by Darek Rudiš on 25.04.2025.
//

#ifndef EQUALIZER_HPP
#define EQUALIZER_HPP

#include "fftw3.h"
#include "base/ftypes.h"
#include "vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/vstaudioeffect.h"
#include "controller.hpp"
#include "utils.hpp"
#include "eqconstants.hpp"
#include "eqshape.hpp"

using namespace Steinberg::Vst;
using namespace Steinberg;

/**
 * @brief Modulates frequency data.
 *
 * Transforms frequency data based on values specified by parameters.
 */
class Equalizer {
public:
    virtual ~Equalizer() = default;

    virtual void modulate(fftwf_complex* freq_bins, int fft_size, int sample_size) = 0;
    virtual void update_params(ProcessData& data) = 0;

};

/**
 * @brief Modulates frequency data based on a polynomial function.
 *
 * Transforms frequency data by a function defined by parameters.
 */
class PolynomialEqualizer : public Equalizer {
public:
    void modulate(fftwf_complex* freq_bins, int fft_size, int sample_rate) override;
    void update_params(ProcessData& data) override;
private:
    float param_shift_ = EQ::kDefaultShift;
    float param_intensity_ = EQ::kDefaultIntensity;
    float param_direction_ = EQ::kDefaultDirection;
    float param_width_ = EQ::kDefaultWidth;
    EqShapePreset param_shape_ = EqShape::kDefaultEqShape;
};

/**
 * @brief Extracts current parameter values from data provided by the host.
 *
 * @param data Data that the host provides
 * @param id ID of the parameter being handled
 * @param outValue Resulting parameter value
 * @return A boolean value based on whether the parameter value got updated.
 */
inline bool getParameterValue(ProcessData& data, CustomParamID id, ParamValue& outValue)
{
    if (!data.inputParameterChanges)
        return false;
    for (int32 i = 0; i < data.inputParameterChanges->getParameterCount(); ++i)
    {
        auto* queue = data.inputParameterChanges->getParameterData(i);
        if (queue && queue->getParameterId() == id)
        {
            int32 sampleOffset;
            if (queue->getPoint(queue->getPointCount() - 1, sampleOffset, outValue) == kResultTrue)
                return true;
        }
    }
    return false;
}
#endif //EQUALIZER_HPP
