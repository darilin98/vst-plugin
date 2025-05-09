//
// Created by Darek Rudiš on 20.03.2025.
//
#include "controller.hpp"

#include "base/source/fstreamer.h"

tresult PLUGIN_API PluginController::initialize(FUnknown* context)
{
    tresult result = EditController::initialize(context);
    if (result != kResultOk)
        return result;
    parameters.removeAll();
    // https://forums.steinberg.net/t/host-bypass-vst2-vs-vst3-explanation/201874?utm=true
    parameters.addParameter(STR16("Bypass"), nullptr, 1, 0.0, ParameterInfo::kIsBypass | ParameterInfo::kCanAutomate | ParameterInfo::kIsList, kParamBypass);
    parameters.addParameter(STR16("Shift"), nullptr, 0, 0.5, ParameterInfo::kCanAutomate, kParamShift);
    parameters.addParameter(STR16("Intensity"), nullptr, 0, 0.5, ParameterInfo::kCanAutomate, kParamIntensity);
    parameters.addParameter(STR16("Direction"), nullptr, 1, 0.0, ParameterInfo::kCanAutomate, kParamDirection);
    parameters.addParameter(STR16("Width"), nullptr, 0, 0.5, ParameterInfo::kCanAutomate, kParamWidth);



    return kResultOk;
}

tresult PLUGIN_API PluginController::terminate()
{
    return EditController::terminate();
}

tresult PLUGIN_API PluginController::setState(IBStream *state)
{
    if (!state)
        return kResultFalse;

    IBStreamer streamer(state, kLittleEndian);

    if (!streamer.readDouble(bypassState))
        return kResultFalse;

    // Update the parameter in the controller
    setParamNormalized(kParamBypass, bypassState);

    return kResultOk;
}

tresult PLUGIN_API PluginController::getState(IBStream *state)
{
    if (!state)
        return kResultFalse;

    IBStreamer streamer(state, kLittleEndian);

    bypassState = getParamNormalized(kParamBypass);

    if (!streamer.writeDouble(bypassState))
        return kResultFalse;

    return kResultOk;
}
