#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <string>

#include "common/common.h"
#include "event_receiver.h"

namespace amts {
    class Window : public EventReceiver {
        private:
            bool m_close;

            SDL_Window* m_sdlWindow;

            std::string m_title;

            u64 m_width;
            u64 m_height;

        public:
            Window(const std::string& title, const u64& width, const u64& height) 
                : m_close(false), m_sdlWindow(nullptr), m_title(title), m_width(width), m_height(height) {

                const auto windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
                // const auto windowFlags = 0;
                
                m_sdlWindow = SDL_CreateWindow(title.c_str(), m_width, m_height, windowFlags);
            }

            ~Window() override {
                SDL_DestroyWindow(m_sdlWindow);
            }

            void handle_event(const SDL_Event& event) override {
                if(event.type == SDL_EVENT_QUIT) {
                    m_close = true;
                }
            }

            const bool& is_open() const {
                return m_close;
            }

            const u64& get_width() const {
                return m_width;
            }

            const u64& get_height() const {
                return m_height;
            }

            SDL_Window* get_sdl_window() {
                return m_sdlWindow;
            }

            void force_close() {
                m_close = true;
            }

    };
}

#endif