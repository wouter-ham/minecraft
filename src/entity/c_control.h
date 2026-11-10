#ifndef C_CONTROL_H
#define C_CONTROL_H

#include "../util/util.h"
#include "ecs.h"

struct ControlComponent {
    f32 mouse_sensitivity;
};

void c_control_init(struct ECS *ecs);

#endif