//
// Created by Darek Rudiš on 07.07.2025.
//
#include "eqcurveview.hpp"

CustomParamListener::CustomParamListener(param_t param, view_t view, controller_t controller)
    : _param(param), _view(view), _controller(controller)
{
    if (param)
        param->addDependent(this);
}
CustomParamListener::~CustomParamListener()
{
    if (_param)
        _param->removeDependent(this);
}

void CustomParamListener::update(Steinberg::FUnknown *changedUnknown, Steinberg::int32 message)
{
    if (message == IDependent::kChanged && _view && _controller && _param) {
        float normalizedValue = static_cast<float>(_controller->getParamNormalized(_param->getInfo().id));
        _view->onParamChanged(_param->getInfo().id, normalizedValue);
        _view->invalid();
    }
}
