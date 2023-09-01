#ifndef _RENDERERING_TARGET_H_
#define _RENDERERING_TARGET_H_

#include <memory>

#include "common/common.h"
#include "renderer.h"

namespace amts {
    class RenderingTarget {
        private:
            SDL_Texture* m_sdlTexture;

            const SDL_Rect m_sdlTextureRect;
            const SDL_FRect m_sdlTextureRectF;

            const u64 m_width;
            const u64 m_height;

            bool m_locked;

            void* m_pixelData;
            i32 m_pixelDataPitch;

            Color* m_accumulatedColor;

        public:
            RenderingTarget(const std::unique_ptr<Renderer>& renderer, const u64& width, const u64& height) 
                : m_sdlTexture(nullptr), 
                  m_sdlTextureRect{0, 0, static_cast<i32>(width), static_cast<i32>(height)}, 
                  m_sdlTextureRectF{0.0f, 0.0f, static_cast<f32>(width), static_cast<f32>(height)}, 
                  m_width(width), 
                  m_height(height), 
                  m_locked(false) {
            
                m_sdlTexture = SDL_CreateTexture(
                    renderer->get_sdl_renderer(), 
                    SDL_PIXELFORMAT_ABGR8888, 
                    SDL_TEXTUREACCESS_STREAMING, 
                    static_cast<i32>(m_width),
                    static_cast<i32>(m_height)
                );

                m_accumulatedColor = static_cast<Color*>(malloc(m_width * m_height * sizeof(Color)));
            }

            void lock() {
                SDL_LockTexture(m_sdlTexture, &m_sdlTextureRect, &m_pixelData, &m_pixelDataPitch);
                m_locked = true;
            }

            void unlock() {
                SDL_UnlockTexture(m_sdlTexture);
                m_locked = false;
            }

            Color* get_accumulated_color() {
                return m_accumulatedColor;
            }

            void reset_accumulation() {
                for(u64 x = 0; x < m_width; ++x) {
                    for(u64 y = 0; y < m_height; ++y) {
                        m_accumulatedColor[x + y * m_width] = Color(0.0f, 0.0f, 0.0f);
                    }
                }
            }

            const bool& is_locked() const {
                return m_locked;
            }

            u32* get_pixel_data() {
                return static_cast<u32*>(m_pixelData);
            } 

            const u64& get_width() const {
                return m_width;
            }

            const u64& get_height() const {
                return m_height;
            }

            SDL_Texture* get_sdl_texture() {
                return m_sdlTexture;
            }

            const SDL_Rect& get_texture_rect() const {
                return m_sdlTextureRect;
            }

            const SDL_FRect& get_texture_rect_f() const {
                return m_sdlTextureRectF;
            }

            ~RenderingTarget() {
                SDL_DestroyTexture(m_sdlTexture);
                free(m_accumulatedColor);
            }
    };
}

#endif