#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// TODO: Implement functions and add more fields as necessary

class Sphere : public Object3D {

public:
    Sphere() {
        // unit ball at the center
        this->center = Vector3f(0, 0, 0);
        this->radius = 1.0;
    }

    Sphere(const Vector3f &center, float radius, Material *material) : Object3D(material) {
        this->center = center;
        this->radius = radius;
    }

    ~Sphere() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        //
        // Vector3f ray_origin = r.getOrigin();
        // Vector3f ray_direction = r.getDirection();
        
        // // l, distance from ray origin to center
        // Vector3f l = this->center - ray_origin;
        

        // // tc, distance from the origin to the a point on the ray halfway between the 2 intersection points
        // float tc = Vector3f::dot(l, ray_direction);
        // // if tc < 0, ray does not intersect the sphere(points away), thus we can exit early on
        // // if it’s not less than 0, we move on.
        // if (tc < 0.0f) return false;

        // // dot of itself is just l squared
        // float l2 = Vector3f::dot(l, l);

        // // d, distance from center to intersecting ray (perp)
        // float d2 = l2 - tc * tc;

        // float r2 = this->radius * this->radius;
        // // t1c, distance from first intersection to point between two points
        // float t2_ = r2 - d2;
        

        // // if d squared > radius squared, no intersection, can leave
        // if( d2 >= r2) return false;


        // // intersection points
        // // t1 = tc - t1c, t2 = tc + t1c
        // // P1 = Origin + Direction * t1, P2 = Origin + Direction * t2

        // float t;
        // float t1c = sqrtf(t2_);
        // Vector3f normal;

        // if (l2 > r2) {
        //     t = tc - t1c;
        //     normal = (r.pointAtParameter(t) - this->center).normalized();
        // }
        // else if (l2 <= r2) {
        //     t = tc + t1c;
        //     normal = -1 * (r.pointAtParameter(t) - this->center).normalized();
        // }

        // if (t < tmin || t > h.getT()) return false;

        // h.set(t, this->material, normal);

        // fix sphere test case
        // Vector3f normal = Vector3f::cross(r.getDirection(), center - r.getOrigin());
        // Vector3f vertical = Vector3f::cross(normal, r.getDirection()).normalized();
        
        // float len1 = ( ( r.getOrigin().x() * r.getDirection().y() / r.getDirection().x() - r.getOrigin().y()) - 
        //                ( center.x()        * r.getDirection().y() / r.getDirection().x() - center.y()) )
        //              / ( vertical.x()      * r.getDirection().y() / r.getDirection().x() - vertical.y());
        // float len2 = ( center.x() - r.getOrigin().x() + len1 * vertical.x() ) / r.getDirection().x();

        // if (r.getOrigin().x() == r.getOrigin().y()) {
        //     if (fabs(r.getDirection().y()) > 1e-6) {
        //         len1 = ( ( r.getOrigin().y() * r.getDirection().z() / r.getDirection().y() - r.getOrigin().z()) - 
        //                ( center.y()        * r.getDirection().z() / r.getDirection().y() - center.z()) )
        //              / ( vertical.y()      * r.getDirection().z() / r.getDirection().y() - vertical.z());
        //         len2 = ( center.y() - r.getOrigin().y() + len1 * vertical.y() ) / r.getDirection().y();
        //     }
        //     else if (fabs(r.getDirection().x()) > 1e-6) {
        //         len1 = ( ( r.getOrigin().x() * r.getDirection().z() / r.getDirection().x() - r.getOrigin().z()) - 
        //                ( center.x()        * r.getDirection().z() / r.getDirection().x() - center.z()) )
        //              / ( vertical.x()      * r.getDirection().z() / r.getDirection().x() - vertical.z());
        //         len2 = ( center.x() - r.getOrigin().x() + len1 * vertical.x() ) / r.getDirection().x();
        //     }
        //     else {
        //         if (fabs(vertical.x()) > 1e-6) {
        //             len1 = -center.x() / vertical.x();
        //             len2 = ( center.z() - r.getOrigin().z() + len1 * vertical.z() ) / r.getDirection().z();
        //         }
        //         else {
        //             len1 = 0.0f;
        //             len2 = ( center.z() - r.getOrigin().z() ) / r.getDirection().z();
        //             vertical = Vector3f::ZERO;
        //         }
        //     }
        // }

        

        // float len_CH = (len1 * vertical).length();

        

        // if (len_CH > radius) return false;

        // float len_OH = (len2 * r.getDirection()).length();
        // float t = len_OH - sqrt(pow(radius, 2) - pow(len_CH, 2));

        // if (t < tmin) return false;
        // if (t > h.getT()) return false;

        // Vector3f normal_P = (r.getOrigin() + t * r.getDirection().normalized() - center).normalized();
        // h.set(t, material, normal_P);




        // ray tracer
        Vector3f p = r.getOrigin() - center;
        float b = Vector3f::dot(p, r.getDirection()) * -1.0;
        float det = b * b - p.length() * p.length() + radius * radius;
        float t;
        bool front;

        if (det > 1e-3) {
            det = sqrt(det);
            float x1 = b - det, x2 = b + det;
            if (x2 < 1e-3) return false;
            if (x1 > 1e-3) {
                t = x1;
                front = true;
            }
            else {
                t = x2;
                front = false;
            }
        }
        else {
            return false;
        }

        if (t < tmin) return false;
        if (t > h.getT()) return false;

        Vector3f normal_P = (r.getOrigin() + r.getDirection() * t - center).normalized();
        if (!front) normal_P = -normal_P;
        h.set(t, material, normal_P, 's', front);
        return true;
    }

protected:

    Vector3f center;
    float radius;

};


#endif
