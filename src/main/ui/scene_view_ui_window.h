#ifndef _SCENE_VIEW_UI_WINDOW_H_
#define _SCENE_VIEW_UI_WINDOW_H_

#include "common/common_ui_window.h"
#include "scene/scene.h"

namespace amts {
    class SceneViewUIWindow : public CommonUIWindow {
        private:

        public:
            SceneViewUIWindow() : CommonUIWindow("Scene View") {

            }

            ~SceneViewUIWindow() override {
                
            }

            void run(std::unique_ptr<Scene>& scene) {
                if(!CommonUIWindow::start_window()) return;

                std::ignore = scene;

                CommonUIWindow::end_window();
            }
    };
}

#endif