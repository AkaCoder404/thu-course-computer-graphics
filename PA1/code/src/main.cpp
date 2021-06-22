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

    SceneParser sceneParser(inputFile.c_str());
    Camera* camera = sceneParser.getCamera();
    Group* group = sceneParser.getGroup();
    int num_lights = sceneParser.getNumLights();
    int w = camera->getWidth();
    int h = camera->getHeight();
    Image img(w, h);
    float tmin = 1e-8;
    
    cout << "casting ..." << endl;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Ray ray = camera->generateRay(Vector2f(x, y));
            //std::cout << "x: "<<x<<" y: "<<y<<" rayDir: ("<<ray.getDirection().x()<<", "<<ray.getDirection().y()<<", "<<ray.getDirection().z()<<")"<<endl;
            //std::cout << "x: "<<x<<" y: "<<y<<" rayDir: ("<<ray.getOrigin().x()<<", "<<ray.getOrigin().y()<<", "<<ray.getOrigin().z()<<")"<<endl;
            Hit hit;
            Vector3f ans(0, 0, 0);
            if (group->intersect(ray, hit, tmin)) {
                Material* material = hit.getMaterial();
                for (int i = 0; i < num_lights; i++) {
                    Light* light = sceneParser.getLight(i);
                    Vector3f dirToLight;
                    Vector3f lightColor;
                    Vector3f p = ray.pointAtParameter(hit.getT());
                    light->getIllumination(p, dirToLight, lightColor);
                    ans += material->Shade(ray, hit, dirToLight, lightColor);
                }
                //cout << "x: "<< x <<" y: " << y << " color: (" << ans.x() << ", " << ans.y() << ", " << ans.z() << ")" << endl;
                //ans = Vector3f(0.8, 0.8, 0.2);
            }
            else {
                ans = sceneParser.getBackgroundColor();
            }
            // cout << "x: "<< x <<" y: " << y << " color: (" << ans.x() << ", " << ans.y() << ", " << ans.z() << ")" << endl;
            img.SetPixel(x, y, ans);
        }
    }
    cout << "casting done" << "\n\n";
    img.SaveBMP(outputFile.c_str());
    return 0;
}

