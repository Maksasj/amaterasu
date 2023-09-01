#ifndef _AMATERASU_H_
#define _AMATERASU_H_

#include <memory>

#include "common.h"
#include "renderer.h"
#include "window.h"
#include "rendering_target.h"
#include "material_pool.h"
#include "ray_renderer.h"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

namespace amts {
    class Amaterasu {
        private:
            bool m_close;

            std::unique_ptr<Window> m_window;
            
            std::unique_ptr<Renderer> m_renderer;
            std::unique_ptr<RayRenderer> m_rayRenderer;

            std::unique_ptr<RenderingTarget> m_target;
            std::unique_ptr<Scene> m_scene;
            std::unique_ptr<Camera> m_mainCamera;
            std::unique_ptr<MaterialPool> m_materialPool;

        public:
            Amaterasu() 
                : m_close(false),
                  m_window(nullptr),
                  m_renderer(nullptr),
                  m_rayRenderer(nullptr),
                  m_target(nullptr),
                  m_scene(nullptr),
                  m_mainCamera(nullptr),
                  m_materialPool(nullptr) {

            }

            ~Amaterasu() {

            }

            void preinit() {
                SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMEPAD);
                SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
            }

            void init() {
                m_window = std::make_unique<Window>("Amaterasu", 800, 600);

                m_renderer = std::make_unique<Renderer>(m_window);
                m_rayRenderer = std::make_unique<RayRenderer>();

                m_target = std::make_unique<RenderingTarget>(m_renderer, 800, 600);
                
                m_scene = std::make_unique<Scene>();
                m_materialPool = std::make_unique<MaterialPool>();

                m_mainCamera = std::make_unique<Camera>();


                IMGUI_CHECKVERSION();
                ImGui::CreateContext();
                ImGuiIO& io = ImGui::GetIO(); (void)io;
                io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
                io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

                // Setup Dear ImGui style
                ImGui::StyleColorsDark();
                //ImGui::StyleColorsLight();

                // Setup Platform/Renderer backends
                ImGui_ImplSDL3_InitForSDLRenderer(m_window->get_sdl_window(), m_renderer->get_sdl_renderer());
                ImGui_ImplSDLRenderer3_Init(m_renderer->get_sdl_renderer());
            }
            
            void load() {
                m_materialPool->m_materials.emplace_back(std::make_unique<Material>(
                    "Red metallic",
                    Color(1.0f, 0.0f, 0.0f),
                    Color(0.0f, 0.0f, 0.0f),
                    0.0f,
                    0.1f
                ));

                m_materialPool->m_materials.emplace_back(std::make_unique<Material>(
                    "Blue diffuse",
                    Color(0.0f, 0.0f, 1.0f),
                    Color(0.0f, 0.0f, 0.0f),
                    0.0f,
                    1.0f
                ));

                m_materialPool->m_materials.emplace_back(std::make_unique<Material>(
                    "Sun",
                    Color(0.99f, 0.32f, 0.0f),
                    Color(0.99f, 0.32f, 0.0f),
                    1.0f,
                    0.8f
                ));

                m_materialPool->m_materials.emplace_back(std::make_unique<Material>(
                    "Gray floor",
                    Color(0.3f, 0.3f, 0.3f),
                    Color(0.0f, 0.0f, 0.0f),
                    0.0f,
                    1.0f
                ));

                m_scene->m_objects.emplace_back(std::make_unique<SphereObject>(Vec3f(0.0f, 0.0f, -2.5f),  0.75f, 0));
                m_scene->m_objects.emplace_back(std::make_unique<SphereObject>(Vec3f(1.0f, 0.0f, -2.0f),  0.32f, 1));
                m_scene->m_objects.emplace_back(std::make_unique<SphereObject>(Vec3f(-1.0f, 0.0f, -2.5f), 0.32f, 2));
                m_scene->m_objects.emplace_back(std::make_unique<PlaneObject>(Vec3f(0.0f, -0.5f, 0.0f), 3));
            }

            void run() {
                
                while (!m_close) {
                    SDL_Event event;

                    while (SDL_PollEvent(&event)) {
                        ImGui_ImplSDL3_ProcessEvent(&event);

                        switch (event.type) {
                            case SDL_EVENT_QUIT:
                                m_close = true;
                                break;
                            default:
                                break;
                        }

                        m_mainCamera->update_input(event);
                    }

                    if(m_mainCamera->update_state(m_window)) {
                        m_target->reset_accumulation();
                        m_rayRenderer->reset_accumulation();
                    }
                    
                    m_renderer->clear(Color(0.0f, 0.0f, 0.0f));
                    
                    ImGui_ImplSDLRenderer3_NewFrame();
                    ImGui_ImplSDL3_NewFrame();
                    ImGui::NewFrame();

                    static bool open = true;

                    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

                    const ImGuiViewport* viewport = ImGui::GetMainViewport();
                    ImGui::SetNextWindowPos(viewport->WorkPos);
                    ImGui::SetNextWindowSize(viewport->WorkSize);
                    ImGui::SetNextWindowViewport(viewport->ID);
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
                    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
                    
                    ImGui::Begin("DockSpace Demo", &open, window_flags);

                    ImGui::PopStyleVar();
                    ImGui::PopStyleVar();
                    ImGui::PopStyleVar();

                    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

                    m_target->lock();
                        m_rayRenderer->render(m_target, m_scene, m_mainCamera, m_materialPool);
                    m_target->unlock();

                    if(ImGui::Begin("Result")) {
                        auto imageSize = ImVec2(m_target->get_width(), m_target->get_height());
                        auto centerPosition = ImVec2((ImGui::GetWindowSize().x - imageSize.x) * 0.5f, (ImGui::GetWindowSize().y - imageSize.y) * 0.5f);

                        ImGui::SetCursorPos(centerPosition);
                        ImGui::Image((void*)m_target->get_sdl_texture(), imageSize);

                        ImGui::End();
                    }

                    if(ImGui::Begin("Scene")) {

                        ImGui::End();
                    }

                    if(ImGui::Begin("Materials")) {

                        ImGui::End();
                    }
                    
                    if(ImGui::Begin("Renderer")) {
                        ImGuiIO& io = ImGui::GetIO(); (void)io;

                        ImGui::Text("Dear ImGui %s", ImGui::GetVersion());
                        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                        ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
                        ImGui::Text("%d visible windows, %d active allocations", io.MetricsRenderWindows, io.MetricsActiveAllocations);

                        ImGui::End();
                    }

                    ImGui::End();

                    // ImGui::ShowDemoWindow();

                    ImGui::Render();
                    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());

                    m_renderer->present();
                }
            }

            void unload() {

            }

            void cleanup() {
                ImGui_ImplSDLRenderer3_Shutdown();
                ImGui_ImplSDL3_Shutdown();
                ImGui::DestroyContext();

                m_target = nullptr;
                m_renderer = nullptr;
                m_window = nullptr;

                m_scene = nullptr;
                m_materialPool = nullptr;

                m_mainCamera = nullptr;

                SDL_Quit();
            }
    };
}

#endif