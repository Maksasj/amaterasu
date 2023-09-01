#ifndef _COMMON_PROXY_H_
#define _COMMON_PROXY_H_

#include "common.h"

#include "renderer/ray_renderer.h"
#include "materials/material_pool.h"
#include "scene/scene.h"
#include "camera.h"

namespace amts {
    class CommonProxy {
        protected:
            std::unique_ptr<RayRenderer> m_rayRenderer;
            std::unique_ptr<Scene> m_scene;
            std::unique_ptr<Camera> m_mainCamera;
            std::unique_ptr<MaterialPool> m_materialPool;

        public:
            CommonProxy() 
                : m_rayRenderer(nullptr),
                  m_scene(nullptr),
                  m_mainCamera(nullptr),
                  m_materialPool(nullptr) { 

            }

            virtual ~CommonProxy() {

            }

            virtual void preinit() {

            }

            virtual void init() {
                m_rayRenderer = std::make_unique<RayRenderer>();
                m_scene = std::make_unique<Scene>();
                m_mainCamera = std::make_unique<Camera>();
                m_materialPool = std::make_unique<MaterialPool>();
            }

            virtual void load() {
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

            virtual void run() {

            }

            virtual void unload() {

            }

            virtual void cleanup() {
                m_rayRenderer = nullptr;
                m_scene = nullptr;
                m_mainCamera = nullptr;
                m_materialPool = nullptr;
            }
    };
}

#endif