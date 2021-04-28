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
        // PHONG

        Vector3f shaded = Vector3f::ZERO;       
        Vector3f n = hit.getNormal(), drw = ray.getDirection();
        Vector3f r = 2 * Vector3f::dot(n, dirToLight) * n - dirToLight;

        shaded += this->diffuseColor * fmax(0.0f, Vector3f::dot(dirToLight, n));
        shaded += this->specularColor * pow(fmax(0.0f, Vector3f::dot(-1 * drw, r)), shininess);
        return shaded * lightColor;

        // return shaded;
    }

protected:
    Vector3f diffuseColor;
    Vector3f specularColor;
    float shininess;
};


#endif // MATERIAL_H
