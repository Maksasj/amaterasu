#ifndef _UI_PROXY_H_
#define _UI_PROXY_H_

#include "renderer.h"
#include "rendering_target.h"
#include "camera_controller.h"

#include "common/common_proxy.h"

#include "ui/scene_view_ui_window.h"
#include "ui/materials_ui_window.h"
#include "ui/main_dockspace.h"

namespace amts {
    class UIProxy : public CommonProxy {
        private:
            bool m_close;

            std::unique_ptr<Window> m_window;
            std::unique_ptr<Renderer> m_renderer;
            std::unique_ptr<RenderingTarget> m_target;

            // Todo I think camera controller should be stored somewhere else
            std::unique_ptr<CameraController> m_cameraController;

            std::unique_ptr<MainDockspaceUIWindow> m_mainDockspace;
            std::unique_ptr<SceneViewUIWindow> m_sceneViewUIWindow;
            std::unique_ptr<MaterialsUIWindow> m_materialsUIWindow;

        public:
            UIProxy() 
                : CommonProxy(), 
                  m_close(false),
                  m_window(nullptr),
                  m_renderer(nullptr),
                  m_target(nullptr),
                  m_cameraController(nullptr) { 
                
                // For convinience lets initlize ui windows with nulls there
                m_mainDockspace = nullptr;
                m_sceneViewUIWindow = nullptr;
                m_materialsUIWindow = nullptr;
            }

            ~UIProxy() override {
                
            }

            void preinit() override {
                CommonProxy::preinit();

                SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMEPAD);
                SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
            }

            void init() override {
                CommonProxy::init();

                m_window = std::make_unique<Window>("Amaterasu", 800, 600);
                m_renderer = std::make_unique<Renderer>(m_window);
                m_target = std::make_unique<RenderingTarget>(m_renderer, 800, 600);
                m_cameraController = std::make_unique<CameraController>();

                // All imgui thing should be under renderer or imgui renderer classes
                IMGUI_CHECKVERSION();
                ImGui::CreateContext();
                ImGuiIO& io = ImGui::GetIO();
                io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
                io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

                // Setup Dear ImGui style
                ImGui::StyleColorsDark();
                //ImGui::StyleColorsLight();

                // Setup Platform/Renderer backends
                ImGui_ImplSDL3_InitForSDLRenderer(m_window->get_sdl_window(), m_renderer->get_sdl_renderer());
                ImGui_ImplSDLRenderer3_Init(m_renderer->get_sdl_renderer());

                m_mainDockspace = std::make_unique<MainDockspaceUIWindow>();
                m_sceneViewUIWindow = std::make_unique<SceneViewUIWindow>();
                m_materialsUIWindow = std::make_unique<MaterialsUIWindow>();
            }

            void load() override {
                CommonProxy::load();
                
                ImGuiIO& io = ImGui::GetIO();
                std::ignore = io.Fonts->AddFontFromFileTTF("assets\\fonts\\Nunito-Medium.ttf", 21);
            }

            void run() override {
                while (!m_close) {

                    // Todo this should be moved to some sort of event handler class
                    // Including camera controller, maybe lets create a new base class, as EventReceiver
                    // And EventHandler will have a list of all event receivers
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

                        m_cameraController->update_input(event);
                    }

                    // Todo move camera should not take window, I assume all mouse input should be processed in update_input method
                    m_cameraController->move_camera(m_mainCamera, m_window);

                    if(m_cameraController->is_moved()) {
                        m_target->reset_accumulation();
                        m_rayRenderer->reset_accumulation();

                        m_cameraController->reset_move_flag();
                    }
                    
                    m_renderer->clear(Color(0.0f, 0.0f, 0.0f));
                    
                    ImGui_ImplSDLRenderer3_NewFrame();
                    ImGui_ImplSDL3_NewFrame();
                    ImGui::NewFrame();

                    // Render image
                    m_target->lock();
                    m_rayRenderer->render(m_target, m_scene, m_mainCamera, m_materialPool);
                    m_target->unlock();

                    m_mainDockspace->run([&]() {
                        if(ImGui::Begin("Result")) {
                            auto imageSize = ImVec2(m_target->get_width(), m_target->get_height());
                            auto centerPosition = ImVec2((ImGui::GetWindowSize().x - imageSize.x) * 0.5f, (ImGui::GetWindowSize().y - imageSize.y) * 0.5f);

                            ImGui::SetCursorPos(centerPosition);
                            ImGui::Image((void*)m_target->get_sdl_texture(), imageSize);

                            ImGui::End();
                        }

                        m_sceneViewUIWindow->run(m_scene);
                        m_materialsUIWindow->run(m_materialPool);
                        
                        if(ImGui::Begin("Renderer")) {
                            ImGuiIO& io = ImGui::GetIO(); (void)io;

                            ImGui::Text("Dear ImGui %s", ImGui::GetVersion());
                            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                            ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
                            ImGui::Text("%d visible windows, %d active allocations", io.MetricsRenderWindows, io.MetricsActiveAllocations);

                            ImGui::End();
                        }
                    });

                    ImGui::Render();
                    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());

                    m_renderer->present();
                }
            }

            void unload() override {
                CommonProxy::unload();
            }

            void cleanup() override {
                CommonProxy::cleanup();

                ImGui_ImplSDLRenderer3_Shutdown();
                ImGui_ImplSDL3_Shutdown();
                ImGui::DestroyContext();

                m_window = nullptr;
                m_renderer = nullptr;
                m_target = nullptr;
                m_cameraController = nullptr;
                
                m_mainDockspace = nullptr;
                m_sceneViewUIWindow = nullptr;
                m_materialsUIWindow = nullptr;

                SDL_Quit();
            }
    };
}

#endif