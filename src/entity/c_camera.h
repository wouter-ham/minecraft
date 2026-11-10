#ifndef C_CAMERA_H
#define C_CAMERA_H

#include "../util/camera.h"
#include "ecs.h"

struct CameraComponent {
    vec3s offset;
    struct PerspectiveCamera camera;
};

void c_camera_init(struct ECS *ecs);

#endif