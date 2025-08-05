//
// Created by Darek Rudiš on 20.03.2025.
//
#include "controller.hpp"
#include "eqshape.hpp"
#include "base/ustring.h"

#include "base/source/fstreamer.h"
#include "vstgui/lib/vstguiinit.h"

tresult PLUGIN_API PluginController::initialize(FUnknown* context)
{
    auto moduleHandle = getPlatformModuleHandle();
    fprintf(stderr, "Module handle: %p\n", moduleHandle);
    tresult result = EditController::initialize(context);
    if (result != kResultOk)
        return result;
    parameters.removeAll();
    parameters.addParameter(STR16("Bypass"), nullptr, 1, 0.0, ParameterInfo::kIsBypass | ParameterInfo::kCanAutomate | ParameterInfo::kIsList, kParamBypass);
    parameters.addParameter(STR16("Shift"), nullptr, 0, EQ::kDefaultShift, ParameterInfo::kCanAutomate, kParamShift);
    parameters.addParameter(STR16("Intensity"), nullptr, 0, EQ::kDefaultIntensity, ParameterInfo::kCanAutomate, kParamIntensity);
    parameters.addParameter(STR16("Direction"), nullptr, 1, EQ::kDefaultDirection, ParameterInfo::kCanAutomate, kParamDirection);
    parameters.addParameter(STR16("Width"), nullptr, 0, EQ::kDefaultWidth, ParameterInfo::kCanAutomate, kParamWidth);
    parameters.addParameter(STR16("Shape"), nullptr, EqShapePreset::Count - 1, EqShape::kDefaultEqShape, ParameterInfo::kCanAutomate | ParameterInfo::kIsList, kParamShape);

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

tresult PLUGIN_API PluginController::getParamStringByValue(ParamID id, ParamValue valueNormalized, String128 string)
{
    if (id == kParamShape)
    {
        int index = static_cast<int>(valueNormalized * (static_cast<int>(EqShapePreset::Count) - 1) + 0.5);
        const char* name;
        switch (static_cast<EqShapePreset>(index)) {
            case EqShapePreset::Bell: name = "Bell"; break;
            case EqShapePreset::Wave: name = "Wave"; break;
            case EqShapePreset::LeftRiser: name = "LeftRiser"; break;

            default: name = "Unknown"; break;
        }
        Steinberg::UString(string, 128).fromAscii(name);
        return kResultTrue;
    }
    return EditController::getParamStringByValue(id, valueNormalized, string);
}

tresult PLUGIN_API PluginController::setParamNormalized(ParamID tag, ParamValue value)
{
    if (tag == kParamShape)
    {
        fprintf(stderr, ">>> setParamNormalized(kParamShape, %f)\n", value);
    }
    return EditController::setParamNormalized(tag, value);
}
