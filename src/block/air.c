#include "block.h"

void air_init() {
    struct Block air = BLOCK_DEFAULT;
    air.id = AIR;
    air.transparent = true;
    air.solid = false;
    air.get_texture_location = nullptr;
    BLOCKS[AIR] = air;
}