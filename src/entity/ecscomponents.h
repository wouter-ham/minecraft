#ifndef ECSCOMPONENTS_H
#define ECSCOMPONENTS_H

#include "c_position.h"
#include "c_camera.h"
#include "c_control.h"
#include "c_physics.h"
#include "c_movement.h"
#include "c_blocklook.h"
#include "c_debug.h"
#include "c_light.h"

#define ECS_DECL_COMPONENT(_name)\
    extern void _name##_init(struct ECS *ecs);\
    _name##_init(ecs);

static inline void ecs_init_internal(struct ECS *ecs) {
    ECS_DECL_COMPONENT(c_position);
    ECS_DECL_COMPONENT(c_camera);
    ECS_DECL_COMPONENT(c_control);
    ECS_DECL_COMPONENT(c_physics);
    ECS_DECL_COMPONENT(c_movement);
    ECS_DECL_COMPONENT(c_blocklook);
    ECS_DECL_COMPONENT(c_debug);
    ECS_DECL_COMPONENT(c_light);
}

#define ECSCOMPONENT_LAST C_LIGHT
enum ECSComponent {
    C_POSITION = 0,
    C_CAMERA,
    C_CONTROL,
    C_PHYSICS,
    C_MOVEMENT,
    C_BLOCKLOOK,
    C_DEBUG,
    C_LIGHT
};

#endif