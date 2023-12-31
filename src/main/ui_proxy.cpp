#include "ui_proxy.h"

amts::UIProxy::UIProxy() 
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
    m_rayRenderingProfileEditorUIWindow = nullptr;
    m_metricsUIWindow = nullptr;
}

amts::UIProxy::~UIProxy() {
    
}

void amts::UIProxy::preinit() {
    CommonProxy::preinit();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMEPAD) != 0) {
        printf("Error: SDL_Init(): %s\n", SDL_GetError());
        throw std::runtime_error("Failed to initialize SDL");
    }

    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
}

void amts::UIProxy::init() {
    CommonProxy::init();

    m_window = std::make_unique<Window>("Amaterasu", 800, 600);
    m_renderer = std::make_unique<Renderer>(m_window);

    m_viewTexture = std::make_unique<SDLRenderingTarget>(m_renderer, 800, 600);
    m_target = std::make_unique<RenderingTarget>(800, 600);
    m_target->allocate();
    
    
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
    m_rayRenderingProfileEditorUIWindow = std::make_unique<RenderingProfileEditorUIWindow>();
    m_metricsUIWindow = std::make_unique<MetricsUIWindow>();
}

void amts::UIProxy::load() {
    CommonProxy::load();
    
    ImGuiIO& io = ImGui::GetIO();
    std::ignore = io.Fonts->AddFontFromFileTTF("assets\\fonts\\Nunito-Medium.ttf", 21);
}

void amts::UIProxy::run() {
    bool stopRenderingThread = false;

    m_rayRenderer->set_active_scene(m_scene);
    m_rayRenderer->set_active_camera(m_mainCamera);
    m_rayRenderer->set_active_material_collection(m_materialCollection);
    m_rayRenderer->set_active_sky_texture(m_activeSkyTexture);

    std::thread renderingThread([&]() {
        while (!m_window->is_open()) {
            if(!stopRenderingThread) {
                m_scene->mark_as_rendering();

                if(m_scene->is_modified() || m_rayRenderer->is_profile_changet()) {
                    m_target->reset_accumulation();
                    
                    if(!m_rayRenderer->get_profile().m_immediateAccumulation)
                        m_target->fill_pixel_data(0xFF000000);

                    m_rayRenderer->reset_accumulation();
                    m_rayRenderer->reset_changet_profile_flag();

                    m_cameraController->reset_move_flag();
                    m_scene->reset_modified_flag();
                }

                m_target->lock();
                    // m_rayRenderer->render(m_target.get());
                    m_rayRenderer->render(m_target.get(), m_workerPool);
                m_target->unlock();
            }
            
            m_scene->reset_rendering_flag();
        }
    });
    
    while (!m_window->is_open()) {
        m_eventHandler->handle_events();

        // Todo move camera should not take window, I assume all mouse input should be processed in update_input method
        stopRenderingThread = true;
        if(m_resultViewUIWindow->is_focused()) 
            m_cameraController->move_camera(m_mainCamera, m_window);

        if(m_cameraController->is_moved())
            m_scene->mark_as_modified();

        m_renderer->begin();
            m_mainDockspace->run([&]() {
                if(ImGui::BeginMenuBar()) {
                    if(ImGui::BeginMenu("File")) {
                        if(ImGui::MenuItem("Save", nullptr, nullptr)) save_rendering_target_to_png(m_target.get(), "result.png");
                        if(ImGui::MenuItem("Exit", nullptr, nullptr)) m_window->force_close();

                        ImGui::EndMenu();
                    }

                    if(ImGui::BeginMenu("View")) {
                        ImGui::MenuItem("Scene View", nullptr,      &m_sceneViewUIWindow->ref_open_flag());
                        ImGui::MenuItem("Materials", nullptr,       &m_resultViewUIWindow->ref_open_flag());
                        ImGui::MenuItem("Result View", nullptr,     &m_resultViewUIWindow->ref_open_flag());
                        ImGui::MenuItem("Metrics", nullptr,         &m_metricsUIWindow->ref_open_flag());

                        ImGui::EndMenu();
                    }

                    ImGui::EndMenuBar();
                }

                m_viewTexture->lock();
                    m_viewTexture->copy_pixel_data(*m_target);
                m_viewTexture->unlock();

                m_resultViewUIWindow->run(m_viewTexture);
                m_sceneViewUIWindow->run(m_scene, m_materialCollection);
                m_materialsUIWindow->run(m_materialCollection, m_scene);
                m_rayRenderingProfileEditorUIWindow->run(m_scene, m_rayRenderer);
                m_metricsUIWindow->run();
            });
        m_renderer->end();
        
        m_renderer->present();
        stopRenderingThread = false;
    }

    renderingThread.join();
}

void amts::UIProxy::unload() {
    CommonProxy::unload();
}

void amts::UIProxy::cleanup() {
    CommonProxy::cleanup();

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    m_window = nullptr;
    m_renderer = nullptr;
    
    m_viewTexture = nullptr;
    m_target = nullptr;

    m_eventHandler = nullptr;

    m_cameraController = nullptr;
    
    m_mainDockspace = nullptr;
    m_sceneViewUIWindow = nullptr;
    m_materialsUIWindow = nullptr;
    m_resultViewUIWindow = nullptr;
    m_rayRenderingProfileEditorUIWindow = nullptr;
    m_metricsUIWindow = nullptr;

    SDL_Quit();
}
