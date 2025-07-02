//
// Created by Darek Rudiš on 02.07.2025.
//

#include "eqcurveview.hpp"

void EQCurveView::draw(VSTGUI::CDrawContext* dc)
{
    dc->setDrawMode(VSTGUI::kAntiAliasing);
    dc->setLineWidth(2);
    dc->setFrameColor(VSTGUI::CColor(0, 255, 0, 255));
    dc->drawLine(VSTGUI::CPoint(0, getHeight() / 2), VSTGUI::CPoint(getWidth(), getHeight() / 2));
}

