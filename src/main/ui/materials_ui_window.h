#ifndef _MATERIALS_UI_WINDOW_H_
#define _MATERIALS_UI_WINDOW_H_

#include "common/common_ui_window.h"
#include "materials/material_pool.h"

namespace amts {
    class MaterialsUIWindow : public CommonUIWindow {
        private:

        public:
            MaterialsUIWindow() : CommonUIWindow("Materials") {

            }

            ~MaterialsUIWindow() override {
                
            }

            void run(std::unique_ptr<MaterialPool>& materials) {
                if(!CommonUIWindow::start_window()) return;

                std::ignore = materials;          

                CommonUIWindow::end_window();
            }
    };
}

#endif