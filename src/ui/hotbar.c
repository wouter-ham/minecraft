#include "hotbar.h"
#include "ui.h"

#include "../state.h"
#include "item_slot.h"

static void render_icon(const vec2s offset, const enum BlockId block_id) {
    mat4s m = glms_mat4_identity();
    m = glms_translate(
        m,
        (vec3s){
            .raw = {
                offset.x + ICON_OFFSET_PIXELS,
                offset.y + ICON_OFFSET_PIXELS,
                0.0f
            }
        }
    );
    m = glms_scale(m, (vec3s){.raw = {ICON_SIZE_PIXELS, ICON_SIZE_PIXELS, 1.0f}});

    if (BLOCKS[block_id].get_texture_location != NULL) {
        vec2s uv_min, uv_max;
        atlas_get(
            state.renderer.block_atlas.atlas,
            BLOCKS[block_id].get_texture_location(&state.world, GLMS_IVEC3_ZERO, UP),
            &uv_min,
            &uv_max
        );
        renderer_quad_texture(
            &state.renderer, state.renderer.block_atlas.atlas.texture,
            (vec2s){.raw = {1.0f, 1.0f}},
            (vec4s){.raw = {1.0f, 1.0f, 1.0f, 1.0f}},
            uv_min,
            uv_max,
            m
        );
    }
}

static void render(const struct UIHotbar *self) {
    const auto base_offset = (vec2s){
        .raw = {((f32) state.window->size.x - (HOTBAR_SLOTS * SLOT_PIXELS)) / 2.0f, 16.0f}
    };

    for (size_t i = 0; i < HOTBAR_SLOTS; i++) {
        const vec2s offset = glms_vec2_add((vec2s){.raw = {(f32) i * SLOT_PIXELS, 0}}, base_offset);

        renderer_quad_texture(
            &state.renderer, state.renderer.textures[TEXTURE_HOTBAR],
            (vec2s){.raw = {SLOT_PIXELS, SLOT_PIXELS}},
            (vec4s){.raw = {1.0f, 1.0f, 1.0f, 1.0f}},
            (vec2s){.raw = {0.0f, 0.0f}}, (vec2s){.raw = {0.5f, 1.0f}},
            glms_translate_make((vec3s){.raw = {offset.x, offset.y, 0.0f}})
        );

        if (i == self->index) {
            renderer_quad_texture(
                &state.renderer, state.renderer.textures[TEXTURE_HOTBAR],
                (vec2s){.raw = {SLOT_PIXELS, SLOT_PIXELS}},
                (vec4s){.raw = {1.0f, 1.0f, 1.0f, 1.0f}},
                (vec2s){.raw = {0.5f, 0.0f}}, (vec2s){.raw = {1.0f, 1.0f}},
                glms_translate_make((vec3s){.raw = {offset.x, offset.y, 0.0f}})
            );
        }

        render_icon(offset, self->values[i]);
    }
}

static void update(struct UIHotbar *self) {
    for (size_t i = 0; i < 10; i++) {
        if (state.window->keyboard.keys[GLFW_KEY_0 + i].pressed) {
            self->index = i == 0 ? 9 : (i - 1);
        }
    }
}

static void scrollCallback(GLFWwindow *_window, double xoffset, double yoffset) {
    const auto self = (struct UIHotbar *) glfwGetWindowUserPointer(_window);
    if (yoffset == 1) {
        if (self->index == 0) {
            self->index = 9;
        } else {
            self->index -= 1;
        }
    } else if (yoffset == -1) {
        self->index += 1;
        self->index = self->index % 10;
    }
}

struct UIComponent hotbar_init(struct UIHotbar *self) {
    self->index = 0;

    glfwSetWindowUserPointer(state.window->handle, self);
    glfwSetScrollCallback(state.window->handle, scrollCallback);

    memcpy(self->values, (enum BlockId[]){
               GRASS,
               DIRT,
               STONE,
               COBBLESTONE,
               PLANKS,
               LOG,
               GLASS,
               ROSE,
               TORCH,
               SAND,
               LEAVES
           }, HOTBAR_SLOTS * sizeof(enum BlockId));

    return (struct UIComponent){
        .component = self,
        .render = (FUIComponent) render,
        .update = (FUIComponent) update,
        .tick = nullptr
    };
}
