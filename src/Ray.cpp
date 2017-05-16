#include "Ray.h"

using namespace glm;

Ray::Ray(vec3 pos, vec3 dir) {
    _dir = normalize(dir);
    _pos = pos;
}

Ray::~Ray() {}

glm::vec3 Ray::get_pos()
{
    return _pos;
}

glm::vec3 Ray::get_dir()
{
    return _dir;
}

glm::vec3 Ray::point_at(float t)
{
    return _pos + _dir * t;
}
