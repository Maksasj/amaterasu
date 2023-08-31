#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <string>

#include "common.h"

namespace amts {
    class Window {
        private:
            SDL_Window* m_sdlWindow;

            std::string m_title;

            u64 m_width;
            u64 m_height;

        public:
            Window(const std::string& title, const u64& width, const u64& height) 
                : m_sdlWindow(nullptr), m_title(title), m_width(width), m_height(height) {

                m_sdlWindow = SDL_CreateWindow(title.c_str(), m_width, m_height, 0);
            }

            ~Window() {
                SDL_DestroyWindow(m_sdlWindow);
            }

            SDL_Window* get_sdl_window() {
                return m_sdlWindow;
            }

    };
}

#endif