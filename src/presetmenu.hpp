//
// Created by Darek Rudiš on 06.08.2025.
//

/**
 * @file presetmenu.hpp
 * @brief Defines all classes related to the preset menu selector view object.
 */

#ifndef PRESETMENU_HPP
#define PRESETMENU_HPP

#include "controller.hpp"
#include "vstgui/lib/controls/coptionmenu.h"
#include "vstgui/uidescription/iviewcreator.h"
#include "vstgui/uidescription/uiviewfactory.h"
#include "vstgui/uidescription/detail/uiviewcreatorattributes.h"


using controller_t = Steinberg::Vst::EditController*;

namespace VSTGUI {
    /**
     * @brief Custom listener that overrides default SDK parameter update behavior.
     */
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

    /**
     * @brief View object for selecting EQ curve shapes.
     *
     * @see eqshape.hpp
     */
    class PresetMenu : public COptionMenu
    {
    public:
        explicit PresetMenu(const CRect& size, const CustomParamID& tag);

        CLASS_METHODS(PresetMenu, COptionMenu)
    };


    /**
     * @brief @ref PresetMenu factory class.
     */
    class PresetMenuCreator : public ViewCreatorAdapter
    {
    public:
        PresetMenuCreator () { UIViewFactory::registerViewCreator(*this); }

        [[nodiscard]] IdStringPtr getViewName () const override { return "PresetMenu"; }
        [[nodiscard]] IdStringPtr getBaseViewName () const override { return UIViewCreator::kCOptionMenu; }
        /**
         * @brief Creates an instance of @ref PresetMenu.
         *
         * Injects the @ref PresetMenuListener. Binds the kParamShape parameter tag from @ref CustomParamID.
         * @return An instance of @ref PresetMenu
         */
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
