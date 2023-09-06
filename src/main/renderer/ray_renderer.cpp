#include "ray_renderer.h"
#include "rendering_target.h"

amts::RayRenderer::RayRenderer::RayRenderer() 
    : m_done(false), 
    m_accumulatedSamples(1),
    m_properties(RayRendererProfile::create_default_renderer_profile()),
    m_activeScene(nullptr),
    m_activeCamera(nullptr),
    m_activeMaterialCollection(nullptr),
    m_activeSkyTexture(nullptr) {

    m_targetWidthIterator.resize(800);
    m_targetHeightIterator.resize(600);

    for(u64 i = 0; i < 800; ++i) 
        m_targetWidthIterator[i] = i;

    for(u64 i = 0; i < 600; ++i) 
        m_targetHeightIterator[i] = i;
        
    m_targetTiledWidthIterator.resize(5);
    m_targetTiledHeightIterator.resize(5);

    for(u64 i = 0; i < 5; ++i) 
        m_targetTiledWidthIterator[i] = i;

    for(u64 i = 0; i < 5; ++i) 
        m_targetTiledHeightIterator[i] = i;
}

amts::RayRenderer::RayRenderer::~RayRenderer() {

}

amts::RayResult amts::RayRenderer::trace_ray(const Ray& ray) const {
    f32 depth = std::numeric_limits<f32>::max();
    RayResult rayResult = RayResult::invalid;
    bool validCollision = false;

    for(u64 i = 0; i < m_activeScene->m_objects.size(); ++i) {
        const RayResult result = m_activeScene->m_objects[i]->hit(m_properties, ray);

        if(!result.is_valid())
            continue;

        if(result.hitDistance > depth)
            continue;

        depth = result.hitDistance;
        rayResult = result;
        rayResult.objectId = i;
        validCollision = true;
    }

    if(validCollision)
        return rayResult;

    return RayResult::invalid;
}

amts::Color amts::RayRenderer::per_pixel(const u64& x, const u64& y, const u64& width, const u64& height) const {
    const f32 aspectRatio = static_cast<f32>(width) / static_cast<f32>(height);

    Vec3f rayDirection = Vec3f(
        (x / static_cast<f32>(width) - 0.5f),
        (y / static_cast<f32>(height) - 0.5f) / aspectRatio, 
        -1.0f);

    if(m_properties.m_enableAntiAliasing) {
        rayDirection.x += (Utils::random_float() - 0.5f) * m_properties.m_antiAliasingFactor;
        rayDirection.y += (Utils::random_float() - 0.5f) * m_properties.m_antiAliasingFactor;
    }

    Vec4f rotated = m_activeCamera->get_rotation_matrix() * Vec4f(rayDirection.x, rayDirection.y, rayDirection.z, 1.0f);
    rayDirection = Vec3f(rotated.x, rotated.y, rotated.z);

    Ray ray{m_activeCamera->get_position(), rayDirection};

    Vec3f light = Vec3f::splat(0.0f);
    Vec3f contribution = Vec3f::splat(1.0f);

    for(u64 b = 0; b < m_properties.m_maxBounces; ++b) {
        const auto result = trace_ray(ray);

        if(result.hitDistance < 0.0f) {
            if(m_properties.m_enableSkybox) {
                const auto& d = ray.m_direction;
                const Vec2f uv = Vec2f(atan2(d.z, d.x) / (2.0f * OMNI_TYPES_PI), asinf(d.y) / OMNI_TYPES_PI) + 0.5f;

                const u64 x = static_cast<u64>(uv.x * static_cast<f32>(m_activeSkyTexture->get_width() - 1));
                const u64 y = static_cast<u64>(uv.y * static_cast<f32>(m_activeSkyTexture->get_height() - 1));

                const u32& pixel = m_activeSkyTexture->get_pixel_at(x, y);
                const Vec3f skyColor = Color::from_vec4f(pixel).to_vec3f(); // Todo, cringe conversions

                light += skyColor * contribution;

                break;
            }

            const Vec3f skyColor = Vec3f(1.0f, 1.0f, 1.0f);
            light += skyColor * contribution;
            break;
        }

        const std::unique_ptr<Object>& object = m_activeScene->m_objects[result.objectId];
        const std::unique_ptr<Material>& material = m_activeMaterialCollection->m_materials[object->m_materialId];  

        contribution *= material->m_albedo.to_vec3f();

        if(m_properties.m_enableEmission)
            light += material->m_emissionColor.to_vec3f() * material->m_emissionStrength;

        if(m_properties.m_enableReflection) {
            ray.m_origin = result.hitPoint + result.hitNormal * m_properties.m_reflectionNormalOffset;

            const Vec3f reflection = ray.m_direction - result.hitNormal * 2.0f * ray.m_direction.dot(result.hitNormal); 
            ray.m_direction = (reflection + Utils::random_in_unit_sphere() * material->m_metallic).normalize();
            
            continue;
        }

        break;
    }

    return Color(light.x, light.y, light.z);
}

