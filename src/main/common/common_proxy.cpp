#include "common_proxy.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

amts::CommonProxy::CommonProxy() 
    : m_rayRenderer(nullptr),
        m_scene(nullptr),
        m_mainCamera(nullptr),
        m_materialPool(nullptr) { 

}

amts::CommonProxy::~CommonProxy() {

}

void amts::CommonProxy::preinit() {

}

void amts::CommonProxy::init() {
    m_rayRenderer = std::make_unique<RayRenderer>();
    m_scene = std::make_unique<Scene>();
    m_mainCamera = std::make_unique<Camera>();
    m_materialPool = std::make_unique<MaterialCollection>();
}

void amts::CommonProxy::load() {
    // Materials
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

    // Scene objects
    m_scene->m_objects.emplace_back(std::make_unique<SphereObject>(
        "Big red sphere", 
        Vec3f(0.0f, 0.0f, -2.5f), 
        1,
        0.75f
    ));

    m_scene->m_objects.emplace_back(std::make_unique<SphereObject>(
        "Blue sphere",
        Vec3f(1.0f, 0.0f, -2.0f),
        2,
        0.32f
    ));
    
    m_scene->m_objects.emplace_back(std::make_unique<SphereObject>(
        "Emissive sphere",
        Vec3f(-1.0f, 0.0f, -2.5f),
        3,
        0.32f
    ));

    m_scene->m_objects.emplace_back(std::make_unique<PlaneObject>(
        "Floor plane",
        Vec3f(0.0f, -0.5f, 0.0f),
        4,
        Vec3f(0.0f, -1.0f, 0.0f)
    ));
}

void amts::CommonProxy::run() {

}

void amts::CommonProxy::unload() {

}

void amts::CommonProxy::cleanup() {
    m_rayRenderer = nullptr;
    m_scene = nullptr;
    m_mainCamera = nullptr;
    m_materialPool = nullptr;
}

void amts::CommonProxy::save_rendering_target_to_png(RenderingTarget* target, const std::string& fileName) {
    stbi_write_png(fileName.c_str(), target->get_width(), target->get_height(), 4, *target->get_pixel_data_ptr(), target->get_width() * sizeof(u32));
}