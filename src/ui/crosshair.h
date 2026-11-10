#ifndef CROSSHAIR_H
#define CROSSHAIR_H

#include "../util/util.h"

struct UICrosshair {
    bool enabled;
};

struct UIComponent crosshair_init(struct UICrosshair *self);

#endif