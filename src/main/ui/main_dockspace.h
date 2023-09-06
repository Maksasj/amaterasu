#ifndef _MAIN_DOCKSPACE_WINDOW_H_
#define _MAIN_DOCKSPACE_WINDOW_H_

#include <functional>
#include "common/common_ui_window.h"
#include "themes/themes.h"

namespace amts {
    class MainDockspaceUIWindow : public CommonUIWindow {
        private:
            std::unique_ptr<Theme> m_activeTheme; // Todo, move this somewhere

        public:
            MainDockspaceUIWindow() : CommonUIWindow("Main Dockspace") {
                m_activeTheme = std::make_unique<DeepDarkTheme>();
                m_activeTheme->apply_theme();
            }

            ~MainDockspaceUIWindow() override {
                m_activeTheme = nullptr;
            }

            void run(const std::function<void(void)>& dockSpaceBody) {
                static bool open = true;

                ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
                windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

                const ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->WorkPos);
                ImGui::SetNextWindowSize(viewport->WorkSize);
                ImGui::SetNextWindowViewport(viewport->ID);
                
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
                
                ImGui::Begin("DockSpace Demo", &open, windowFlags);

                ImGui::PopStyleVar();
                ImGui::PopStyleVar();
                ImGui::PopStyleVar();

                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
                
                dockSpaceBody();

                ImGui::End();
            }
    };
}

#endif