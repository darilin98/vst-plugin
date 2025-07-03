//
// Created by Darek Rudiš on 02.07.2025.
//

#include "eqcurveview.hpp"

#include "vstgui/lib/cgraphicspath.h"

void EQCurveView::draw(VSTGUI::CDrawContext* dc)
{
    dc->setDrawMode(VSTGUI::kAntiAliasing);
    dc->setLineWidth(2);
    dc->setFrameColor(VSTGUI::CColor(0, 255, 0, 255));

    const auto width = getWidth();
    const auto height = getHeight();
    std::vector<VSTGUI::CPoint> points;

    for (int x = 0; x < static_cast<int>(width); ++x)
    {
        float normX = static_cast<float>(x) / width;

        // hard coded for now
        float shape = -powf(normX, 4.0f) + powf(normX, 3.0f) + powf(normX, 2.0f) + 0.5f;
        float y = (1.0f - shape) * height;
        points.emplace_back(static_cast<float>(x), y);
    }
    auto path = dc->createGraphicsPath();
    path->beginSubpath(points[0]);
    for (int i = 1; i < points.size(); ++i) {
        path->addLine(points[i]);
    }
    dc->drawGraphicsPath(path, VSTGUI::CDrawContext::kPathStroked);
    path->forget();
    // TODO trigger invalid state upon param change
    //dc->drawLine(VSTGUI::CPoint(0, getHeight() / 2), VSTGUI::CPoint(getWidth(), getHeight() / 2));
}

