#pragma once

#include <glm\glm.hpp>

#include "Ray.h"

class Camera {
    glm::vec3 _pos;
    glm::vec3 _forward;
    glm::vec3 _right;
    glm::vec3 _up;

public:
    Camera(glm::vec3 pos, glm::vec3 look_at);
    ~Camera();

    Ray cast_ray(int i, int j, int width, int height, float near, float fov);
    glm::vec3 get_right();
    glm::vec3 get_up();
    glm::vec3 get_pos();
};