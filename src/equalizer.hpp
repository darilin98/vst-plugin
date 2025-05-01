//
// Created by Darek Rudiš on 25.04.2025.
//
#include "fftw3.h"
#include "base/ftypes.h"
#include "vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/vstaudioeffect.h"
#include "controller.hpp"

using namespace Steinberg::Vst;
using namespace Steinberg;

class Equalizer {
public:
    virtual ~Equalizer() = default;

    virtual void modulate(fftw_complex* freq_bins, int fft_size, int sample_size) = 0;

};

class PolynomialEqualizer : public Equalizer {
public:
    void modulate(fftw_complex* freq_bins, int fft_size, int sample_rate) override;
};

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
            if (queue->getPoint(queue->getPointCount() - 1, sampleOffset, outValue))
                return true;
        }
    }
    return false;
}
#endif //EQUALIZER_HPP
