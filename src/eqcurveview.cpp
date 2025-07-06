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

    // hardcoded params for now
    float shift = 0.5f;
    float intensity = 1.0f;
    float direction = 0.0f;
    float width = 0.5f;


    float centerFreq = expf(logf(EQ::minFreq) + shift * (logf(EQ::maxFreq) - logf(EQ::minFreq)));
    float flip = 1.0f - 2.0f * direction;
    float dBpeakGain = flip * intensity * EQ::dBMaxBoost;

    auto path = dc->createGraphicsPath();

    const int numPoints = static_cast<int>(getWidth());
    for (int i = 0; i < numPoints; ++i)
    {
        float normX = static_cast<float>(i) / numPoints;
        float logFreq = log10f(EQ::minFreq) + normX * (log10f(EQ::maxFreq) - log10f(EQ::minFreq));
        float freq = powf(10.0f, logFreq);

        float x = (freq - centerFreq) / (centerFreq * width);
        // TODO remove hardcoded shape
        float shape = -powf(x, 4.0f) + powf(x, 3.0f) + powf(x, 2.0f) + 0.5f;
        if (shape < 0.0f) shape = 0.0f;

        float dBgain = dBpeakGain * shape;

        float px = freqToX(freq);
        float py = dbToY(dBgain);

        if (i == 0)
            path->beginSubpath({px, py});
        else
            path->addLine({px, py});
    }

    dc->drawGraphicsPath(path, VSTGUI::CDrawContext::kPathStroked);
    dc->drawGraphicsPath(path, VSTGUI::CDrawContext::kPathStroked);
    path->forget();
    // TODO trigger invalid state upon param change
    //dc->drawLine(VSTGUI::CPoint(0, getHeight() / 2), VSTGUI::CPoint(getWidth(), getHeight() / 2));
}

float EQCurveView::freqToX(const float freq) const
{
    float normX = (log10f(freq) - log10f(EQ::minFreq))/ (log10f(EQ::maxFreq) - log10f(EQ::minFreq));
    return normX * getWidth();
}

float EQCurveView::dbToY(const float db) const
{
    constexpr float padding = 20.0f;
    constexpr float visualMaxDB = EQ::dBMaxBoost + 8.0f;
    float drawableHeight = getHeight() - 2 * padding;

    float normY = (visualMaxDB - db) / (2 * visualMaxDB);
    return padding + normY * drawableHeight;
}
