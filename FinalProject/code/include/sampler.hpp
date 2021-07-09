#ifndef SAMPLER_H
#define SAMPLER_H

#include <cmath>
#include <vector>
#include "ray.hpp"
#include "group.hpp"
#include "light.hpp"
#include "photonmap.hpp"
#include "hitpointmap.hpp"
#include "raytracer.hpp"
#include "photontracer.hpp"
#include "camera.hpp"
#include "image.hpp"
#include <vecmath.h>

#define EPS 1e-3

class Sampler
{
public:
    Sampler(std::string outputFile,std::vector<Light*>& l, Camera* c, Image* i, RayTracer* r, Group* g, int ww, int hh) : 
            outputFile(outputFile), lights(l), img(i), group(g), tracer(r), camera(c), w(ww), h(hh), dof_sample(16), iterations(64), aperture(0) {}
    ~Sampler() {}

    void start();
    void sampling();
    void randomsampling();
    void resampling();
    void ProgressivePhotonMapping(int SPPMIter);

private:
    Camera* camera;
    Image* img;
    Image* vol;
    Group* group;
    RayTracer* tracer;
    HitpointMap* hitpointMap;
    std::vector<Light*>& lights;

    std::string outputFile;
    int w, h;
    int dof_sample; // depth of field samples
    int iterations;
    float aperture;
};

#endif //SAMPLER_H
