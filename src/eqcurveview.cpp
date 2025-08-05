//
// Created by Darek Rudiš on 02.07.2025.
//

#include "eqcurveview.hpp"

#include "controller.hpp"

void EQCurveView::draw(VSTGUI::CDrawContext* dc)
{
    dc->setDrawMode(VSTGUI::kAntiAliasing);
    dc->setLineWidth(2);
    dc->setFrameColor(VSTGUI::CColor(0, 255, 0, 255));

    float centerFreq = expf(logf(EQ::minFreq) + _shift * (logf(EQ::maxFreq) - logf(EQ::minFreq)));
    float flip = 1.0f - 2.0f * _direction;
    float dBpeakGain = flip * _intensity * EQ::dBMaxBoost;

    auto path = dc->createGraphicsPath();

    const int numPoints = static_cast<int>(getWidth());
    for (int i = 0; i < numPoints; ++i)
    {
        float normX = static_cast<float>(i) / numPoints;
        float logFreq = log10f(EQ::minFreq) + normX * (log10f(EQ::maxFreq) - log10f(EQ::minFreq));
        float freq = powf(10.0f, logFreq);

        float x = (freq - centerFreq) / (centerFreq * _width);
        float shape = shapeFromPreset(_shape, x);
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

void EQCurveView::onParamChanged(Steinberg::Vst::ParamID id, float normalizedValue)
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
            int shapeIndex = static_cast<int>(normalizedValue * static_cast<int>(EqShapePreset::Count));
            _shape = static_cast<EqShapePreset>(shapeIndex);
            break;
        }
    }
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
