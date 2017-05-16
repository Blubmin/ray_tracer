#pragma once

#include <glm/glm.hpp>

class Ray {
    glm::vec3 _pos;
    glm::vec3 _dir;
public:
    Ray(glm::vec3 pos, glm::vec3 dir);
    ~Ray();

    glm::vec3 get_pos();
    glm::vec3 get_dir();
    glm::vec3 point_at(float t);
};