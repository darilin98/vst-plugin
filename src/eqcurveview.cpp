//
// Created by Darek Rudiš on 02.07.2025.
//

#include "eqcurveview.hpp"

#include "controller.hpp"

void EQCurveView::draw(VSTGUI::CDrawContext* dc)
{
    dc->setDrawMode(VSTGUI::kAntiAliasing);

    drawFreqMarkers(dc);
    drawDbMarkers(dc);

    dc->setLineWidth(2);
    dc->setFrameColor(VSTGUI::CColor(0, 255, 0, 255));

    const float centerFreq = expf(logf(EQ::minFreq) + _shift * (logf(EQ::maxFreq) - logf(EQ::minFreq)));
    const float flip = 1.0f - 2.0f * _direction;
    const float dBpeakGain = flip * _intensity * EQ::dBMaxBoost;

    const auto path = dc->createGraphicsPath();

    const int numPoints = static_cast<int>(getWidth());
    for (int i = 0; i < numPoints; ++i)
    {
        const float normX = static_cast<float>(i) / numPoints;
        const float logFreq = log10f(EQ::minFreq) + normX * (log10f(EQ::maxFreq) - log10f(EQ::minFreq));
        const float freq = powf(10.0f, logFreq);

        const float x = (freq - centerFreq) / (centerFreq * _width);
        float shape = shapeFromPreset(_shape, x);
        if (shape < 0.0f) shape = 0.0f;

        const float dBgain = dBpeakGain * shape;

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
}

void EQCurveView::setParamListeners(controller_t controller)
{
    this->_controller = controller;

    _shiftListener = std::make_unique<CustomParamListener>(
        _controller->getParameterObject(kParamShift), this, _controller);
    _widthListener = std::make_unique<CustomParamListener>(
        _controller->getParameterObject(kParamWidth), this, _controller);
    _intensityListener = std::make_unique<CustomParamListener>(
        _controller->getParameterObject(kParamIntensity), this, _controller);
    _directionListener = std::make_unique<CustomParamListener>(
        _controller->getParameterObject(kParamDirection), this, _controller);
    _shapeListener = std::make_unique<CustomParamListener>(
        _controller->getParameterObject(kParamShape), this, _controller);
}

void EQCurveView::onParamChanged(const Steinberg::Vst::ParamID id, const float normalizedValue)
{
    switch (id)
    {
        case kParamShift:
            _shift = normalizedValue;
        break;
        case kParamWidth:
            _width = normalizedValue;
        break;
        case kParamIntensity:
            _intensity = normalizedValue;
        break;
        case kParamDirection:
            _direction = normalizedValue;
        break;
        case kParamShape:
        {
            int maxShapeIndex = static_cast<int>(EqShapePreset::Count) - 1;
            int shapeIndex = static_cast<int>(normalizedValue * maxShapeIndex + 0.5f);
            shapeIndex = std::clamp(shapeIndex, 0, maxShapeIndex);
            _shape = static_cast<EqShapePreset>(shapeIndex);
            break;
        }
    }
}



float EQCurveView::freqToX(const float freq) const
{
    const float normX = (log10f(freq) - log10f(EQ::minFreq))/ (log10f(EQ::maxFreq) - log10f(EQ::minFreq));
    return normX * getWidth();
}

float EQCurveView::dbToY(const float db) const
{
    constexpr float visualMaxDB = EQ::dBMaxBoost + 8.0f;
    const float drawableHeight = getHeight() - 2 * _padding;

    const float normY = (visualMaxDB - db) / (2 * visualMaxDB);
    return _padding + normY * drawableHeight;
}

void EQCurveView::drawFreqMarkers(VSTGUI::CDrawContext* dc) const
{
    dc->setLineWidth(1);
    dc->setFrameColor(VSTGUI::CColor(100, 100, 100, 128));
    dc->setFont(VSTGUI::kNormalFontVerySmall);
    dc->setFontColor(VSTGUI::CColor(180, 180, 180, 255));

    static const std::vector<float> freqMarkers = {
        30.0f, 50.0f, 100.0f, 200.0f, 500.0f,
        1000.0f, 2000.0f, 5000.0f, 10000.0f
    };

    for (const float freq : freqMarkers)
    {
        if (freq < EQ::minFreq || freq > EQ::maxFreq)
            continue;

        float x = freqToX(freq);
        dc->drawLine(VSTGUI::CPoint(x, 0), VSTGUI::CPoint(x, getHeight()));

        std::string label;
        if (freq >= 1000.0f)
            label = std::to_string(static_cast<int>(freq / 1000.0f)) + "k";
        else
            label = std::to_string(static_cast<int>(freq));

        VSTGUI::CRect textRect(x - 15, getHeight() - 15, x + 15, getHeight());
        dc->drawString(label.c_str(), textRect, VSTGUI::kCenterText);
    }
}

void EQCurveView::drawDbMarkers(VSTGUI::CDrawContext *dc) const
{

    static const std::vector<float> dbMarkers = { 18.0f, 12.0f, 6.0f, -6.0f, -12.0f, -18.0f};

    dc->setFont(VSTGUI::kNormalFontVerySmall);
    dc->setFontColor(VSTGUI::CColor(180, 180, 180, 255));

    for (float db : dbMarkers)
    {
        float y = dbToY(db);

        if (y < _padding || y > getHeight() - _padding)
            continue;

        std::string label;
        if (db > 0)
            label = "+" + std::to_string(static_cast<int>(db)) + "dB";
        else
            label = std::to_string(static_cast<int>(db)) + "dB";

        VSTGUI::CRect textRect(3, y - 6, 30, y + 6);
        dc->drawString(label.c_str(), textRect, VSTGUI::kLeftText);
    }
}
