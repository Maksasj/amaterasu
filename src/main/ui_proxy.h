#ifndef _UI_PROXY_H_
#define _UI_PROXY_H_

#include "renderer.h"
#include "rendering_target.h"
#include "camera_controller.h"
#include "event_handler.h"
#include "imgui_event_receiver.h"

#include "common/common_proxy.h"

#include "ui/scene_view_ui_window.h"
#include "ui/materials_ui_window.h"
#include "ui/main_dockspace.h"
#include "ui/result_view_ui_window.h"
#include "ui/metrics_ui_window.h"

namespace amts {
    class UIProxy : public CommonProxy {
        private:
            std::unique_ptr<Window> m_window;
            std::unique_ptr<Renderer> m_renderer;
            std::unique_ptr<RenderingTarget> m_target;

            std::unique_ptr<EventHandler> m_eventHandler;

            // Todo I think camera controller should be stored somewhere else
            std::unique_ptr<CameraController> m_cameraController;
            std::unique_ptr<ImGuiEventReceiver> m_imguiEventReceiver;

            std::unique_ptr<MainDockspaceUIWindow> m_mainDockspace;
            std::unique_ptr<SceneViewUIWindow> m_sceneViewUIWindow;
            std::unique_ptr<MaterialsUIWindow> m_materialsUIWindow;
            std::unique_ptr<ResultViewUIWindow> m_resultViewUIWindow;
            std::unique_ptr<MetricsUIWindow> m_metricsUIWindow;

        public:
            UIProxy() 
                : CommonProxy(), 
                  m_window(nullptr),
                  m_renderer(nullptr),
                  m_target(nullptr),
                  m_eventHandler(nullptr) { 
                
                m_cameraController = nullptr;
                m_imguiEventReceiver = nullptr;

                // For convinience lets initlize ui windows with nulls there
                m_mainDockspace = nullptr;
                m_sceneViewUIWindow = nullptr;
                m_materialsUIWindow = nullptr;
                m_resultViewUIWindow = nullptr;
                m_metricsUIWindow = nullptr;
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
                m_eventHandler = std::make_unique<EventHandler>();

                m_cameraController = std::make_unique<CameraController>();
                m_imguiEventReceiver = std::make_unique<ImGuiEventReceiver>();

                m_eventHandler->bind_event_receiver(m_window.get());
                m_eventHandler->bind_event_receiver(m_cameraController.get());
                m_eventHandler->bind_event_receiver(m_imguiEventReceiver.get());

                m_mainDockspace = std::make_unique<MainDockspaceUIWindow>();
                m_sceneViewUIWindow = std::make_unique<SceneViewUIWindow>();
                m_materialsUIWindow = std::make_unique<MaterialsUIWindow>();
                m_resultViewUIWindow = std::make_unique<ResultViewUIWindow>();
                m_metricsUIWindow = std::make_unique<MetricsUIWindow>();
            }

            void load() override {
                CommonProxy::load();
                
                ImGuiIO& io = ImGui::GetIO();
                std::ignore = io.Fonts->AddFontFromFileTTF("assets\\fonts\\Nunito-Medium.ttf", 21);
            }

            void run() override {
                while (!m_window->is_open()) {
                    m_eventHandler->handle_events();

                    // Todo move camera should not take window, I assume all mouse input should be processed in update_input method
                    m_cameraController->move_camera(m_mainCamera, m_window);

                    if(m_cameraController->is_moved()) {
                        m_target->reset_accumulation();
                        m_rayRenderer->reset_accumulation();

                        m_cameraController->reset_move_flag();
                    }
                    
                    m_renderer->begin();
                        m_target->lock();
                            m_rayRenderer->render(m_target, m_scene, m_mainCamera, m_materialPool);
                        m_target->unlock();

                        m_mainDockspace->run([&]() {
                            m_resultViewUIWindow->run(m_target);
                            m_sceneViewUIWindow->run(m_scene);
                            m_materialsUIWindow->run(m_materialPool);
                            m_metricsUIWindow->run();
                        });
                    m_renderer->end();
                    
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
                m_eventHandler = nullptr;

                m_cameraController = nullptr;
                
                m_mainDockspace = nullptr;
                m_sceneViewUIWindow = nullptr;
                m_materialsUIWindow = nullptr;
                m_resultViewUIWindow = nullptr;
                m_metricsUIWindow = nullptr;

                SDL_Quit();
            }
    };
}

#endif