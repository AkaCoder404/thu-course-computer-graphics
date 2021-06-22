#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "ray.hpp"
#include "hit.hpp"
#include <iostream>

// TODO: Implement Shade function that computes Phong introduced in class.
class Material {
public:

    explicit Material(const Vector3f &d_color, const Vector3f &s_color = Vector3f::ZERO, float s = 0) :
            diffuseColor(d_color), specularColor(s_color), shininess(s) {

    }

    virtual ~Material() = default;

    virtual Vector3f getDiffuseColor() const {
        return diffuseColor;
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
        Vector3f Rx = 2 * Vector3f::dot(dirToLight, hit.getNormal()) * hit.getNormal() - dirToLight;
        shaded = lightColor * (diffuseColor * std::max(0.0f, Vector3f::dot(dirToLight, hit.getNormal())) + 
                               specularColor * pow(std::max(0.0f, - Vector3f::dot(ray.getDirection(), Rx)), shininess) );
        return shaded;
    }
    float relu(float x) {
        return std::max((float)0, x);
    }

protected:
    Vector3f diffuseColor;
    Vector3f specularColor;
    float shininess;
};


#endif // MATERIAL_H
