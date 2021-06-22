#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// TODO: Implement Plane representing an infinite plane
// function: ax+by+cz=d
// choose your representation , add more fields and fill in the functions

class Plane : public Object3D {
public:
    Plane() {
        this->normal = Vector3f(0, 0, 1);
        this->d = 0;

    }

    Plane(const Vector3f &normal, float d, Material *m) : Object3D(m) {
        this->normal = normal;  // plane normal of unit length
        this->d = d;

    }

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        Vector3f ray_origin = r.getOrigin();
        Vector3f ray_direction = r.getDirection();

        // t = -(Pn dot Ro + D) / (Pn dot Rd)
        // t = (Pn dot Ro + D)
        // j = -(Pn dot Rd)

        // if j = 0 ray is parallel to plane
        //     if j > 0 normal of plane points away from array
        float j = Vector3f::dot(this->normal, ray_direction);
        if (j == 0) return false;

        // t is equal to equals the distance of the ray from origin in World Coordinates
            // t < 0 ray intersects plane behind origin
        float t = (this->d + Vector3f::dot(this->normal, ray_origin)) / j;

        if (t < tmin || t > h.getT()) return false;
    
        if( -1.0 * j >  0) h.set(t, this->material, this->normal);
        else if ( -1.0 * j < 0) h.set(t, this->material, -1 * this->normal);

        return true;
    }

protected:
    Vector3f normal; 
    float d;


};

#endif //PLANE_H
		

