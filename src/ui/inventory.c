#include "inventory.h"
#include "ui.h"

#include "../state.h"
#include "item_slot.h"

static vec2s get_scale() {
    int window_x, window_y;
    glfwGetWindowSize(state.window->handle, &window_x, &window_y);

    int frame_x, frame_y;
    glfwGetFramebufferSize(state.window->handle, &frame_x, &frame_y);

    return (vec2s){.raw = {(float) frame_x / (float) window_x, (float) frame_y / (float) window_y}};
}

static vec2s base_offset() {
    return (vec2s){
        .raw = {
            ((f32) state.window->size.x - (INVENTORY_COLUMNS * SLOT_PIXELS)) / 2.0f,
            ((f32) state.window->size.y - (INVENTORY_ROWS * SLOT_PIXELS)) / 2.0f
        }
    };
}

static bool in_bounds(const vec4s bounds, const vec2s coords) {
    if (coords.x < bounds.x || coords.x > bounds.z) {
        return false;
    }

    if (coords.y < bounds.y || coords.y > bounds.w) {
        return false;
    }

    return true;
}

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
            BLOCKS[block_id].get_texture_location(&state.world, GLMS_IVEC3_ZERO, NORTH),
            &uv_min,
            &uv_max
        );

        renderer_quad_texture(
            &state.renderer,
            state.renderer.block_atlas.atlas.texture,
            (vec2s){.raw = {1.0f, 1.0f}},
            (vec4s){.raw = {1.0f, 1.0f, 1.0f, 1.0f}},
            uv_min,
            uv_max,
            m
        );
    }
}

static bool slot_at(const struct UIInventory *self, const vec2s mouse, size_t *row_out, size_t *col_out) {
    if (!self->visible) {
        return false;
    }

    int window_x, window_y;
    glfwGetWindowSize(state.window->handle, &window_x, &window_y);

    const vec2s offset = base_offset();

    const vec2s scale = get_scale();
    const vec2s mouse_pos = {.raw = {mouse.x * scale.x, (float) state.window->size.y - (mouse.y * scale.y)}};

    const vec4s bounds = {
        .raw = {
            offset.x,
            offset.y,
            offset.x + SLOT_PIXELS * INVENTORY_COLUMNS,
            offset.y + SLOT_PIXELS * INVENTORY_ROWS
        }
    };

    if (!in_bounds(bounds, mouse_pos)) {
        printf("in_bounds: false\n");
        return false;
    }

    *col_out = (int) (((float) mouse_pos.x - offset.x) / SLOT_PIXELS);
    *row_out = (int) (((float) mouse_pos.y - offset.y) / SLOT_PIXELS);

    return true;
}

static void render(const struct UIInventory *self) {
    if (!self->visible) {
        return;
    }

    // background shade
    renderer_quad_color(
        &state.renderer,
        (vec2s){.raw = {(float) state.window->size.x, (float) state.window->size.y}},
        (vec4s){.raw = {0.0f, 0.0f, 0.0f, 0.5f}},
        glms_mat4_identity()
    );

    size_t row, col;
    const bool is_hovering = slot_at(self, state.window->mouse.position, &row, &col);

    for (size_t i = 0; i < INVENTORY_ROWS; i++) {
        for (size_t j = 0; j < INVENTORY_COLUMNS; j++) {
            const vec2s offset = glms_vec2_add(
                (vec2s){
                    .raw = {
                        (f32) j * SLOT_PIXELS,
                        (f32) i * SLOT_PIXELS
                    }
                },
                base_offset()
            );

            // renders the item frame
            renderer_quad_texture(
                &state.renderer,
                state.renderer.textures[TEXTURE_HOTBAR],
                (vec2s){.raw = {SLOT_PIXELS, SLOT_PIXELS}},
                // red frame when hovering over it
                is_hovering && row == i && col == j
                    ? (vec4s){.raw = {1.0f, 0.0f, 0.0f, 1.0f}}
                    : (vec4s){.raw = {1.0f, 1.0f, 1.0f, 1.0f}},
                (vec2s){.raw = {0.0f, 0.0f}},
                (vec2s){.raw = {0.5f, 1.0f}},
                glms_translate_make((vec3s){.raw = {offset.x, offset.y, 0.0f}})
            );

            // renders the item itself
            render_icon(offset, self->values[i][j]);
        }
    }
}

static void update(struct UIInventory *self) {
    if (state.window->keyboard.keys[GLFW_KEY_E].pressed) {
        self->visible = !self->visible;

        mouse_set_grabbed(!self->visible);
    }
}

struct UIComponent inventory_init(struct UIInventory *self) {
    self->visible = false;

    enum BlockId items[INVENTORY_ROWS][INVENTORY_COLUMNS];

    for (int i = 0; i < INVENTORY_ROWS; ++i) {
        for (int j = 0; j < INVENTORY_COLUMNS; ++j) {
            items[i][j] = AIR;
        }
    }

    memcpy(self->values, items, INVENTORY_ROWS * INVENTORY_COLUMNS * sizeof(enum BlockId));

    return (struct UIComponent){
        .component = self,
        .render = (FUIComponent) render,
        .update = (FUIComponent) update,
        .tick = nullptr
    };
}
