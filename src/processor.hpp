//
// Created by Darek Rudiš on 20.03.2025.
//

#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "pluginterfaces/base/ustring.h"

using namespace Steinberg::Vst;
using namespace Steinberg;

class PluginProcessor : public AudioEffect
{
public:
    PluginProcessor() = default;
    ~PluginProcessor() override = default;

    static FUnknown* createInstance(void*) { return (IAudioProcessor*)new PluginProcessor(); }

    tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
    tresult PLUGIN_API terminate() SMTG_OVERRIDE;
    tresult PLUGIN_API process(ProcessData& data) SMTG_OVERRIDE;
    tresult PLUGIN_API setState(IBStream* state) SMTG_OVERRIDE;
    tresult PLUGIN_API getState(IBStream* state) SMTG_OVERRIDE;
};
#endif //PROCESSOR_HPP
