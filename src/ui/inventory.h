#ifndef INVENTORY_H
#define INVENTORY_H

#include "../util/util.h"
#include "../block/block.h"

#define INVENTORY_ROWS 3
#define INVENTORY_COLUMNS 10

struct UIInventory {
    enum BlockId values[INVENTORY_ROWS][INVENTORY_COLUMNS];
    bool visible;
};

#endif
