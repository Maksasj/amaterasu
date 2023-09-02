#ifndef _RESULT_VIEW_UI_WINDOW_H_
#define _RESULT_VIEW_UI_WINDOW_H_

#include "common/common_ui_window.h"
#include "rendering_target.h"

namespace amts {
    class ResultViewUIWindow : public CommonUIWindow {
        private:

        public:
            ResultViewUIWindow() : CommonUIWindow("Result View") {

            }

            ~ResultViewUIWindow() override {
                
            }

            void run(std::unique_ptr<RenderingTarget>& target) {
                if(!CommonUIWindow::start_window()) return;

                auto imageSize = ImVec2(target->get_width(), target->get_height());
                auto centerPosition = ImVec2((ImGui::GetWindowSize().x - imageSize.x) * 0.5f, (ImGui::GetWindowSize().y - imageSize.y) * 0.5f);

                ImGui::SetCursorPos(centerPosition);
                ImGui::Image((void*)target->get_sdl_texture(), imageSize);

                ImGui::End();

                CommonUIWindow::end_window();
            }
    };
}

#endif