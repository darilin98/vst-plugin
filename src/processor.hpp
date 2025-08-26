//
// Created by Darek Rudiš on 20.03.2025.
//

#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "pluginterfaces/base/ustring.h"
#include "equalizer.hpp"
#include "fftprocessor.hpp"
#include <vector>

using namespace Steinberg::Vst;
using namespace Steinberg;

static const FUID PluginProcessorUID(0xd07f0f74, 0x71df4221, 0x98a7dc36, 0x845c4cf5);
static const FUID PluginControllerUID(0x16616cb1, 0xdace4b43, 0xb29ea085, 0xa674bb2d);

constexpr int FFT_SIZE = 1024;

class PluginProcessor : public AudioEffect
{
public:
    PluginProcessor() = default;
    ~PluginProcessor() override = default;

    static FUnknown* createInstance(void*) { return static_cast<IAudioProcessor *>(new PluginProcessor()); }

    tresult PLUGIN_API getControllerClassId(TUID classId) SMTG_OVERRIDE;
    tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
    tresult PLUGIN_API terminate() SMTG_OVERRIDE;
    tresult PLUGIN_API process(ProcessData& data) SMTG_OVERRIDE;
    tresult PLUGIN_API setupProcessing(ProcessSetup &setup) SMTG_OVERRIDE;
    tresult PLUGIN_API setState(IBStream* state) SMTG_OVERRIDE;
    tresult PLUGIN_API getState(IBStream* state) SMTG_OVERRIDE;
private:
    [[nodiscard]] bool getBypassState(const ProcessData& data) const;
    std::vector<std::unique_ptr<FFTProcessor>> fft_processors_;
    ParamValue bypassState = 0.0f;
    std::shared_ptr<Equalizer> equalizer_;
};
#endif //PROCESSOR_HPP
