#include "Light.h"

Light::Light(glm::vec3 pos, glm::vec3 color)
{
    _pos = pos;
    _color = color;
}

Light::~Light()
{
}

glm::vec3 Light::get_pos()
{
    return _pos;
}

glm::vec3 Light::get_color()
{
    return _color;
}
