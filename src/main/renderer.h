#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <cassert>
#include <memory>

#include "common/common.h"
#include "window.h"

namespace amts {
    class RenderingTarget;

    class Renderer {
        private:
            SDL_Renderer* m_sdlRenderer; 
            bool m_started;

        public:
            Renderer(const std::unique_ptr<Window>& window);

            ~Renderer();

            void present_target(const std::unique_ptr<RenderingTarget>& target);
            void present();

            void begin();
            void end();

            void clear(const Color& color);

            SDL_Renderer* get_sdl_renderer();
    };
}

#endif