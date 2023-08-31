#ifndef _RENDERERING_TARGET_H_
#define _RENDERERING_TARGET_H_

#include "common.h"

namespace amts {
    class RendereringTarget {
        private:
            SDL_Texture* m_sdlTexture;
            const SDL_Rect m_sdlTextureRect;

            const u64 m_width;
            const u64 m_height;

            bool m_locked;

            void *m_pixelData;
            i32 m_pixelDataPitch;

        public:
            RendereringTarget(SDL_Renderer* sdlRenderer, const u64& width, const u64& height) 
                : m_sdlTexture(nullptr), m_sdlTextureRect{0, 0, width, height}, m_width(width), m_height(height), m_locked(false) {
            
                m_sdlTexture = SDL_CreateTexture(
                    sdlRenderer, 
                    SDL_PIXELFORMAT_ABGR8888, 
                    SDL_TEXTUREACCESS_STREAMING, 
                    static_cast<i32>(m_width),
                    static_cast<i32>(m_height)
                );
            }

            void lock() {
                SDL_LockTexture(m_sdlTexture, &m_sdlTextureRect, &m_pixelData, &m_pixelDataPitch);
                m_locked = true;
            }

            void unlock() {
                SDL_UnlockTexture(m_sdlTexture);
                m_locked = false;
            }

            ~RendereringTarget() {
                SDL_DestroyTexture(m_sdlTexture);
            }
    };
}

#endif