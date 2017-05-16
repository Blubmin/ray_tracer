#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <rapidjson\document.h>

#include "Light.h"
#include "Ray.h"

class Sphere {
    int _idx;
    glm::vec3 _pos;
    glm::vec3 _color;
    float _radius;

public:
    Sphere(int idx, glm::vec3 pos, glm::vec3 color, float radius);
    Sphere(int idx, const rapidjson::Value& sphere);
    ~Sphere();

    std::vector<glm::vec3> get_intersections(Ray ray);
    glm::vec3 get_color(Light light, glm::vec3 cam, glm::vec3 pt, bool has_light);
    int get_idx();
};