//
// Created by Darek Rudiš on 20.03.2025.
//

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "public.sdk/source/vst/vsteditcontroller.h"

using namespace Steinberg::Vst;
using namespace Steinberg;

class PluginController : public EditController
{
public:
    PluginController() = default;
    ~PluginController() override = default;

    static FUnknown* createInstance(void*) { return (IEditController*)new PluginController(); }

    tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
};
#endif //CONTROLLER_HPP
