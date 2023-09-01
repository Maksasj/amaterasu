#include "renderer.h"
#include "rendering_target.h"

amts::Renderer::Renderer::Renderer(const std::unique_ptr<Window>& window) 
    : m_sdlRenderer(nullptr) {

    m_sdlRenderer = SDL_CreateRenderer(window->get_sdl_window(), NULL, SDL_RENDERER_ACCELERATED);
}

amts::Renderer::Renderer::~Renderer() {
    SDL_DestroyRenderer(m_sdlRenderer);
}

void amts::Renderer::present_target(const std::unique_ptr<RenderingTarget>& target) {
    assert(target->is_locked() || std::cout << "Target should be locked before trying to do present" << std::endl);

    SDL_RenderTexture(m_sdlRenderer, target->get_sdl_texture(), &target->get_texture_rect_f(), &target->get_texture_rect_f());
}

void amts::Renderer::clear(const Color& color) {
    SDL_SetRenderDrawColor(
        m_sdlRenderer,  
        static_cast<u8>(255.0f * color.r()), 
        static_cast<u8>(255.0f * color.g()), 
        static_cast<u8>(255.0f * color.b()), 
        static_cast<u8>(255.0f * color.a())
    );

    SDL_RenderClear(m_sdlRenderer);
}

void amts::Renderer::present() {
    SDL_RenderPresent(m_sdlRenderer);
}

SDL_Renderer* amts::Renderer::get_sdl_renderer() {
    return m_sdlRenderer;
}
