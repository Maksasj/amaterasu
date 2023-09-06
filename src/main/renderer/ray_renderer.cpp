#include "ray_renderer.h"
#include "rendering_target.h"

amts::RayRenderer::RayRenderer::RayRenderer() 
    : m_frame(1),
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
}

amts::RayRenderer::RayRenderer::~RayRenderer() {

}

amts::RayResult amts::RayRenderer::trace_ray(const Ray& ray) const {
    f32 depth = std::numeric_limits<f32>::max();
    RayResult rayResult = RayResult::invalid;
    bool validCollision = false;

    for(u64 i = 0; i < m_activeScene->m_objects.size(); ++i) {
        const RayResult result = m_activeScene->m_objects[i]->hit(ray);

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

    Vec4f rotated = m_activeCamera->get_rotation_matrix() * Vec4f(rayDirection.x, rayDirection.y, rayDirection.z, 1.0);
    rayDirection = Vec3f(rotated.x, rotated.y, rotated.z);

    Ray ray{m_activeCamera->get_position(), rayDirection};

    Vec3f light = Vec3f::splat(0.0f);
    Vec3f contribution = Vec3f::splat(1.0f);

    const u64 maxBounces = 10;
    for(u64 b = 0; b < maxBounces; ++b) {
        const auto result = trace_ray(ray);

        if(result.hitDistance < 0.0f) {
            if(m_activeSkyTexture == nullptr) {
                const Vec3f skyColor = Vec3f(1.0f, 1.0f, 1.0f);
                light += skyColor * contribution;
                break;
            }

            const auto& d = ray.m_direction;
            const Vec2f uv = Vec2f(atan2(d.z, d.x) / (2.0f * OMNI_TYPES_PI), asinf(d.y) / OMNI_TYPES_PI) + 0.5f;

            const u64 x = static_cast<u64>(uv.x * static_cast<f32>(m_activeSkyTexture->get_width() - 1));
            const u64 y = static_cast<u64>(uv.y * static_cast<f32>(m_activeSkyTexture->get_height() - 1));

            const u32& pixel = m_activeSkyTexture->get_pixel_at(x, y);
            const Vec3f skyColor = Color::from_vec4f(pixel).to_vec3f(); // Todo, cringe conversions

            light += skyColor * contribution;

            break;
        }

        const std::unique_ptr<Object>& object = m_activeScene->m_objects[result.objectId];
        const std::unique_ptr<Material>& material = m_activeMaterialCollection->m_materials[object->m_materialId];  

        contribution *= material->m_albedo.to_vec3f();
        light += material->m_emissionColor.to_vec3f() * material->m_emissionStrength;

        ray.m_origin = result.hitPoint + result.hitNormal * 0.0001f;

        const Vec3f reflection = ray.m_direction - result.hitNormal * 2.0f * ray.m_direction.dot(result.hitNormal); 
        ray.m_direction = (reflection + Utils::random_in_unit_sphere() * material->m_metallic).normalize();
    }

    return Color(light.x, light.y, light.z);
}

void amts::RayRenderer::render(RenderingTarget* target) {
    assert(target->is_locked() || std::cout << "Target should be locked before trying to render" << std::endl);

    assert((m_activeScene == nullptr) || std::cout << "Active scene is not specified before rendering" << std::endl);
    assert((m_activeCamera == nullptr) || std::cout << "Active camera is not specified before rendering" << std::endl);
    assert((m_activeMaterialCollection == nullptr) || std::cout << "Active material collection is not specified before rendering" << std::endl);
    assert((m_activeSkyTexture == nullptr) || std::cout << "Active sky texture collection is not specified before rendering" << std::endl);

    const auto targetWidth = target->get_width();
    const auto targetHeight = target->get_height();

    TextureBuffer<Color>& accumulatedColor = target->get_accumulated_color();

    std::for_each(std::execution::par, m_targetWidthIterator.begin(), m_targetWidthIterator.end(), [&](const u64& x) {
        std::for_each(std::execution::par, m_targetHeightIterator.begin(), m_targetHeightIterator.end(), [&](const u64& y) {
            const Color color = per_pixel(x, y, targetWidth, targetHeight).clamp(0.0f, 1.0f);

            Color& accumulatedPixel = accumulatedColor.get_pixel_at(x, y);
            accumulatedPixel += color;

            target->get_pixel_at(x, y) = (accumulatedPixel / static_cast<f32>(m_frame)).to_u32();
        });
    });

    /*
    for(u64 x = 0; x < targetWidth; ++x) {
        for(u64 y = 0; y < targetHeight; ++y) {
            const Color color = per_pixel(x, y, targetWidth, targetHeight, *scene, *camera, *materialCollection).clamp(0.0f, 1.0f);

            Color& accumulatedPixel = accumulatedColor.get_pixel_at(x, y);
            accumulatedPixel += color;

            target->get_pixel_at(x, y) = (accumulatedPixel / static_cast<f32>(m_frame)).to_u32();
        }
    }
    */

    ++m_frame;
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
    m_frame = 1;
}

