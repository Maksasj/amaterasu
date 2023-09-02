#ifndef _SDL_RENDERERING_TARGET_H_
#define _SDL_RENDERERING_TARGET_H_

#include <memory>

#include "common/common.h"
#include "rendering_target.h"

namespace amts {
    class Renderer;

    class SDLRenderingTarget : public RenderingTarget {
        private:
            SDL_Texture* m_sdlTexture;

            const SDL_Rect m_sdlTextureRect;
            const SDL_FRect m_sdlTextureRectF;

            i32 m_pixelDataPitch;

        public:
            SDLRenderingTarget(std::unique_ptr<Renderer>& renderer, const u64& width, const u64& height);
            ~SDLRenderingTarget() override;

            void lock() override;
            void unlock() override;

            SDL_Texture* get_sdl_texture();

            const SDL_Rect& get_texture_rect() const;
            const SDL_FRect& get_texture_rect_f() const;
    };
}

#endif