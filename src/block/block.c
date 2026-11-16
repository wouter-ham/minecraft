#include "block.h"
#include "../gfx/blockatlas.h"

static ivec2s get_texture_location(struct World *world, ivec3s pos, enum Direction d) {
//    assert(false);
    return GLMS_IVEC2_ZERO;
}

static void get_animation_frames(ivec2s out[BLOCK_ATLAS_FRAMES]) {
}

static Torchlight get_torchlight(struct World *world, ivec3s pos) {
    return 0x0000;
}

static void get_aabb(struct World *world, const ivec3s pos, AABB dest) {
    dest[0] = IVEC3S2V(pos);
    dest[1] = (vec3s) {.raw = {(f32) pos.x + 1.0f, (f32) pos.y + 1.0f, (f32) pos.z + 1.0f}};
}

static void get_mesh_information(
        struct World *world, ivec3s pos, enum Direction d,
        vec3s *offset_out, vec3s *size_out,
        ivec2s *uv_offset_out, ivec2s *uv_size_out) {
    assert(false);
}

// declared in block.h
struct Block BLOCKS[MAX_BLOCK_ID];

struct Block BLOCK_DEFAULT = {
        .id = -1,
        .transparent = false,
        .liquid = false,
        .can_emit_light = false,
        .animated = false,
        .solid = true,
        .gravity_modifier = 1.0f,
        .drag = 1.0f,
        .slipperiness = 1.0f,
        .mesh_type = BLOCKMESH_DEFAULT,
        .get_texture_location = get_texture_location,
        .get_animation_frames = get_animation_frames,
        .get_torchlight = get_torchlight,
        .get_aabb = get_aabb,
        .get_mesh_information = get_mesh_information
};

#define BLOCK_DECL(_name)\
    extern void _name##_init();\
    _name##_init();

void block_init() {
    BLOCK_DECL(air);
    BLOCK_DECL(grass);
    BLOCK_DECL(dirt);
    BLOCK_DECL(stone);
    BLOCK_DECL(sand);
    BLOCK_DECL(water);
    BLOCK_DECL(glass);
    BLOCK_DECL(log);
    BLOCK_DECL(leaves);
    BLOCK_DECL(rose);
    BLOCK_DECL(buttercup);
    BLOCK_DECL(coal);
    BLOCK_DECL(copper);
    BLOCK_DECL(lava);
    BLOCK_DECL(clay);
    BLOCK_DECL(gravel);
    BLOCK_DECL(planks);
    BLOCK_DECL(torch);
    BLOCK_DECL(cobblestone);
    BLOCK_DECL(snow);
    BLOCK_DECL(podzol);
    BLOCK_DECL(shrub);
    BLOCK_DECL(tallgrass);
    BLOCK_DECL(pine_log);
    BLOCK_DECL(pine_leaves);
}