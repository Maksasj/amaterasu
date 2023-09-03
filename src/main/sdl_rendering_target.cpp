#include "sdl_rendering_target.h"
#include "renderer.h"

amts::SDLRenderingTarget::SDLRenderingTarget(std::unique_ptr<Renderer>& renderer, const u64& width, const u64& height) 
    : RenderingTarget(width, height),
        m_sdlTexture(nullptr),
        m_sdlTextureRect{0, 0, static_cast<i32>(width), static_cast<i32>(height)}, 
        m_sdlTextureRectF{0.0f, 0.0f, static_cast<f32>(width), static_cast<f32>(height)} {

    m_sdlTexture = SDL_CreateTexture(
        renderer->get_sdl_renderer(), 
        SDL_PIXELFORMAT_ABGR8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        static_cast<i32>(get_width()),
        static_cast<i32>(get_height())
    );
}

amts::SDLRenderingTarget::~SDLRenderingTarget() {
    SDL_DestroyTexture(m_sdlTexture);
}

void amts::SDLRenderingTarget::lock() {
    RenderingTarget::lock();

    SDL_LockTexture(m_sdlTexture, &m_sdlTextureRect, get_pixel_data_ptr(), &m_pixelDataPitch);
}

void amts::SDLRenderingTarget::unlock() {
    RenderingTarget::unlock();

    // SDL_UpdateTexture(m_sdlTexture, NULL, get_pixel_data_ptr(), m_pixelDataPitch);
    SDL_UnlockTexture(m_sdlTexture);
}

SDL_Texture* amts::SDLRenderingTarget::get_sdl_texture() {
    return m_sdlTexture;
}

const SDL_Rect& amts::SDLRenderingTarget::get_texture_rect() const {
    return m_sdlTextureRect;
}

const SDL_FRect& amts::SDLRenderingTarget::get_texture_rect_f() const {
    return m_sdlTextureRectF;
}
