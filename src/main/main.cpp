#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <unordered_map>

#include <functional>

#define OMNI_TYPES_MATRIX_COLLUM_MAJOR_ORDER
#include <omni_types.tpp>

using namespace omni::types;

const i32 WINDOW_WIDTH = 800;
const i32 WINDOW_HEIGHT = 600;

using Color = Vec4f;

struct RayCollisionData {
    bool collision;

    Vec3f point = Vec3f::splat(0.0f);
    Vec3f reflection = Vec3f::splat(0.0f);
    Color color = Color(0.0f, 0.0f, 0.0f, 1.0f);

    bool reflect = false;
    f32 depth = std::numeric_limits<f32>::max();
};

struct Ray {
    Vec3f origin;
    Vec3f direction;
};

u32 to_rgba_color(const Color& color) {
    u8 r = u8(color.x * 255.0f); // 0x000000FF
    u8 g = u8(color.y * 255.0f); // 0x000000FF
    u8 b = u8(color.z * 255.0f); // 0x000000FF
    u8 a = u8(color.w * 255.0f); // 0x000000FF

    return (a << 24) | (b << 16) | (g << 8) | (r << 0);
}

struct Object {
    Vec3f pos;

    Object(const Vec3f _pos) : pos(_pos) {}
    virtual ~Object() {}
    virtual RayCollisionData hit(const Ray& ray) = 0;
};

struct SphereObject : public Object {
    f32 radius;
    Color color;

    SphereObject(const Vec3f _pos, Color _color, const f32 _radius) : Object(_pos), radius(_radius) {
        color = _color;
    } 

    RayCollisionData hit(const Ray& ray) override {
        const auto oc = ray.origin - pos;

        const f32 a = ray.direction.dot(ray.direction);
        const f32 b = 2.0f * ray.direction.dot(oc);
        const f32 c = oc.dot(oc) - radius*radius;

        const f32 discriminant = b*b - 4*a*c;

        if(discriminant >= 0) {
            f32 d0 = (-b + sqrt(discriminant)) / (2.0f * a);
            f32 d1 = (-b - sqrt(discriminant)) / (2.0f * a);

            if(d0 > d1) std::swap(d0, d1);

            if(d0 < 0.0f) {
                d0 = d1;
                if(d0 < 0) return RayCollisionData{false};
            }

            const f32 d = d0;

            const Vec3f collisionPoint = ray.origin + ray.direction*(d * 0.99f);
            const Vec3f normal = (collisionPoint - pos).normalize();

            const Vec3f refl = ray.direction - normal * 2.0f * (ray.direction.dot(normal));
            const Vec3f normalColor = ((normal + 1.0f) / 2.0f);

            return RayCollisionData{
                true,
                collisionPoint,
                refl,
                color,// Color{normalColor.x, normalColor.y, normalColor.z, 1.0f},
                true,
                d
            };
        }

        return RayCollisionData{false};
    }
};

struct PlaneObject : public Object {
    Vec3f normal;

    PlaneObject(const Vec3f _pos) : Object(_pos) {
        normal = Vec3f(0.0f, 1.0f, 0.0f);
    } 

    RayCollisionData hit(const Ray& ray) override {
        f32 denom = pos.dot(normal);

        if (abs(denom) <= 1e-4f)
            return RayCollisionData{false};

        f32 t = -(ray.origin.dot(normal) + denom) / ray.direction.dot(normal);

        if (t <= 1e-4)
            return RayCollisionData{false};

        const Vec3f collisionPoint = ray.origin + ray.direction * t;
        const Vec3f refl = ray.direction - normal * 2.0f * (ray.direction.dot(normal));

        return RayCollisionData{
            true,
            collisionPoint,
            refl,
            Color{0.2f, 0.2f, 0.2f, 1.0f},
            false,
            t
        };
    }
};

