#include "renderer.h"
#include "rendering_target.h"

amts::Renderer::Renderer::Renderer(const std::unique_ptr<Window>& window) : m_sdlRenderer(nullptr) {
    m_sdlRenderer = SDL_CreateRenderer(window->get_sdl_window(), NULL, SDL_RENDERER_ACCELERATED);
}

amts::Renderer::Renderer::~Renderer() {
    SDL_DestroyRenderer(m_sdlRenderer);
}

amts::Color amts::Renderer::per_pixel(const u64& x, const u64& y, const u64& width, const u64& height) const {
    std::ignore = x;
    std::ignore = y;
    std::ignore = width;
    std::ignore = height;

    return Color(1.0f, 0.0f, 0.0f);
}

void amts::Renderer::render(const std::unique_ptr<RenderingTarget>& target) {
    assert(target->is_locked() || std::cout << "Target should be locked before trying to render" << std::endl);

    const auto targetWidth = target->get_width();
    const auto targetHeight = target->get_height();

    u32* pixels = target->get_pixel_data();

    for(u64 x = 0; x < targetWidth; ++x) {
        for(u64 y = 0; y < targetHeight; ++y) {
            const Color color = per_pixel(x, y, targetWidth, targetHeight);

            pixels[x + y * targetWidth] = color.to_u32();
        }
    }
}

void amts::Renderer::present_target(const std::unique_ptr<RenderingTarget>& target) {
    assert(target->is_locked() || std::cout << "Target should be locked before trying to do present" << std::endl);

    SDL_RenderTexture(m_sdlRenderer, target->get_sdl_texture(), &target->get_texture_rect_f(), &target->get_texture_rect_f());
    SDL_RenderPresent(m_sdlRenderer);
}

SDL_Renderer* amts::Renderer::get_sdl_renderer() {
    return m_sdlRenderer;
}
