#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// TODO (PA2): Copy from PA1

class Plane : public Object3D {
public:
    Plane() {
        this->norm = Vector3f(0, 0, 1);
        this->d = 0;
    }

    Plane(const Vector3f &normal, float d, Material *m) : Object3D(m) {
        this->norm = normal;  // plane normal of unit length
        this->d = d;          // distance from plan
    }

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        Vector3f ray_origin = r.getOrigin();
        Vector3f ray_direction = r.getDirection();

        // t = -(Pn dot Ro + D) / (Pn dot Rd)
        // t = (Pn dot Ro + D)
        // j = -(Pn dot Rd)

        // if j = 0 ray is parallel to plane
        // if j > 0 normal of plane points away from array
        float j = Vector3f::dot(this->norm, ray_direction);
        if (j == 0) return false;

        // t is equal to equals the distance of the ray from origin in World Coordinates
            // t < 0 ray intersects plane behind origin
        float t = (this->d + Vector3f::dot(this->norm, ray_origin)) / j;

        if (t < tmin || t > h.getT()) return false;
    
        if( -1.0 * j >  0) h.set(t, this->material, this->norm);
        else if ( -1.0 * j < 0) h.set(t, this->material, -1 * this->norm);

        return true;
        // return false;
    }

    void drawGL() override {
        Object3D::drawGL();
        Vector3f xAxis = Vector3f::RIGHT;
        Vector3f yAxis = Vector3f::cross(norm, xAxis);
        xAxis = Vector3f::cross(yAxis, norm);
        const float planeSize = 10.0;
        glBegin(GL_TRIANGLES);
        glNormal3fv(norm);
        glVertex3fv(d * norm + planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * norm - planeSize * xAxis - planeSize * yAxis);
        glVertex3fv(d * norm + planeSize * xAxis - planeSize * yAxis);
        glNormal3fv(norm);
        glVertex3fv(d * norm + planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * norm - planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * norm - planeSize * xAxis - planeSize * yAxis);
        glEnd();
    }

protected:
    Vector3f norm;
    float d;

};

#endif //PLANE_H
		