void amts::RayRenderer::render(RenderingTarget* target) {
    if(m_done) // if we done rendering then lets stop
        return;

    assert(target->is_locked() || std::cout << "Target should be locked before trying to render" << std::endl);

    assert((m_activeScene == nullptr) || std::cout << "Active scene is not specified before rendering" << std::endl);
    assert((m_activeCamera == nullptr) || std::cout << "Active camera is not specified before rendering" << std::endl);
    assert((m_activeMaterialCollection == nullptr) || std::cout << "Active material collection is not specified before rendering" << std::endl);
    assert((m_activeSkyTexture == nullptr) || std::cout << "Active sky texture collection is not specified before rendering" << std::endl);

    const auto targetWidth = target->get_width();
    const auto targetHeight = target->get_height();

    TextureBuffer<Color>& accumulatedColor = target->get_accumulated_color();

    if(m_properties.m_presentMode == IMMEDIATE_ACCUMULATION) {
        if(m_properties.m_multiThreading) {
            std::for_each(std::execution::par, m_targetWidthIterator.begin(), m_targetWidthIterator.end(), [&](const u64& x) {
                std::for_each(std::execution::par, m_targetHeightIterator.begin(), m_targetHeightIterator.end(), [&](const u64& y) {
                    const Color color = per_pixel(x, y, targetWidth, targetHeight).clamp(0.0f, 1.0f);

                    Color& accumulatedPixel = accumulatedColor.get_pixel_at(x, y);
                    accumulatedPixel += color;

                    target->get_pixel_at(x, y) = (accumulatedPixel / static_cast<f32>(m_accumulatedSamples)).to_u32();
                });
            });
        } else {
            for(u64 x = 0; x < targetWidth; ++x) {
                for(u64 y = 0; y < targetHeight; ++y) {
                    const Color color = per_pixel(x, y, targetWidth, targetHeight).clamp(0.0f, 1.0f);

                    Color& accumulatedPixel = accumulatedColor.get_pixel_at(x, y);
                    accumulatedPixel += color;

                    target->get_pixel_at(x, y) = (accumulatedPixel / static_cast<f32>(m_accumulatedSamples)).to_u32();
                }
            }
        }
    } else if(m_properties.m_presentMode == TILED_ACCUMULATION) {
        const u64 tileWidth = targetWidth / m_properties.m_tilesInRow;
        const u64 tileHeight = targetHeight / m_properties.m_tilesInCollum;
        
        if(m_properties.m_multiThreading) {
            std::for_each(std::execution::par, m_targetTiledWidthIterator.begin(), m_targetTiledWidthIterator.end(), [&](const u64& x) {
                std::for_each(std::execution::par, m_targetTiledHeightIterator.begin(), m_targetTiledHeightIterator.end(), [&](const u64& y) {
                    if(m_properties.m_immediateAccumulation) {
                        for(u64 xt = x * tileWidth; xt < (x + 1) * tileWidth; ++xt) {
                            for(u64 yt = y * tileHeight; yt < (y + 1) * tileHeight; ++yt) {
                                const Color color = per_pixel(xt, yt, targetWidth, targetHeight).clamp(0.0f, 1.0f);

                                Color& accumulatedPixel = accumulatedColor.get_pixel_at(xt, yt);
                                accumulatedPixel += color;

                                target->get_pixel_at(xt, yt) = (accumulatedPixel / static_cast<f32>(m_accumulatedSamples)).to_u32();
                            }
                        }
                    } else {
                        for(u64 i = 0; i < m_properties.m_samplesPerPixel; ++i) {
                            for(u64 xt = x * tileWidth; xt < (x + 1) * tileWidth; ++xt) {
                                for(u64 yt = y * tileHeight; yt < (y + 1) * tileHeight; ++yt) {
                                    const Color color = per_pixel(xt, yt, targetWidth, targetHeight).clamp(0.0f, 1.0f);

                                    Color& accumulatedPixel = accumulatedColor.get_pixel_at(xt, yt);
                                    accumulatedPixel += color;

                                    target->get_pixel_at(xt, yt) = (accumulatedPixel / static_cast<f32>(m_properties.m_samplesPerPixel)).to_u32();
                                }
                            }
                        }
                    }
                });
            });

            m_done = !m_properties.m_immediateAccumulation;
        } else {
            for(u64 x = 0; x < m_properties.m_tilesInRow; ++x) {
                for(u64 y = 0; y < m_properties.m_tilesInCollum; ++y) {
                    for(u64 xt = x * tileWidth; xt < (x + 1) * tileWidth; ++xt) {
                        for(u64 yt = y * tileHeight; yt < (y + 1) * tileHeight; ++yt) {
                            const Color color = per_pixel(xt, yt, targetWidth, targetHeight).clamp(0.0f, 1.0f);

                            Color& accumulatedPixel = accumulatedColor.get_pixel_at(xt, yt);
                            accumulatedPixel += color;

                            target->get_pixel_at(xt, yt) = (accumulatedPixel / static_cast<f32>(m_accumulatedSamples)).to_u32();
                        }
                    }
                }
            }
        }
    }

    ++m_accumulatedSamples;
}

void amts::RayRenderer::set_active_sky_texture(std::unique_ptr<TextureBuffer<u32>>& skyTexture) {
    m_activeSkyTexture = skyTexture.get();
}

void amts::RayRenderer::set_active_scene(std::unique_ptr<Scene>& scene) {
    m_activeScene = scene.get();
}

void amts::RayRenderer::set_active_material_collection(std::unique_ptr<MaterialCollection>& materialCollection) {
    m_activeMaterialCollection = materialCollection.get();
}

void amts::RayRenderer::set_active_camera(std::unique_ptr<Camera>& camera) {
    m_activeCamera = camera.get();
}

void amts::RayRenderer::reset_accumulation() {
    m_accumulatedSamples = 1;
}

