#pragma once

#include <glm\glm.hpp>

class Light
{
    glm::vec3 _pos;
    glm::vec3 _color;

public:
    Light(glm::vec3 pos, glm::vec3 color);
    ~Light();

    glm::vec3 get_pos();
    glm::vec3 get_color();
};

