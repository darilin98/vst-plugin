//
// Created by Darek Rudiš on 20.03.2025.
//

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "vstgui4/vstgui/plugin-bindings/vst3editor.h"
#include "public.sdk/source/main/moduleinit.h"
#include "eqconstants.hpp"

using namespace Steinberg::Vst;
using namespace Steinberg;

enum CustomParamID : ParamID{
    kParamBypass = 101,
    kParamShift,
    kParamIntensity,
    kParamDirection,
    kParamWidth,
    kParamShape,
};

class PluginController : public EditController
{
public:
    PluginController() = default;
    ~PluginController() override = default;

    static FUnknown* createInstance(void*) { return (IEditController*)new PluginController(); }
    IPlugView* PLUGIN_API createView (FIDString name) SMTG_OVERRIDE;

    tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
    tresult PLUGIN_API terminate() SMTG_OVERRIDE;
    tresult PLUGIN_API getState(IBStream *state) SMTG_OVERRIDE;
    tresult PLUGIN_API setState(IBStream*) SMTG_OVERRIDE;
    tresult PLUGIN_API getParamStringByValue(ParamID id, ParamValue valueNormalized, String128 string) SMTG_OVERRIDE;
    tresult PLUGIN_API setParamNormalized(ParamID tag, ParamValue value) SMTG_OVERRIDE;

    ParamValue bypassState = 0.0;
};
#endif //CONTROLLER_HPP
