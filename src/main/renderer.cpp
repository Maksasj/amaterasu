#include "renderer.h"
#include "rendering_target.h"

amts::Renderer::Renderer::Renderer(const std::unique_ptr<Window>& window) 
    : m_sdlRenderer(nullptr), m_started(false) {

    m_sdlRenderer = SDL_CreateRenderer(window->get_sdl_window(), NULL, SDL_RENDERER_ACCELERATED);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    ImGui_ImplSDL3_InitForSDLRenderer(window->get_sdl_window(), m_sdlRenderer);
    ImGui_ImplSDLRenderer3_Init(m_sdlRenderer);
}

amts::Renderer::Renderer::~Renderer() {
    SDL_DestroyRenderer(m_sdlRenderer);
}

void amts::Renderer::present_target(const std::unique_ptr<SDLRenderingTarget>& target) {
    assert(target->is_locked() || std::cout << "Target should be locked before trying to do present" << std::endl);

    SDL_RenderTexture(m_sdlRenderer, target->get_sdl_texture(), &target->get_texture_rect_f(), &target->get_texture_rect_f());
}

void amts::Renderer::begin() {
    assert(m_started || std::cout << "Previous renderer begin instance is still running" << std::endl);
    m_started = true;

    clear(Color(0.0f, 0.0f, 0.0f));
        
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void amts::Renderer::end() {
    assert(!m_started || std::cout << "Renderer is not started, therefore it is ending ?" << std::endl);
    m_started = false;

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());
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
