#ifndef _COMMON_UI_WINDOW_H_
#define _COMMON_UI_WINDOW_H_

#include "common.h"

namespace amts {
    class CommonUIWindow {
        private:
            const std::string m_title;

            bool m_focused;

            void update_iternal_state() {
                m_focused = ImGui::IsWindowFocused();
            }
        protected:
            bool start_window() {
                update_iternal_state();

                if(ImGui::Begin(m_title.c_str())) {
                    return true;
                }

                ImGui::End();
                return false;
            }

            void end_window() {
                ImGui::End();
            }

        public:
            CommonUIWindow(const std::string& title) 
                : m_title(title),
                  m_focused(false) {

            }

            virtual ~CommonUIWindow() {

            }

            const bool& is_focused() const {
                return m_focused;
            }
    };
}

#endif