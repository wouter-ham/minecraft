#include "window.h"

#include <stdio.h>
#include <stdlib.h>

struct Window window;

static void size_callback(GLFWwindow *handle, const int width, const int height) {
    glViewport(0, 0, width, height);
    window.size = (ivec2s){.raw = {width, height}};
}

static void cursor_callback(GLFWwindow *handle, const double xp, const double yp) {
    const vec2s p = {.raw = {(float) xp, (float) yp}};

    if (mouse_get_grabbed()) {
        window.mouse.delta = glms_vec2_sub(p, window.mouse.position);
        window.mouse.delta.x = clamp(window.mouse.delta.x, -100.0f, 100.0f);
        window.mouse.delta.y = clamp(window.mouse.delta.y, -100.0f, 100.0f);
    }

    window.mouse.position = p;
}

static void key_callback(GLFWwindow *handle, const int key, int scancode, const int action, int mods) {
    if (key < 0) {
        return;
    }

    switch (action) {
        case GLFW_PRESS:
            window.keyboard.keys[key].down = true;
            break;
        case GLFW_RELEASE:
            window.keyboard.keys[key].down = false;
            break;
        default:
            break;
    }
}

static void mouse_callback(GLFWwindow *handle, int button, int action, int mods) {
    if (button < 0) {
        return;
    }

    switch (action) {
        case GLFW_PRESS:
            window.mouse.buttons[button].down = true;
            break;
        case GLFW_RELEASE:
            window.mouse.buttons[button].down = false;
            break;
        default:
            break;
    }
}

static void error_callback(int code, const char *description) {
    fprintf(stderr, "GLFW error %d: %s\n", code, description);
}

void window_create(FWindow init, FWindow destroy, FWindow tick, FWindow update, FWindow render) {
    window.init = init;
    window.destroy = destroy;
    window.tick = tick;
    window.update = update;
    window.render = render;

    window.last_frame = NOW();
    window.last_second = NOW();

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        fprintf(stderr, "%s", "error initializing GLFW\n");
        exit(1);
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window.size = (ivec2s){.raw = {1280, 720}};
    window.handle = glfwCreateWindow(window.size.x, window.size.y, "Minecraft", nullptr, nullptr);
    if (window.handle == NULL) {
        fprintf(stderr, "%s", "error creating window\n");
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window.handle);

    // configure callbacks
    glfwSetFramebufferSizeCallback(window.handle, size_callback);
    glfwSetCursorPosCallback(window.handle, cursor_callback);
    glfwSetKeyCallback(window.handle, key_callback);
    glfwSetMouseButtonCallback(window.handle, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fprintf(stderr, "%s", "error initializing GLAD\n");
        glfwTerminate();
        exit(1);
    }

    glfwSwapInterval(1);
}

static void button_array_tick(const size_t n, struct Button *buttons) {
    for (size_t i = 0; i < n; i++) {
        buttons[i].pressed_tick = buttons[i].down && !buttons[i].last_tick;
        buttons[i].last_tick = buttons[i].down;
    }
}

static void button_array_update(const size_t n, struct Button *buttons) {
    for (size_t i = 0; i < n; i++) {
        buttons[i].pressed = buttons[i].down && !buttons[i].last;
        buttons[i].last = buttons[i].down;
    }
}

static void init() {
    window.init();
}

static void destroy(void) {
    window.destroy();
    glfwTerminate();
}

static void tick(void) {
    window.ticks++;
    button_array_tick(GLFW_MOUSE_BUTTON_LAST, window.mouse.buttons);
    button_array_tick(GLFW_KEY_LAST, window.keyboard.keys);
    window.tick();
}

static void update(void) {
    button_array_update(GLFW_MOUSE_BUTTON_LAST, window.mouse.buttons);
    button_array_update(GLFW_KEY_LAST, window.keyboard.keys);
    window.update();

    // reset update delta
    window.mouse.delta = GLMS_VEC2_ZERO;
}

static void render(void) {
    window.frames++;
    window.render();
}

void window_loop(void) {
    init();

    while (!glfwWindowShouldClose(window.handle)) {
        const u64 now = NOW();

        window.frame_delta = now - window.last_frame;
        window.last_frame = now;

        if (now - window.last_second > NS_PER_SECOND) {
            window.fps = window.frames;
            window.tps = window.ticks;
            window.frames = 0;
            window.ticks = 0;
            window.last_second = now;

            printf("FPS: %ld | TPS: %ld\n", window.fps, window.tps);
        }

        // tick processing
        const u64 NS_PER_TICK = (NS_PER_SECOND / 60);
        u64 tick_time = window.frame_delta + window.tick_remainder;
        while (tick_time > NS_PER_TICK) {
            tick();
            tick_time -= NS_PER_TICK;
        }
        window.tick_remainder = max(tick_time, 0);

        update();
        render();
        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }

    destroy();
    exit(0);
}

void mouse_set_grabbed(bool grabbed) {
    glfwSetInputMode(window.handle, GLFW_CURSOR, grabbed ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

bool mouse_get_grabbed(void) {
    return glfwGetInputMode(window.handle, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
}
