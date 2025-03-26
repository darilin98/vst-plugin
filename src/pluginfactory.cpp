//
// Created by Darek Rudiš on 20.03.2025.
//
#include "public.sdk/source/main/pluginfactory.h"
#include "public.sdk/source/vst/utility/stringconvert.h"
#include "public.sdk/source/vst/vstcomponent.h"
#include "public.sdk/source/vst/vsteditcontroller.h"
#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivsteditcontroller.h"

#include "processor.hpp"
#include "controller.hpp"

using namespace Steinberg::Vst;

extern "C" bool bundleEntry() {
    return true;
}

extern "C" void bundleExit() {
}

BEGIN_FACTORY_DEF("MyCompany",
                  "https://mycompany.com",
                  "mailto:contact@mycompany.com")

    // Register the processor
    DEF_CLASS2(INLINE_UID_FROM_FUID(PluginProcessorUID),
               PClassInfo::kManyInstances, // multiple instances allowed
               kVstAudioEffectClass,       // Component category
               "The Best Plugin Ever",      // Plugin name
               Vst::kDistributable,        // Distributable attribute
               "Fx|Synth",                 // Subcategories
               "1.0.0",                     // Version
               "MyCompany",                // Vendor
               PluginProcessor::createInstance)

    // Register the controller
    DEF_CLASS2(INLINE_UID_FROM_FUID(PluginControllerUID),
               PClassInfo::kManyInstances,
               kVstComponentControllerClass,
               "My Plugin Controller",
               0,
               "",
               "1.0.0",
               "MyCompany",
               PluginController::createInstance)

END_FACTORY

