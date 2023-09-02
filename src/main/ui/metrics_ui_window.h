#ifndef _METRICS_UI_WINDOW_H_
#define _METRICS_UI_WINDOW_H_

#include "common/common_ui_window.h"

namespace amts {
    class MetricsUIWindow : public CommonUIWindow {
        private:

        public:
            MetricsUIWindow() : CommonUIWindow("Metrics") {

            }

            ~MetricsUIWindow() override {
                
            }

            void run() {
                if(!CommonUIWindow::start_window()) return;

                ImGuiIO& io = ImGui::GetIO();

                ImGui::Text("Dear ImGui %s", ImGui::GetVersion());
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
                ImGui::Text("%d visible windows, %d active allocations", io.MetricsRenderWindows, io.MetricsActiveAllocations);

                CommonUIWindow::end_window();
            }
    };
}

#endif