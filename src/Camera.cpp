#include "Camera.h"

using namespace glm;

Camera::Camera(vec3 pos, vec3 forward)
{
    _pos = pos;
    _forward = normalize(forward);
    _right = normalize(glm::cross(_forward, vec3(0, 1, 0)));
    _up = normalize(-glm::cross(_forward, _right));
}

Camera::~Camera()
{
}

Ray Camera::cast_ray(int i, int j, int width, int height, float near, float fov)
{
    vec3 P1 = _pos + near*_forward - near*tan(fov)*_right - near*tan(fov)*(height/(float)width)*_up;

    float factor = std::fmaxf(height, width);
    vec3 pos = P1 + 
               (i / (float) factor) * 2 * near * glm::tan(fov) * _right +
               (j / (float) factor) * 2 * near * glm::tan(fov) * _up;
    vec3 dir = normalize(P1 - _pos);

    return Ray(pos, dir);
}

glm::vec3 Camera::get_right()
{
    return _right;
}

glm::vec3 Camera::get_up()
{
    return _up;
}

glm::vec3 Camera::get_pos()
{
    return _pos;
}
