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
#include "ray_renderer_properties.h"

namespace amts {
    class RenderingTarget;

    class RayRenderer {
        private:
            u64 m_frame;

            RayRendererProperties m_properties;

            std::vector<u64> m_targetWidthIterator;
            std::vector<u64> m_targetHeightIterator;

            TextureBuffer<u32>* m_skyTexture;

        public:
            RayRenderer();
            ~RayRenderer();

            RayResult trace_ray(const Ray& ray, const Scene& scene) const;
            Color per_pixel(const u64& x, const u64& y, const u64& width, const u64& height, const Scene& scene, const Camera& camera, const MaterialCollection& materialPool) const;

            void render(
                RenderingTarget* target,
                const std::unique_ptr<Scene>& scene,
                const std::unique_ptr<Camera>& camera,
                const std::unique_ptr<MaterialCollection>& materialPool
            );

            void set_active_sky_texture(std::unique_ptr<TextureBuffer<u32>>& skyTexture);

            void reset_accumulation();
    };
}

#endif