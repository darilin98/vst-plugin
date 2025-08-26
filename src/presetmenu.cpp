//
// Created by Darek Rudiš on 06.08.2025.
//

#include "presetmenu.hpp"

#include "eqshape.hpp"

namespace VSTGUI {

    PresetMenu::PresetMenu(const CRect& size, const CustomParamID& tag)
    : COptionMenu(size, nullptr, tag)
    { }

    void PresetMenuListener::onOptionMenuPostPopup(COptionMenu *menu) {}

    void PresetMenuListener::onOptionMenuPrePopup(COptionMenu *menu) {}

    bool PresetMenuListener::onOptionMenuSetPopupResult(COptionMenu *menu, COptionMenu *selectedMenu, int32_t selectedIndex)
    {
        if (!_controller || !menu)
            return false;

        ParamID tag = menu->getTag();
        auto* param = _controller->getParameterObject(tag);
        if (!param)
            return false;

        const auto normalized = static_cast<float>(selectedIndex) / static_cast<float>(param->getInfo().stepCount);

        fprintf(stderr, "Setting param ID %d to index %d (normalized = %f)\n", tag, selectedIndex, normalized);

        _controller->setParamNormalized(tag, normalized);

        menu->setValue(static_cast<float>(selectedIndex));

        return true;
    }

} // namespace VSTGUI