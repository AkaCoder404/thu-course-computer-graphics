#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <glut.h>

// TODO (PA2): Copy from PA1

class Sphere : public Object3D {
public:
    Sphere() {
        // unit ball at the center
        this->center = Vector3f(0, 0, 0);
        this->radius = 1.0;
        
    }

    Sphere(const Vector3f &center, float radius, Material *material) : Object3D(material) {
        // 
        // printf("sphere\n");
        this->center = center;
        this->radius = radius;
    }

    ~Sphere() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        Vector3f ray_origin = r.getOrigin();
        Vector3f ray_direction = r.getDirection();
        
        // l, distance from ray origin to center
        Vector3f l = this->center - ray_origin;
        

        // tc, distance from the origin to the a point on the ray halfway between the 2 intersection points
        float tc = Vector3f::dot(l, ray_direction);
        // if tc < 0, ray does not intersect the sphere(points away), thus we can exit early on
        // if it’s not less than 0, we move on.
        if (tc < 0.0f) return false;

        // dot of itself is just l squared
        float l2 = Vector3f::dot(l, l);

        // d, distance from center to intersecting ray (perp)
        float d2 = l2 - tc * tc;

        float r2 = this->radius * this->radius;
        // t1c, distance from first intersection to point between two points
        float t2_ = r2 - d2;
        
        // if d squared > radius squared, no intersection, can leave
        if( d2 >= r2) return false;

        // intersection points
        // t1 = tc - t1c, t2 = tc + t1c
        // P1 = Origin + Direction * t1, P2 = Origin + Direction * t2

        float t;
        float t1c = sqrtf(t2_);
        Vector3f normal;

        if (l2 > r2) {
            t = tc - t1c;
            normal = (r.pointAtParameter(t) - this->center).normalized();
        }
        else if (l2 <= r2) {
            t = tc + t1c;
            normal = -1 * (r.pointAtParameter(t) - this->center).normalized();
        }

        if (t < tmin || t > h.getT()) return false;

        h.set(t, this->material, normal);
        return true;
    }

    void drawGL() override {
        Object3D::drawGL();
        glMatrixMode(GL_MODELVIEW); glPushMatrix();
        glTranslatef(center.x(), center.y(), center.z());
        glutSolidSphere(radius, 80, 80);
        glPopMatrix();
    }

protected:
    Vector3f center;
    float radius;

};


#endif
