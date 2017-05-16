/*
  CPE 471 Lab 1 
  Base code for Rasterizer
  Example code using B. Somers' image code - writes out a sample tga
*/

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <tuple>
#include <vector>

#include "../bitmap/bitmap_image.hpp"
#include "Camera.h"
#include "Light.h"
#include "Sphere.h"
#include "types.h"
#include "rapidjson\document.h"
#include "rapidjson\filereadstream.h"

using namespace glm;
using namespace rapidjson;
using namespace std;

int height, width;
float near, fov;

class PointComp {
    vec3 _cam;

public:
    PointComp(vec3 cam) {
        _cam = cam;
    }

    bool operator() (tuple<vec3, Sphere> t1, tuple<vec3, Sphere> t2) {      
        return glm::distance(get<0>(t1), _cam) < glm::distance(get<0>(t2), _cam);
    }
};

vector<Sphere> parse_spheres(const string& filename) {
    FILE* fp;
    fopen_s(&fp, filename.c_str(), "r");

    char buff[65536];
    FileReadStream is(fp, buff, sizeof(buff));

    Document doc;
    doc.ParseStream(is);

    vector<Sphere> spheres = vector<Sphere>();
    const Value& sphere_list = doc["spheres"];
    for (int i = 0; i < sphere_list.Size(); i++) {
        const Value& sphere = sphere_list[i];
        spheres.push_back(Sphere(i, sphere));
    }

    fclose(fp);

    return spheres;
}

Camera parse_camera(const string& filename) {
    FILE* fp;
    fopen_s(&fp, filename.c_str(), "r");

    char buff[65536];
    FileReadStream is(fp, buff, sizeof(buff));

    Document doc;
    doc.ParseStream(is);

    const Value& camera = doc["camera"];

    const Value& pos = camera["pos"];
    vec3 position = vec3(pos["x"].GetDouble(), pos["y"].GetDouble(), pos["z"].GetDouble());

    const Value& look = camera["look_at"];
    vec3 look_at = vec3(look["x"].GetDouble(), look["y"].GetDouble(), look["z"].GetDouble());

    fclose(fp);

    return Camera(position, look_at - position);
}

Light parse_light(const string& filename) {
    FILE* fp;
    fopen_s(&fp, filename.c_str(), "r");

    char buff[65536];
    FileReadStream is(fp, buff, sizeof(buff));

    Document doc;
    doc.ParseStream(is);

    const Value& light = doc["light"];

    const Value& pos = light["pos"];
    vec3 position = vec3(pos["x"].GetDouble(), pos["y"].GetDouble(), pos["z"].GetDouble());

    const Value& col = light["color"];
    vec3 color = vec3(col["r"].GetDouble(), col["g"].GetDouble(), col["b"].GetDouble());

    fclose(fp);

    return Light(position, color);
}

vec3 parse_background(const string& filename) {
    FILE* fp;
    fopen_s(&fp, filename.c_str(), "r");

    char buff[65536];
    FileReadStream is(fp, buff, sizeof(buff));

    Document doc;
    doc.ParseStream(is);

    const Value& background = doc["background"];

    vec3 color = vec3(background["r"].GetDouble(), background["g"].GetDouble(), background["b"].GetDouble());

    fclose(fp);

    return color;
}

int main(int argc, char* argv[]) {

    if (argc < 2 ) {
        cout << "Usage: ray_tracer spherefile" << endl;
        exit(EXIT_FAILURE);
    }

    width = 1280;
    height = 720;
    near = .01;
    fov = 45;
    vector<Sphere> spheres = parse_spheres(argv[1]);

    vec3 background = parse_background(argv[1]);

    // make a 640x480 image (allocates buffer on the heap)
    bitmap_image image(width, height);
    Camera cam = parse_camera(argv[1]);

    Ray first = cam.cast_ray(0, 0, width, height, near, fov);

    float factor = std::fmaxf(width, height);
    vec3 right = near * tan(fov) * cam.get_right() * 2.0f / (float)factor;
    vec3 up = near * tan(fov) * cam.get_up() * 2.0f / (float)factor;

    Light light = parse_light(argv[1]);

    // set a square to be the color above
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            vec3 new_pos = vec3(first.get_pos() + (float) i * right + (float) j * up);
            vec3 new_dir = normalize(new_pos - cam.get_pos());
            Ray ray = Ray(new_pos, new_dir);

            vector<tuple<vec3, Sphere>> points = vector<tuple<vec3, Sphere>>();

            for (int s = 0; s < spheres.size(); s++) {
                vector<vec3> pts = spheres[s].get_intersections(ray);
                for (int p = 0; p < pts.size(); p++) {
                    points.push_back(make_tuple(pts[p], spheres[s]));
                }
            }
            vec3 color = background;

            if (points.size() > 0) {
                sort(points.begin(), points.end(), PointComp(cam.get_pos()));
                Sphere sphere = get<1>(points[0]);

                vector<tuple<vec3, Sphere>> lights = vector<tuple<vec3, Sphere>>();
                for (int s = 0; s < spheres.size(); s++) {
                    vector<vec3> pts = spheres[s].get_intersections(Ray(light.get_pos(), light.get_pos() - get<0>(points[0])));
                    for (int p = 0; p < pts.size(); p++) {
                        lights.push_back(make_tuple(pts[p], spheres[s]));
                    }
                }        
                bool has_light = true;

                if (lights.size() > 0) {
                    sort(lights.begin(), lights.end(), PointComp(light.get_pos()));
                    has_light = get<1>(points[0]).get_idx() == get<1>(lights[0]).get_idx();
                }

                color = sphere.get_color(light, cam.get_pos(), get<0>(points[0]), has_light);
            }

            image.set_pixel(i, height - j - 1, color.r * 255, color.g * 255, color.b * 255);
        }
    }

    // write the targa file to disk
    image.save_image("awesome.bmp");
  // true to scale to max color, false to clamp to 1.0

    return 0;
}
