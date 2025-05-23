//
// Created by Darek Rudiš on 20.03.2025.
//

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "vstgui4/vstgui/plugin-bindings/vst3editor.h"
#include "public.sdk/source/main/moduleinit.h"

using namespace Steinberg::Vst;
using namespace Steinberg;

enum CustomParamID {
    kParamBypass = 101,
    kParamShift,
    kParamIntensity,
    kParamDirection,
    kParamWidth,
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
    ParamValue bypassState = 0.0;
};
#endif //CONTROLLER_HPP
