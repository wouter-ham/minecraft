#ifndef POSITION_H
#define POSITION_H

#include "../util/util.h"
#include "ecs.h"

struct PositionComponent {
    vec3s position;

    ivec3s block, offset;
    bool block_changed, offset_changed;
};

void c_position_init(struct ECS *ecs);

#endif