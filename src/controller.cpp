//
// Created by Darek Rudiš on 20.03.2025.
//
#include "controller.hpp"

#include "base/source/fstreamer.h"
#include "vstgui/lib/vstguiinit.h"

tresult PLUGIN_API PluginController::initialize(FUnknown* context)
{
    //VSTGUI::init(getPlatformModuleHandle());
    auto moduleHandle = getPlatformModuleHandle();
    fprintf(stderr, "Module handle: %p\n", moduleHandle);
    //VSTGUI::init(moduleHandle);
    tresult result = EditController::initialize(context);
    if (result != kResultOk)
        return result;
    parameters.removeAll();
    parameters.addParameter(STR16("Bypass"), nullptr, 1, 0.0, ParameterInfo::kIsBypass | ParameterInfo::kCanAutomate | ParameterInfo::kIsList, kParamBypass);
    parameters.addParameter(STR16("Shift"), nullptr, 0, EQ::kDefaultShift, ParameterInfo::kCanAutomate, kParamShift);
    parameters.addParameter(STR16("Intensity"), nullptr, 0, EQ::kDefaultIntensity, ParameterInfo::kCanAutomate, kParamIntensity);
    parameters.addParameter(STR16("Direction"), nullptr, 1, EQ::kDefaultDirection, ParameterInfo::kCanAutomate, kParamDirection);
    parameters.addParameter(STR16("Width"), nullptr, 0, EQ::kDefaultWidth, ParameterInfo::kCanAutomate, kParamWidth);

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

IPlugView* PLUGIN_API PluginController::createView (FIDString name)
{
    if (strcmp (name, ViewType::kEditor) == 0)
    {
        return new VSTGUI::VST3Editor (this, "view", "viewGUI.uidesc");
    }
    return nullptr;
}