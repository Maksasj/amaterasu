#ifndef _SDL_TEXTURE_H_
#define _SDL_TEXTURE_H_

#include <memory>

#include "common/common.h"
#include "texture_buffer.h"

namespace amts {
    class Renderer;

    class SDLTexture : public TextureBuffer<u32> {
        private:
            SDL_Texture* m_sdlTexture;

            const SDL_Rect m_sdlTextureRect;
            const SDL_FRect m_sdlTextureRectF;

            i32 m_pixelDataPitch;

        public:
            SDLTexture(std::unique_ptr<Renderer>& renderer, const u64& width, const u64& height);
            ~SDLTexture() override;

            SDL_Texture* get_sdl_texture();

            void lock() {
                SDL_LockTexture(m_sdlTexture, &m_sdlTextureRect, get_pixel_data_ptr(), &m_pixelDataPitch);
            }

            void unlock() {
                SDL_UnlockTexture(m_sdlTexture);
            }

            const SDL_Rect& get_texture_rect() const;
            const SDL_FRect& get_texture_rect_f() const;
    };
}

#endif