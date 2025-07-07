//
// Created by Darek Rudiš on 02.07.2025.
//

#ifndef EQCURVEVIEW_HPP
#define EQCURVEVIEW_HPP

#include <memory>

#include "eqconstants.hpp"
#include "vstgui/lib/cview.h"
#include "vstgui/uidescription/iuidescription.h"
#include "vstgui/uidescription/iviewcreator.h"
#include "vstgui/uidescription/uiattributes.h"
#include "vstgui/uidescription/uiviewfactory.h"
#include "vstgui/uidescription/detail/uiviewcreatorattributes.h"
#include "vstgui/lib/cdrawcontext.h"
#include "vstgui/lib/idependency.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "vstgui/lib/cgraphicspath.h"
#include "vstgui/uidescription/editing/uiviewcreatecontroller.h"

using controller_t = Steinberg::Vst::EditController*;

class CustomParamListener;
using listener_t = std::unique_ptr<CustomParamListener>;

class EQCurveView : public VSTGUI::CView, public Steinberg::FObject {
public:
    explicit EQCurveView(const VSTGUI::CRect& size)
        : CView(size) { }

    void draw(VSTGUI::CDrawContext* dc) override;
    void setParamListeners(controller_t controller);
    void onParamChanged(Steinberg::Vst::ParamID id, float normalizedValue);

private:
    [[nodiscard]] float freqToX(float freq) const;
    [[nodiscard]] float dbToY(float db) const;
    controller_t _controller = nullptr;

    listener_t _shiftListener;
    listener_t _widthListener;
    listener_t _intensityListener;
    listener_t _directionListener;

    float _shift = EQ::kDefaultShift;
    float _width = EQ::kDefaultWidth;
    float _intensity = EQ::kDefaultIntensity;
    float _direction = EQ::kDefaultDirection;
};

namespace VSTGUI {
    struct EQCurveViewCreator : public ViewCreatorAdapter {
        EQCurveViewCreator() { UIViewFactory::registerViewCreator(*this); }
        IdStringPtr getViewName() const override { return "EQCurveView"; }
        IdStringPtr getBaseViewName() const override { return UIViewCreator::kCView; }
        CView* create(const UIAttributes& attr, const IUIDescription* d) const override {
            fprintf(stderr, "creating custom view");
            auto* view = new EQCurveView({0, 0, 100, 100});

            auto* controller = dynamic_cast<VST3Editor*>(d->getController());
            if (controller)
            {
                auto* editController = controller->getController();
                if (editController)
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
