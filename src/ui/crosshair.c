#include "crosshair.h"
#include "ui.h"

#include "../state.h"
#include "../gfx/renderer.h"

static void render(struct UICrosshair *self) {
    if (state.ui.inventory.visible) {
        return;
    }

    renderer_quad_texture(
        &state.renderer, state.renderer.textures[TEXTURE_CROSSHAIR],
        (vec2s) {{ 16, 16 }}, (vec4s) {{ 1.0, 1.0, 1.0, 0.4}},
        (vec2s) {{ 0, 0 }}, (vec2s) {{ 1, 1 }},
        glms_translate_make((vec3s) {{ ((f32)state.window->size.x / 2) - 8, ((f32)state.window->size.y / 2) - 8, 0}}));
}

struct UIComponent crosshair_init(struct UICrosshair *self) {
    self->enabled = true;

    return (struct UIComponent) {
        .component = self,
        .render = (FUIComponent) render
    };
}