i32 main(i32 argc, char *argv[]) {
    std::ignore = argc;
    std::ignore = argv;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow("Hello World", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(win, NULL, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

    const f64 centerXPos = WINDOW_WIDTH / 2.0f;
    const f64 centerYPos = WINDOW_HEIGHT / 2.0f;
    SDL_WarpMouseInWindow(win, centerXPos, centerYPos);

    std::vector<Object*> objects;

    objects.push_back(new SphereObject(Vec3f(0.0f, 0.0f, -2.5f), Color{1.0f, 0.0f, 0.0f, 1.0f}, 0.75f));
    objects.push_back(new SphereObject(Vec3f(1.0f, 0.0f, -1.1f), Color{0.0f, 1.0f, 0.0f, 1.0f}, 0.32f));
    objects.push_back(new SphereObject(Vec3f(-1.0f, 0.0f, -1.1f), Color{0.0f, 0.0f, 1.0f, 1.0f}, 0.32f));
    objects.push_back(new PlaneObject(Vec3f(0.0f, -0.5f, 0.0f)));

    Vec3f origin = Vec3f::splat(0.0f);
    Vec3f direction = Vec3f(0.0f, 0.0f, -1.0f);
    Vec3f rotation = Vec3f::splat(0.0f);

    std::unordered_map<i32, bool> KEYS;
    bool mouseLocked = true;

    bool loopShouldStop = false;
    while (!loopShouldStop) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    loopShouldStop = true;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    KEYS[event.key.keysym.sym] = true;
                    break;
                case SDL_EVENT_KEY_UP:
                    KEYS[event.key.keysym.sym] = false;
                    break;
                default:
                    break;
            }
        }

        if (KEYS[SDLK_w]) {
            // origin += Vec3f(1.0f, 0.0f, 1.0f) * direction.normalize() * 0.1f;
            origin -= Vec3f(direction.z, 0.0f, direction.x).normalize() * 0.02f;
        }

        if (KEYS[SDLK_s]) {
            origin += Vec3f(direction.z, 0.0f, direction.x).normalize() * 0.02f;
            // origin -= Vec3f(1.0f, 0.0f, 1.0f) * direction.normalize() * 0.1f;
        }

        if (KEYS[SDLK_a]) {
            origin -= Vec3f(1.0f, 0.0f, -1.0f) * direction.normalize() * 0.02f;
        }

        if (KEYS[SDLK_d]) {
            origin += Vec3f(1.0f, 0.0f, -1.0f) * direction.normalize() * 0.02f;
        }

        if (KEYS[SDLK_SPACE]) {
            origin.y += 0.1f;
        }

        if (KEYS[SDLK_LSHIFT]) {
            origin.y -= 0.1f;
        }

        bool oldMouseLockState = mouseLocked;
        static bool buttonPressed = false;
        const bool isDebugButtonPressed = KEYS[SDLK_t];
        mouseLocked = (isDebugButtonPressed && !buttonPressed) ? !mouseLocked : mouseLocked;  
        buttonPressed = !isDebugButtonPressed ? false : true;

        if(oldMouseLockState != mouseLocked) {
            SDL_WarpMouseInWindow(win, centerXPos, centerYPos);
        }

        if(mouseLocked == true) {
            f32 xPos, yPos;
            SDL_GetMouseState(&xPos, &yPos);
            
            const f32 deltaX = floor(centerXPos) - xPos;
            const f32 deltaY = floor(centerYPos) - yPos;

            rotation.x -= deltaY * 0.005f;
            rotation.y += deltaX * 0.005f;

            SDL_WarpMouseInWindow(win, centerXPos, centerYPos);
        }

        direction.x = cos(rotation.y) * cos(rotation.x);
        direction.y = sin(rotation.x);
        direction.z = sin(rotation.y) * cos(rotation.x);

        direction.normalize();

        SDL_RenderClear(renderer);

        const SDL_FRect window_rect_f = {0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT};
        const SDL_Rect window_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

        void *pixels; i32 pitch;
        SDL_LockTexture(texture, &window_rect, &pixels, &pitch);

        const f32 aspect = static_cast<f32>(WINDOW_WIDTH) / static_cast<f32>(WINDOW_HEIGHT);

        char title[40];
        sprintf(title, "Amaterasu pos: %.1f %.1f %.1f", origin.x, origin.y, origin.z);
        SDL_SetWindowTitle(win, title);

        Mat4f rot = trait_bryan_angle_yxz(rotation);

        for(i32 xW = 0; xW < WINDOW_WIDTH; ++xW) {
            for(i32 yH = 0; yH < WINDOW_HEIGHT; ++yH) {

                Vec3f dir(
                    (xW /static_cast<f32>(WINDOW_WIDTH) - 0.5f),
                    (yH /static_cast<f32>(WINDOW_HEIGHT) - 0.5f) / aspect,
                    -1.0f
                );

                u32& pixel = static_cast<u32*>(pixels)[xW + yH * WINDOW_WIDTH];
                Color pixelColor{0.0f, 0.0f, 0.0f, 1.0f};

                Vec4f rotated = rot * Vec4f(dir.x, dir.y, dir.z, 1.0);
                dir = Vec3f(rotated.x, rotated.y, rotated.z);

                Ray ray{origin, dir};

                f32 depth = std::numeric_limits<f32>::max();

                const u64 maxSteps = 5;
                f32 mult = 1.0f;

                for(u64 i = 0; i < maxSteps; ++i) {
                    RayCollisionData collision{false};

                    for(Object* obj : objects) {
                        const RayCollisionData tmpCollision = obj->hit(ray); 

                        if(tmpCollision.collision) {
                            if(tmpCollision.depth < depth) {
                                depth = tmpCollision.depth;
                                collision = tmpCollision;
                            }
                        }
                    }

                    if(collision.collision) {
                        pixelColor += collision.color * mult;
                        mult *= 0.75f;

                        if(collision.reflect)
                            ray = Ray{collision.point + (collision.reflection * 0.0001f), collision.reflection};
                        else
                            break;
                    }
                }

                pixel = to_rgba_color(pixelColor);
            }
        }

        SDL_UnlockTexture(texture);
        SDL_RenderTexture(renderer, texture, &window_rect_f, &window_rect_f);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}