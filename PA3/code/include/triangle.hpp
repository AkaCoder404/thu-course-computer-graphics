#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>

using namespace std;

// TODO (PA2): Copy from PA1
class Triangle: public Object3D
{

public:
	Triangle() = delete;
        ///@param a b c are three vertex positions of the triangle

	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m) : Object3D(m) {
        this->vertices[0] = this->a = a;
		this->vertices[1] = this->b = b;
		this->vertices[2] = this->c = c;
		// calculate triangles normal
		this->normal = Vector3f::cross(a,b).normalized();
	    
	}

	bool intersect( const Ray& ray,  Hit& hit , float tmin) override {
         Vector3f ray_origin = ray.getOrigin();
		Vector3f ray_direction = ray.getDirection();

		float j = Vector3f::dot(this->normal, ray_direction);
		// if ray is parallel to plane
		if (j == 0) return false;

		Vector3f edge1 = this->vertices[0] - this->vertices[1];
		Vector3f edge2 = this->vertices[0] - this->vertices[2];
		Vector3f s = this->vertices[0] - ray_origin;

		float determinant = Matrix3f(ray_direction, edge1, edge2).determinant();

		float t = Matrix3f(s, edge1, edge2).determinant() / determinant;
		float beta = Matrix3f(ray_direction, s, edge2).determinant() / determinant;
		float gamma = Matrix3f(ray_direction, edge1, s).determinant() / determinant;

		if (t < tmin || t >= hit.getT()) return false;
		
		// triangle's center of gravity
		else if (beta < 0 || beta > 1 || gamma < 0  || gamma > 1 || beta + gamma > 1) 
			return false;

		if (j < 0)
			hit.set(t, this->material, normal);
		else if (j > 0)
			hit.set(t, this->material, -1 * normal);
		return true;
        return false;
	}
	Vector3f normal;
	Vector3f vertices[3];

    void drawGL() override {
        Object3D::drawGL();
        glBegin(GL_TRIANGLES);
        glNormal3fv(normal);
        glVertex3fv(vertices[0]); glVertex3fv(vertices[1]); glVertex3fv(vertices[2]);
        glEnd();
    }

protected:
    Vector3f a, b, c;
};


#endif //TRIANGLE_H
