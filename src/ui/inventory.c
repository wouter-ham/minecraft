#include "inventory.h"
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

static void render(const struct UIInventory *self) {
    if (!self->visible) {
        return;
    }

    renderer_quad_color(
        &state.renderer,
        (vec2s){.raw = {(float) state.window->size.x, (float) state.window->size.y}},
        (vec4s){.raw = {0.0f, 0.0f, 0.0f, 0.5f}},
        glms_mat4_identity()
    );

    const auto base_offset = (vec2s){
        .raw = {
            ((f32) state.window->size.x - (INVENTORY_COLUMNS * SLOT_PIXELS)) / 2.0f,
            ((f32) state.window->size.y - (INVENTORY_ROWS * SLOT_PIXELS)) / 2.0f
        }
    };

    for (size_t i = 0; i < INVENTORY_ROWS; i++) {
        for (size_t j = 0; j < INVENTORY_COLUMNS; j++) {
            const vec2s offset = glms_vec2_add(
                (vec2s){
                    .raw = {
                        (f32) j * SLOT_PIXELS,
                        (f32) i * SLOT_PIXELS
                    }
                },
                base_offset
            );

            // renders the item frame
            renderer_quad_texture(
                &state.renderer,
                state.renderer.textures[TEXTURE_HOTBAR],
                (vec2s){.raw = {SLOT_PIXELS, SLOT_PIXELS}},
                (vec4s){.raw = {1.0f, 1.0f, 1.0f, 1.0f}},
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
