//
// Created by Darek Rudiš on 20.03.2025.
//
#include "controller.hpp"

tresult PLUGIN_API PluginController::initialize(FUnknown* context)
{
    tresult result = EditController::initialize(context);
    if (result != kResultOk)
        return result;
    parameters.removeAll();
    parameters.addParameter(STR16("Gain"), nullptr, 0, 0.5, ParameterInfo::kCanAutomate, kParamGain);
    // https://forums.steinberg.net/t/host-bypass-vst2-vs-vst3-explanation/201874?utm=true
    parameters.addParameter(STR16("Bypass"), nullptr, 1, 0.0, ParameterInfo::kIsBypass | ParameterInfo::kCanAutomate | ParameterInfo::kIsList, kParamBypass);

    return kResultOk;
}

tresult PLUGIN_API PluginController::terminate()
{
    return EditController::terminate();
}
