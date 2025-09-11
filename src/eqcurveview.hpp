//
// Created by Darek Rudiš on 02.07.2025.
//

#ifndef EQCURVEVIEW_HPP
#define EQCURVEVIEW_HPP

/**
 * @file eqcurveview.hpp
 * @brief Declares the @ref EQCurveView class and its helpers.
 */

#include <memory>

#include "eqconstants.hpp"
#include "eqshape.hpp"
#include "vstgui/lib/cview.h"
#include "vstgui/uidescription/iuidescription.h"
#include "vstgui/uidescription/iviewcreator.h"
#include "vstgui/uidescription/uiattributes.h"
#include "vstgui/uidescription/uiviewfactory.h"
#include "vstgui/uidescription/detail/uiviewcreatorattributes.h"
#include "vstgui/lib/cdrawcontext.h"
#include "vstgui/lib/idependency.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "vstgui/uidescription/editing/uiviewcreatecontroller.h"

using controller_t = Steinberg::Vst::EditController*;

class CustomParamListener;
using listener_t = std::unique_ptr<CustomParamListener>;

/**
 * @brief Visualizes the current state of the EQ curve.
 *
 * Manages the live visualization of the EQ curve and reflects the changes made by altering parameters.
 */
class EQCurveView : public VSTGUI::CView, public Steinberg::FObject {
public:
    explicit EQCurveView(const VSTGUI::CRect& size)
        : CView(size) { }

    /**
     * @brief Core rendering method of the view
     *
     * Draws an up-to-date graph of the EQ curve.
     * @param dc A drawing context abstracting drawing regardless of OS
     */
    void draw(VSTGUI::CDrawContext* dc) override;
    void setParamListeners(controller_t controller);
    /**
     * @brief Relays user input to internal states.
     *
     *
     * @param id ID of the parameter being handled.
     * @param normalizedValue Value to-be updated internally for rendering purposes.
     */
    void onParamChanged(Steinberg::Vst::ParamID id, float normalizedValue);

private:
    /**
     * @brief Helper that binds a frequency to the graph's X axis.
     * @param freq EQ
     * @return X value on the graph.
     */
    [[nodiscard]] float freqToX(float freq) const;
    [[nodiscard]] float dbToY(float db) const;
    void drawFreqMarkers(VSTGUI::CDrawContext* dc) const;
    void drawDbMarkers(VSTGUI::CDrawContext* dc) const;

    controller_t _controller = nullptr;

    listener_t _shiftListener;
    listener_t _widthListener;
    listener_t _intensityListener;
    listener_t _directionListener;
    listener_t _shapeListener;

    float _shift = EQ::kDefaultShift;
    float _width = EQ::kDefaultWidth;
    float _intensity = EQ::kDefaultIntensity;
    float _direction = EQ::kDefaultDirection;
    EqShapePreset _shape = EqShape::kDefaultEqShape;

    const float _padding = 20.0f;

};

namespace VSTGUI {
    /**
     * @brief Factory method for @ref EQCurveView
     */
    struct EQCurveViewCreator : public ViewCreatorAdapter {

        EQCurveViewCreator() { UIViewFactory::registerViewCreator(*this); }

        [[nodiscard]] IdStringPtr getViewName() const override { return "EQCurveView"; }

        [[nodiscard]] IdStringPtr getBaseViewName() const override { return UIViewCreator::kCView; }

        CView* create(const UIAttributes& attr, const IUIDescription* d) const override {
            fprintf(stderr, "creating custom view");
            auto* view = new EQCurveView({0, 0, 100, 100});

            if (const auto* controller = dynamic_cast<VST3Editor*>(d->getController()))
            {
                if (auto* editController = controller->getController())
                {
                    view->setParamListeners(editController);
                }
            }
            return view;
        }
    };
    static EQCurveViewCreator __gEQCurveViewCreator;
}

using param_t = Steinberg::Vst::Parameter*;
using view_t = EQCurveView*;

/**
 * @brief Detects changes made to a parameter by the user.
 *
 * One listener is bound to one parameter at a time. Multiple instances need to be used to cover each parameter of the controller.
 */
class CustomParamListener : public Steinberg::FObject {
public:
    CustomParamListener(param_t param, view_t view, controller_t controller);

    ~CustomParamListener() override;

    void PLUGIN_API update(Steinberg::FUnknown* changedUnknown, Steinberg::int32 message) override;

private:
    param_t _param;
    view_t _view;
    controller_t _controller;

};
#endif //EQCURVEVIEW_HPP
