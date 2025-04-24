//
// Created by Darek Rudiš on 20.03.2025.
//

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "public.sdk/source/vst/vsteditcontroller.h"

using namespace Steinberg::Vst;
using namespace Steinberg;

enum ParamID {
    kParamGain = 100,
    kParamBypass = 101
};

class PluginController : public EditController
{
public:
    PluginController() = default;
    ~PluginController() override = default;

    static FUnknown* createInstance(void*) { return (IEditController*)new PluginController(); }

    tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
    tresult PLUGIN_API terminate() SMTG_OVERRIDE;
    tresult PLUGIN_API getState(IBStream *state) SMTG_OVERRIDE;
    tresult PLUGIN_API setState(IBStream*) SMTG_OVERRIDE;
    ParamValue bypassState = 0.0;
};
#endif //CONTROLLER_HPP
