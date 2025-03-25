//
// Created by Darek Rudiš on 20.03.2025.
//
#include "controller.hpp"

tresult PLUGIN_API PluginController::initialize(FUnknown* context)
{
    EditController::initialize(context);
    return kResultOk;
}

tresult PLUGIN_API PluginController::terminate()
{
    return EditController::terminate();
}
