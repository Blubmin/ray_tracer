#include "Sphere.h"

#include <iostream>

using namespace glm;
using namespace rapidjson;
using namespace std;

Sphere::Sphere(int idx, vec3 pos, vec3 color, float rad) {
    _idx = idx;
    _pos = pos;
    _color = color;
    _radius = rad;
}

Sphere::Sphere(int idx, const Value& sphere) {
    _idx = idx;

    const Value& pos = sphere["pos"];
    _pos = vec3(pos["x"].GetDouble(), pos["y"].GetDouble(), pos["z"].GetDouble());

    const Value& color = sphere["color"];
    _color = vec3(color["r"].GetDouble(), color["g"].GetDouble(), color["b"].GetDouble());

    const Value& radius = sphere["rad"];
    _radius = radius.GetDouble();
}

Sphere::~Sphere() {}

vector<vec3> Sphere::get_intersections(Ray ray)
{
    vector<vec3> points = vector<vec3>();

    float A = dot(ray.get_dir(), ray.get_dir());
    float B = 2 * dot(ray.get_dir(), ray.get_pos() - _pos);
    float C = dot(ray.get_pos() - _pos, ray.get_pos() - _pos) - pow(_radius, 2);

    float descriminate = pow(B, 2) - 4 * A * C;
    if (descriminate < 0) {
        return points;
    }

    //if (abs(descriminate) < 0.00001f){
    //    points.push_back(ray.point_at(-B / (2 * A)));
    //    return points;
    //}

    points.push_back(ray.point_at((-B + sqrt(descriminate)) / (2 * A)));
    points.push_back(ray.point_at((-B - sqrt(descriminate)) / (2 * A)));

    return points;
}

vec3 Sphere::get_color(Light light, vec3 cam, vec3 pt, bool has_light)
{
    vec3 norm = normalize(pt - _pos);
    vec3 l = normalize(light.get_pos() - pt);
    vec3 view = normalize(cam - pt);

    vec3 ambient = _color * 0.1f;
    if (!has_light) {
        return ambient;
    }

    float ddot = dot(l, norm);
    vec3 diff = _color * fmaxf(ddot, 0) * light.get_color();
    vec3 spec = vec3(1, 1, 1) * pow(fmaxf(dot(norm, normalize(view + l)), 0), 15.f) * 0.2f * light.get_color();

    vec3 final_col = ambient + diff + spec;
    final_col.r = fminf(final_col.r, 1);
    final_col.g = fminf(final_col.g, 1);
    final_col.b = fminf(final_col.b, 1);
    return final_col;
}

int Sphere::get_idx()
{
    return _idx;
}
