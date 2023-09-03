#ifndef _COMMON_PROXY_H_
#define _COMMON_PROXY_H_

#include "common.h"

#include "renderer/ray_renderer.h"
#include "materials/material_collection.h"
#include "scene/scene.h"
#include "camera.h"

namespace amts {
    class CommonProxy {
        protected:
            std::unique_ptr<RayRenderer> m_rayRenderer;
            std::unique_ptr<Scene> m_scene;
            std::unique_ptr<Camera> m_mainCamera;
            std::unique_ptr<MaterialCollection> m_materialPool;

        public:
            CommonProxy();

            virtual ~CommonProxy();

            virtual void preinit();

            virtual void init();

            virtual void load();

            virtual void run();

            virtual void unload();

            virtual void cleanup();

            void save_rendering_target_to_png(RenderingTarget* target, const std::string& fileName);
    };
}

#endif