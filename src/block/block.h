#ifndef BLOCK_H
#define BLOCK_H

#include "../util/util.h"
#include "../gfx/blockatlas.h"
#include "../world/blockmesh.h"
#include "../world/light.h"

struct World;

#define BLOCK_ID_LAST PINE_LEAVES
enum BlockId : uint8_t {
    AIR = 0,
    GRASS = 1,
    DIRT = 2,
    STONE = 3,
    SAND = 4,
    WATER = 5,
    GLASS = 6,
    LOG = 7,
    LEAVES = 8,
    ROSE = 9,
    BUTTERCUP = 10,
    COAL = 11,
    COPPER = 12,
    LAVA = 13,
    CLAY = 14,
    GRAVEL = 15,
    PLANKS = 16,
    TORCH = 17,
    COBBLESTONE = 18,
    SNOW = 19,
    PODZOL = 20,
    SHRUB = 21,
    TALLGRASS = 22,
    PINE_LOG = 23,
    PINE_LEAVES = 24,
    BLOCK_ID_COUNT
};

struct Block {
    enum BlockId id;

    bool transparent;

    bool liquid;

    bool can_emit_light;

    bool animated;

    // the mesh type of this block, see blockmesh.h
    enum BlockMeshType mesh_type;

    // if true, then this block's aabb is determined by get_aabb
    bool solid;

    f32 gravity_modifier;

    // drag for non-solid blocks
    f32 drag;

    // slipperiness for solid blocks
    f32 slipperiness;

    ivec2s (*get_texture_location)(struct World *world, ivec3s pos, enum Direction d);
    void (*get_mesh_information)(
        struct World *world, ivec3s pos, enum Direction d,
        vec3s *offset_out, vec3s *size_out,
        ivec2s *uv_offset_out, ivec2s *uv_size_out);
    void (*get_animation_frames)(ivec2s out[BLOCK_ATLAS_FRAMES]);
    Torchlight (*get_torchlight)(struct World *world, ivec3s pos);
    void (*get_aabb)(struct World *world, ivec3s pos, AABB dest);
};

#define MAX_BLOCK_ID INT16_MAX

extern struct Block BLOCK_DEFAULT;
extern struct Block BLOCKS[MAX_BLOCK_ID];

void block_init();

#endif
