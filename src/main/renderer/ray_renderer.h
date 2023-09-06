#ifndef _RAY_RENDERER_H_
#define _RAY_RENDERER_H_

#include <cassert>
#include <memory>
#include <thread>
#include <algorithm>
#include <execution>

#include "common/common.h"
#include "window.h"
#include "ray.h"
#include "ray_result.h"
#include "scene/scene.h"
#include "camera.h"
#include "utils.h"
#include "materials/material_collection.h"
#include "rendering_target.h"
#include "ray_renderer_profile.h"

namespace amts {
    class RenderingTarget;

    class RayRenderer {
        private:
            u64 m_accumulatedSamples;

            RayRendererProfile m_properties;

            std::vector<u64> m_targetWidthIterator;
            std::vector<u64> m_targetHeightIterator;

            Scene* m_activeScene;
            Camera* m_activeCamera;
            MaterialCollection* m_activeMaterialCollection;
            TextureBuffer<u32>* m_activeSkyTexture;

        public:
            RayRenderer();
            ~RayRenderer();

            RayResult trace_ray(const Ray& ray) const;
            Color per_pixel(const u64& x, const u64& y, const u64& width, const u64& height) const;

            void render(RenderingTarget* target);

            void set_active_scene(std::unique_ptr<Scene>& scene);
            void set_active_camera(std::unique_ptr<Camera>& camera);
            void set_active_material_collection(std::unique_ptr<MaterialCollection>& materialCollection);
            void set_active_sky_texture(std::unique_ptr<TextureBuffer<u32>>& skyTexture);

            void reset_accumulation();
    };
}

#endif