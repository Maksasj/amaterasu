#ifndef _UI_PROXY_H_
#define _UI_PROXY_H_

#include "renderer.h"
#include "sdl_rendering_target.h"
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
            std::unique_ptr<SDLRenderingTarget> m_target;

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
            UIProxy();

            ~UIProxy() override;

            void preinit() override ;

            void init() override;

            void load() override;

            void run() override;

            void unload() override;

            void cleanup() override;
    };
}

#endif