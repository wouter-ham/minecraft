#ifndef C_DEBUG_H
#define C_DEBUG_H

#include "../util/util.h"
#include "ecs.h"

struct DebugComponent {
    bool enabled;
};

void c_debug_init(struct ECS *ecs);

#endif