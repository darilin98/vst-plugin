//
// Created by Darek Rudiš on 06.08.2025.
//

#ifndef PRESETMENU_HPP
#define PRESETMENU_HPP

#include "controller.hpp"
#include "vstgui/lib/controls/coptionmenu.h"
#include "vstgui/uidescription/iviewcreator.h"
#include "vstgui/uidescription/uiviewfactory.h"
#include "vstgui/uidescription/detail/uiviewcreatorattributes.h"

using controller_t = Steinberg::Vst::EditController*;

namespace VSTGUI {

    class PresetMenuListener : public IOptionMenuListener {
    public:
        explicit PresetMenuListener(controller_t controller) : _controller(controller) {}

        void onOptionMenuPrePopup(COptionMenu* menu) override;
        void onOptionMenuPostPopup(COptionMenu* menu) override;
        bool onOptionMenuSetPopupResult(COptionMenu* menu, COptionMenu* selectedMenu, int32_t selectedIndex) override;

        virtual ~PresetMenuListener() = default;
    private:
        controller_t _controller;
    };

    class PresetMenu : public COptionMenu
    {
    public:
        explicit PresetMenu(const CRect& size, const CustomParamID& tag);

        CLASS_METHODS(PresetMenu, COptionMenu)
    };


    class PresetMenuCreator : public ViewCreatorAdapter
    {
    public:
        PresetMenuCreator () { UIViewFactory::registerViewCreator(*this); }

        [[nodiscard]] IdStringPtr getViewName () const override { return "PresetMenu"; }
        [[nodiscard]] IdStringPtr getBaseViewName () const override { return UIViewCreator::kCOptionMenu; }

        CView* create (const UIAttributes& attributes, const IUIDescription* description) const override
        {
            const CRect size (CPoint (0, 0), CPoint (100, 20));
            constexpr CustomParamID tag = kParamShape;
            auto menu = new PresetMenu(size, tag);

            if (auto* controller = dynamic_cast<VST3Editor*>(description->getController()))
            {
                if (auto* editController = controller->getController())
                {
                    static PresetMenuListener listener(editController);
                    menu->registerOptionMenuListener(&listener);
                }
            }
            return menu;
        }
    };

    PresetMenuCreator __gPresetMenuCreator;
} // namespace VSTGUI



#endif //PRESETMENU_HPP
