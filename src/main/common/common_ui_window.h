#ifndef _COMMON_UI_WINDOW_H_
#define _COMMON_UI_WINDOW_H_

#include "common.h"

namespace amts {
    class CommonUIWindow {
        private:
            const std::string m_title;

            bool m_focused;
            bool m_open;

            void update_iternal_state() {
                m_focused = ImGui::IsWindowFocused();
            }

        protected:
            bool start_window() {
                if(m_open == false) 
                    return false;

                if(ImGui::Begin(m_title.c_str(), &m_open)) {
                    update_iternal_state();
                    return true;
                }

                ImGui::End();
                return false;
            }

            void end_window() {
                ImGui::End();
            }

        public:
            CommonUIWindow(const std::string& title, const bool& open = true) 
                : m_title(title),
                  m_focused(false),
                  m_open(open) {

            }

            virtual ~CommonUIWindow() {

            }

            const bool& is_focused() const {
                return m_focused;
            }

            const bool& is_open() const {
                return m_open;
            }

            bool& ref_open_flag() {
                return m_open;
            }

            void force_open() {
                m_open = true;
            }

            void force_close() {
                m_open = false;
            }
    };
}

#endif