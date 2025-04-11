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
    parameters.addParameter(STR16("Gain"), nullptr, 0, 0.5, ParameterInfo::kCanAutomate, 100);
    return kResultOk;
}

tresult PLUGIN_API PluginController::terminate()
{
    return EditController::terminate();
}
