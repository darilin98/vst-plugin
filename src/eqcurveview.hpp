//
// Created by Darek Rudiš on 02.07.2025.
//

#ifndef EQCURVEVIEW_HPP
#define EQCURVEVIEW_HPP

#include "eqconstants.hpp"
#include "vstgui/lib/cview.h"
#include "vstgui/uidescription/iuidescription.h"
#include "vstgui/uidescription/iviewcreator.h"
#include "vstgui/uidescription/uiattributes.h"
#include "vstgui/uidescription/uiviewfactory.h"
#include "vstgui/uidescription/detail/uiviewcreatorattributes.h"
#include "vstgui/lib/cdrawcontext.h"

class EQCurveView : public VSTGUI::CView {
public:
    explicit EQCurveView(const VSTGUI::CRect& size)
        : CView(size) { }

    void draw(VSTGUI::CDrawContext* dc) override;
private:
    [[nodiscard]] float freqToX(float freq) const;
    [[nodiscard]] float dbToY(float db) const;
};

namespace VSTGUI {
    struct EQCurveViewCreator : public ViewCreatorAdapter {
        EQCurveViewCreator() { UIViewFactory::registerViewCreator(*this); }
        IdStringPtr getViewName() const override { return "EQCurveView"; }
        IdStringPtr getBaseViewName() const override { return UIViewCreator::kCView; }
        CView* create(const UIAttributes& attr, const IUIDescription* d) const override {
            fprintf(stderr, "creating custom view");
            return new EQCurveView({0, 0, 100, 100});
        }
    };
    static EQCurveViewCreator __gEQCurveViewCreator;
}
#endif //EQCURVEVIEW_HPP
