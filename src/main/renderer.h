#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <memory>

#include "common.h"
#include "window.h"
#include "rendering_target.h"

namespace amts {
    class Renderer {
        private:
            SDL_Renderer* m_sdlRenderer; 

        public:
            Renderer(const std::unique_ptr<Window>& window) 
                : m_sdlRenderer(nullptr) {
                
                m_sdlRenderer = SDL_CreateRenderer(window->get_sdl_window(), NULL, SDL_RENDERER_ACCELERATED);
            }

            ~Renderer() {
                SDL_DestroyRenderer(m_sdlRenderer);
            }

            void Render() {

            }
    };
}

#endif