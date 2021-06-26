#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "scene_parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"
// ray tracer
#include "photonmap.hpp"
#include "raytracer.hpp"
#include "photontracer.hpp"
#include "sampler.hpp"

#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

    if (argc != 3) {
        cout << "Usage: ./bin/PA1 <input scene file> <output bmp file>" << endl;
        return 1;
    }
    string inputFile = argv[1];
    string outputFile = argv[2];  // only bmp is allowed.

    // TODO: Main RayCasting Logic
    // First, parse the scene using SceneParser.
    // Then loop over each pixel in the image, shooting a ray
    // through that pixel and finding its intersection with
    // the scene.  Write the color at the intersection to that
    // pixel in your output image.

    // SceneParser sceneParser(inputFile.c_str());
    // Camera* camera = sceneParser.getCamera();
    // Group* group = sceneParser.getGroup();
    // int num_lights = sceneParser.getNumLights();
    // int w = camera->getWidth();
    // int h = camera->getHeight();
    // Image img(w, h);
    // float tmin = 1e-8;
    
    // cout << "casting ..." << endl;
    // for (int y = 0; y < h; y++) {
    //     for (int x = 0; x < w; x++) {
    //         Ray ray = camera->generateRay(Vector2f(x, y));
    //         //std::cout << "x: "<<x<<" y: "<<y<<" rayDir: ("<<ray.getDirection().x()<<", "<<ray.getDirection().y()<<", "<<ray.getDirection().z()<<")"<<endl;
    //         //std::cout << "x: "<<x<<" y: "<<y<<" rayDir: ("<<ray.getOrigin().x()<<", "<<ray.getOrigin().y()<<", "<<ray.getOrigin().z()<<")"<<endl;
    //         Hit hit;
    //         Vector3f ans(0, 0, 0);
    //         if (group->intersect(ray, hit, tmin)) {
    //             Material* material = hit.getMaterial();
    //             for (int i = 0; i < num_lights; i++) {
    //                 Light* light = sceneParser.getLight(i);
    //                 Vector3f dirToLight;
    //                 Vector3f lightColor;
    //                 Vector3f p = ray.pointAtParameter(hit.getT());
    //                 light->getIllumination(p, dirToLight, lightColor);
    //                 ans += material->Shade(ray, hit, dirToLight, lightColor);
    //             }
    //             //cout << "x: "<< x <<" y: " << y << " color: (" << ans.x() << ", " << ans.y() << ", " << ans.z() << ")" << endl;
    //             //ans = Vector3f(0.8, 0.8, 0.2);
    //         }
    //         else {
    //             ans = sceneParser.getBackgroundColor();
    //         }
    //         // cout << "x: "<< x <<" y: " << y << " color: (" << ans.x() << ", " << ans.y() << ", " << ans.z() << ")" << endl;
    //         img.SetPixel(x, y, ans);
    //     }
    // }
    // cout << "casting done" << "\n\n";

    // cout << "path tracing ..." << endl;  
    // #pragma omp parallel for
    // for (int it = 0; it < w * h; it++) {
    //     // cout << "progress: " << (float) y / h * 100 << "%\n";
    //     // for (unsigned short x = 0, Xi[3] = {y, y*y, y*y*y}; x < w; x++) {
    //         int y = it / w, x = it % w;
    //         unsigned short Xi[3] = {y, y*y, y*y*y};
    //         Vector3f ans(0, 0, 0);
    //         for (int sy = 0, i = (h-y-1) * w + x; sy < 2; sy++) {
    //             Vector3f r;
    //             for (int sx = 0; sx < 2; sx++, r = Vector3f::ZERO) {
    //                 for (int s = 0; s < samps; s++) {
    //                     double r1 = 2 * erand48(Xi), dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1); 
    //                     double r2 = 2 * erand48(Xi), dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2); 
    //                     Ray ray = camera->generateRay(Vector2f((sx + 0.5 + dx) / 2 + x, (sy + 0.5 + dy) / 2 + y));
    //                     // Ray Tracing Todo
    //                     ans += tracer.trace(ray, Xi, 1, 0) * 0.25 / samps;
    //                 }
    //             }
    //         }
    //         img.SetPixel(x, y, ans);
    //         multiThreadCounter++;
    //         cout << "progress: " << (float) multiThreadCounter / h / w * 100 << "%\n";
    //     // }
    //     if (it % 50 == 0) img.SaveBMP(argv[2]);
    // }

    // cout << "tracing done." << endl;
    // img.SaveBMP(argv[2]);

    SceneParser sceneParser(inputFile.c_str());
    cout << "parse done" << endl;
    Camera* camera = sceneParser.getCamera();
    Group* group = sceneParser.getGroup();
    int num_lights = sceneParser.getNumLights();
    int w = camera->getWidth();
    int h = camera->getHeight();
    int samps = 500; // generate n random rays at different positions for each pixel sample
    int depth = 10; // threshold for ray generation 
    Image img(w, h);
    float tmin = 1e-3;

    std::vector<Light*> lights;
    for (int i = 0; i < num_lights; i++)
        lights.emplace_back(sceneParser.getLight(i)); 
    
    RayTracer tracer(depth, group, lights, camera->getType(), sceneParser.getBackgroundColor());

    int multiThreadCounter = 0;

    cout << "SPPMing ..." << endl;

    Sampler sampler(outputFile.c_str(), lights, camera, &img, &tracer, group, w, h);
    sampler.start();

	cout << "SPPM done." << endl;
    // img.SaveBMP(outputFile.c_str());
    return 0;
}

