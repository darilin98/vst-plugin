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

/**
 * @brief Defines parameters used by the plugin
 */
enum CustomParamID : ParamID{
    kParamBypass = 101,
    kParamShift,
    kParamIntensity,
    kParamDirection,
    kParamWidth,
    kParamShape,
};

/**
 * @brief Custom implementation of the VSTSDK @ref EditController
 */
class PluginController : public EditController
{
public:
    PluginController() = default;
    ~PluginController() override = default;

    static FUnknown* createInstance(void*) { return static_cast<IEditController *>(new PluginController()); }
    /**
     * @brief Initializes a custom root view as part of VSTGUI defined in: @file resources/viewGUI.uidesc
     * @return Newly created view
     */
    IPlugView* PLUGIN_API createView (FIDString name) SMTG_OVERRIDE;
    /**
     * @brief Creates a controller instance and defines all of its parameters
     * @return State based on the validity of controller creation
     */
    tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
    tresult PLUGIN_API terminate() SMTG_OVERRIDE;
    /**
     * @brief Tries to update internal bypass state based on the values of @kParamBypass
     */
    tresult PLUGIN_API getState(IBStream *state) SMTG_OVERRIDE;
    /**
     * @brief Tries to update @kParamBypass by the state of the stream
     */
    tresult PLUGIN_API setState(IBStream*) SMTG_OVERRIDE;
    /**
     * @brief Handles translation between parameter float values and their name representations inside selector lists.
     * @param id Type of parameter being handled
     * @param valueNormalized Current value of the parameter
     * @param string String representing the translated name
     * @return
     */
    tresult PLUGIN_API getParamStringByValue(ParamID id, ParamValue valueNormalized, String128 string) SMTG_OVERRIDE;
    tresult PLUGIN_API setParamNormalized(ParamID tag, ParamValue value) SMTG_OVERRIDE;

    ParamValue bypassState = 0.0;
};
#endif //CONTROLLER_HPP
