#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>
using namespace std;

// TODO: implement this class and add more fields as necessary,
class Triangle: public Object3D {

public:
	Triangle() = delete;

    // a b c are three vertex positions of the triangle
	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m) : Object3D(m) {
		this->vertices[0] = this->a = a;
		this->vertices[1] = this->b = b;
		this->vertices[2] = this->c = c;
		// calculate triangles normal
		// this->normal = Vector3f::cross(a,b).normalized();
		// ray tracing
		this->normal = Vector3f::cross(b - a, c - a).normalized();
		d = Vector3f::dot(normal, a);
	}

	bool intersect(const Ray& ray,  Hit& hit , float tmin) override {
		// 
 		// Vector3f ray_origin = ray.getOrigin();
		// Vector3f ray_direction = ray.getDirection();

		// float j = Vector3f::dot(this->normal, ray_direction);
		// // if ray is parallel to plane
		// if (j == 0) return false;

		// Vector3f edge1 = this->vertices[0] - this->vertices[1];
		// Vector3f edge2 = this->vertices[0] - this->vertices[2];
		// Vector3f s = this->vertices[0] - ray_origin;

		// float determinant = Matrix3f(ray_direction, edge1, edge2).determinant();

		// float t = Matrix3f(s, edge1, edge2).determinant() / determinant;
		// float beta = Matrix3f(ray_direction, s, edge2).determinant() / determinant;
		// float gamma = Matrix3f(ray_direction, edge1, s).determinant() / determinant;

		// if (t < tmin || t >= hit.getT()) return false;
		
		// // triangle's center of gravity
		// else if (beta < 0 || beta > 1 || gamma < 0  || gamma > 1 || beta + gamma > 1) 
		// 	return false;

		// if (j < 0)
		// 	hit.set(t, this->material, normal);
		// else if (j > 0)
		// 	hit.set(t, this->material, -1 * normal);
		
		if (fabs(Vector3f::dot(ray.getDirection(), normal)) < 1e-3) return false;
        float t = (d - Vector3f::dot(normal, ray.getOrigin())) / Vector3f::dot(normal, ray.getDirection());

        if (t < tmin) return false;
        if (t > hit.getT()) return false;

		Vector3f p = ray.getOrigin() + t * ray.getDirection();
		Vector3f n1 = Vector3f::cross(vertices[1] - p, vertices[2] - p);
		Vector3f n2 = Vector3f::cross(vertices[0] - p, vertices[1] - p);
		Vector3f n3 = Vector3f::cross(vertices[2] - p, vertices[0] - p);
		
		if (Vector3f::dot(normal, n1) < 0) return false;
		if (Vector3f::dot(normal, n2) < 0) return false;
		if (Vector3f::dot(normal, n3) < 0) return false;

		bool front = (Vector3f::dot(normal, ray.getDirection()) < 0);

		if (!front) hit.set(t, material, -normal, 't', front);
		else hit.set(t, material, normal, 't', front);

		Vector3f po = ray.pointAtParameter(hit.getT());
		hit.setParametricParameters(fmod((po.x() + po.y()) / 100, 1),
								  fmod((po.y() + po.z()) / 100, 1));
		
		return true;
		
		
		// Möller–Trumbore intersection algorithm 
		// faster algroithm for calculating intersection between ray and 
		// Vector3f edge1, edge2, h, s, q;

		// const float EPSILON = tmin;
		// float a,f,u,v;
		// edge1 = this->vertices[1] - this->vertices[0];
		// edge2 = this->vertices[2] - this->vertices[0];
		// h = Vector3f::cross(ray_direction, edge2);
		// a = Vector3f::dot(edge1, h);
		// if (a > -EPSILON && a < EPSILON)
		// 	return false;    
		// f = 1.0/a;
		// s = ray_origin - this->vertices[0];
		// u = f* Vector3f::dot(s, h);
		// if (u < 0.0 || u > 1.0)
		// 	return false;
		// q = Vector3f::cross(s, edge1);
		// v = f * Vector3f::dot(ray_direction, q);
		// if (v < 0.0 || u + v > 1.0)
		// 	return false;
		// // At this stage we can compute t to find out where the intersection point is on the line.
		// float t = f * Vector3f::dot(edge2, q);
		// if (t > EPSILON && t <= hit.getT()) // ray intersection
		// {
		// 	// outIntersectionPoint = rayOrigin + rayVector * t;
		// 	if (j < 0)
		// 		hit.set(t, this->material, normal);
		// 	else if (j > 0)
		// 		hit.set(t, this->material, -1 * normal);
		// 	return true;
		// }
		// else // This means that there is a line intersection but not a ray intersection.
		// 	return false;

	}

	float getMin(int axis) {
		int x0 = vertices[0][axis], x1 = vertices[1][axis], x2 = vertices[2][axis];
		return std::min(x0, std::min(x1, x2));
  }

	float getMax(int axis) {
		int x0 = vertices[0][axis], x1 = vertices[1][axis], x2 = vertices[2][axis];
		return std::max(x0, std::max(x1, x2));
  }
	Vector3f normal;
	Vector3f vertices[3];
protected:
	Vector3f a, b, c;
	float d;

};

#endif //TRIANGLE_H
