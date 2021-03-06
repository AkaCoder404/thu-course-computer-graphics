#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "ray.hpp"
#include "hit.hpp"
#include <iostream>

// ray tracer
#include "texture.hpp"

// TODO: Implement Shade function that computes Phong introduced in class.
class Material {
public:
    // ray tracing
    Vector3f absorbColor;
    float diff_factor;
    float spec_factor;
    float refr_factor;
    float n;

    explicit Material(const char* filename, const Vector3f &d_color, const Vector3f &s_color = Vector3f::ZERO, const Vector3f &a_color = Vector3f::ZERO,
                      float s = 0, float diff = 1.0f, float spec = 0.0f, float refr = 0.0f, float nn = 1.5f) :
            diffuseColor(d_color), specularColor(s_color), absorbColor(a_color), shininess(s), diff_factor(diff),
            spec_factor(spec), refr_factor(refr), n(nn), texture(filename) { 

    }

    virtual ~Material() = default;

    // ray casting
    virtual Vector3f getDiffuseColor() const {
        return diffuseColor;
    }

    Vector3f getRealDiffuseColor(const Hit &hit, const Ray& ray) {
        if (texture.haveTexture()) {
            if (hit.getType() == 's') return texture.calcSphereTexture(hit.getNormal());
            else if (hit.getType() == 'r') return texture.calcRectangleTexture(ray.pointAtParameter(hit.getT()), hit.x_axis, hit.y_axis, hit.position);
            else if (hit.getType() == 'c') return texture.calcParametricTexture(hit.para_u, hit.para_v);
            else if (hit.getType() == 't') return texture.calcSimpleMeshTexture(hit.para_u, hit.para_v);
        } else {
            return diffuseColor;
        }
    }

    Vector3f Shade(const Ray &ray, const Hit &hit,
                   const Vector3f &dirToLight, const Vector3f &lightColor) {
        // PHONG model
        // Vector3f N = hit.getNormal();
        // Vector3f V = -ray.getDirection().normalized();
        // Vector3f Lx = dirToLight.normalized();
        // Vector3f Rx = (2 * (Vector3f::dot(Lx, N)) * N - Lx).normalized();
        // Vector3f shaded = lightColor * (diffuseColor * relu(Vector3f::dot(Lx,N)) + specularColor * (pow(relu(Vector3f::dot(V, Rx)), shininess)));
        
        // 
        Vector3f shaded = Vector3f::ZERO;
        Vector3f realColor = getRealDiffuseColor(hit, ray);
        
        Vector3f Rx = 2 * Vector3f::dot(dirToLight, hit.getNormal()) * hit.getNormal() - dirToLight;
        shaded = lightColor * (realColor * std::max(0.0f, Vector3f::dot(dirToLight, hit.getNormal())) + 
                               specularColor * pow(std::max(0.0f, - Vector3f::dot(ray.getDirection(), Rx)), shininess) );
        return shaded;
    }

    float relu(float x) { return std::max((float)0, x); }
    
    //  ray casting
    float BRDF(Vector3f ray_R, Vector3f N, Vector3f ray_I) {
        float ret = 0;
        ray_R = ray_R.normalized();
        ray_I = ray_I.normalized();
        
        if (diff_factor > EPS && Vector3f::dot(ray_R, N) > EPS)
            ret += diff_factor * Vector3f::dot(ray_R, N);
        
        return ret;
    }

protected:
    Vector3f diffuseColor;
    Vector3f specularColor;
    float shininess;
    Texture texture;
};


#endif // MATERIAL_H
