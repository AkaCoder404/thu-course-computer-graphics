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
    // printf("hello0\n\n");
    SceneParser sceneParser(inputFile.c_str());
    Image img = Image(sceneParser.getCamera()->getWidth(), sceneParser.getCamera()->getHeight());
    // printf("hello2.5\n\n");
    // printf("scene width %d, height %d\n", sceneParser.getCamera()->getWidth(), sceneParser.getCamera()->getHeight());
       
    for (int x = 0; x < sceneParser.getCamera()->getWidth(); ++x) {
        for (int y = 0; y < sceneParser.getCamera()->getHeight(); ++y) {
            // 计算当前像素(x,y)处相机出射光线camRay
            // implement generateRay
            // printf("hello3\n\n");
            Ray camRay = sceneParser.getCamera()->generateRay(Vector2f(x,y));
            // printf("hello4\n\n");
            // implement group
            Group* baseGroup = sceneParser.getGroup();
            Hit hit;           
            // implement intersect
            bool isIntersect = baseGroup->intersect(camRay, hit, 0);
            if(isIntersect) {
                Vector3f finalColor = Vector3f::ZERO;

                for (int li = 0; li < sceneParser.getNumLights(); ++li) {
                    Light* light = sceneParser.getLight(li);
                    Vector3f L, LightColor;
                    light->getIllumination(camRay.pointAtParameter(hit.getT()), L, LightColor);
                    // implement shade
                    finalColor += hit.getMaterial()->Shade(camRay, hit, L, LightColor);
                }
                img.SetPixel(x, y, finalColor);
            }
            else {
                img.SetPixel(x, y, sceneParser.getBackgroundColor());
            }
        }
    }

    img.SaveBMP(outputFile.c_str());
    cout << "Hello! Computer Graphics!" << endl << endl;
    return 0;
}

