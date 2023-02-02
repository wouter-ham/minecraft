#include "inventory.h"
#include "ui.h"

#include "../state.h"
#include "item_slot.h"

static void render_icon(vec2s offset, enum BlockId block_id) {
    mat4s m = glms_mat4_identity();
    m = glms_translate(
            m,
            (vec3s) {
                    {
                            offset.x + ICON_OFFSET_PIXELS,
                            offset.y + ICON_OFFSET_PIXELS,
                            0.0f
                    }
            }
    );
    m = glms_scale(m, (vec3s) {{ICON_SIZE_PIXELS, ICON_SIZE_PIXELS, 1.0f}});

    vec2s uv_min, uv_max;
    atlas_get(
            state.renderer.block_atlas.atlas,
            BLOCKS[block_id].get_texture_location(&state.world, GLMS_IVEC3_ZERO, UP), // TODO fix AIR texture
            &uv_min,
            &uv_max
    );

    renderer_quad_texture(
            &state.renderer,
            state.renderer.block_atlas.atlas.texture,
            (vec2s) {{1.0f, 1.0f}},
            (vec4s) {{1.0f, 1.0f, 1.0f, 1.0f}},
            uv_min,
            uv_max,
            m
    );
}

static void render(struct UIInventory *self) {
    if (!self->visible) {
        return;
    }

    vec2s base_offset = (vec2s) {
            {
                    ((f32) state.window->size.x - (INVENTORY_ROWS * SLOT_PIXELS)) / 2.0f,
                    ((f32) state.window->size.y - (INVENTORY_COLUMNS * SLOT_PIXELS)) / 2.0f
            }
    };

    for (size_t i = 0; i < INVENTORY_ROWS; i++) {
        for (size_t j = 0; j < INVENTORY_COLUMNS; j++) {
            vec2s offset = glms_vec2_add(
                    (vec2s) {
                            {
                                    (f32) i * SLOT_PIXELS,
                                    (f32) j * SLOT_PIXELS
                            }
                    },
                    base_offset
            );

            renderer_quad_texture(
                    &state.renderer,
                    state.renderer.textures[TEXTURE_HOTBAR],
                    (vec2s) {{SLOT_PIXELS, SLOT_PIXELS}},
                    (vec4s) {{1.0f, 1.0f, 1.0f, 1.0f}},
                    (vec2s) {{0.0f, 0.0f}},
                    (vec2s) {{0.5f, 1.0f}},
                    glms_translate_make((vec3s) {{offset.x, offset.y, 0.0f}})
            );

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

    return (struct UIComponent) {
            .component = self,
            .render = (FUIComponent) render,
            .update = (FUIComponent) update,
            .tick = NULL
    };
}
