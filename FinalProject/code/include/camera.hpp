#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include <vecmath.h>
#include <float.h>
#include <cmath>


class Camera {
public:
    Camera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH) {
        this->center = center;
        this->direction = direction.normalized();
        this->horizontal = Vector3f::cross(this->direction, up);
		this->horizontal.normalize();
        this->up = Vector3f::cross(this->horizontal, this->direction);
        this->width = imgW;
        this->height = imgH;
    }

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(const Vector2f &point) = 0;
    virtual ~Camera() = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

protected:
    // Extrinsic parameters
    Vector3f center;
    Vector3f direction;
    Vector3f up;
    Vector3f horizontal;
    // Intrinsic parameters
    int width;
    int height;
};

// TODO: Implement Perspective camera
// You can add new functions or variables whenever needed.
class PerspectiveCamera : public Camera {
    /*  scaled from image space to real world space, in other words, 
        the corresponding length of the distance of a pixel in the image in the real scene 
    */
public:
    float fx;           
    float fy;
    float fz;
    // optical center position
    float cx;          
    float cy;          
    float degree;

public:
    PerspectiveCamera(const Vector3f &center, const Vector3f &direction,
            const Vector3f &up, int imgW, int imgH, float angle) : Camera(center, direction, up, imgW, imgH) {
        // angle is in radian.
        // printf("perspective camera\n");
        // printf("camera position: \ncenter : (%f, %f, %f)\ndirection : (%f, %f, %f)\n", center.x(), center.y(), center.z(), direction.x(), direction.y(), direction.z());
        // printf("angle : %f\n", angle);
        // printf("imgW, imgH : (%d, %d)\n", imgW, imgH);
    
        degree = angle / M_PI * 180.0;
        // DegreesToRadians(angle);
        // printf("hello1\n\n");
        // spatial scaling factor
        fx = (float) width / (2 * tanf(angle / 2));
        fy = (float) height / (2 * tanf(angle / 2));
        // optical center position
        cx = width / 2.0f;
        cy = height / 2.0f;
        // printf("hello2\n\n");

    }

    Ray generateRay(const Vector2f &point) override {
        // TODO
        // printf("generating ray\n\n");
        // printf("scene width %d, height %d\n",getWidth(), getHeight());

        // ray in camera space
        float csx = (point.x() - cx) / fx;
        float csy = (point.y() - cy) / fy;

        // direction of ray
        Vector3f direction(csx, -csy, 1.0f);
        Vector3f origin(this->center);

        // coordinate transformation
        Matrix3f R(this->horizontal, -1 * this->up, this->direction);
        direction = R * direction;                    // transform
        direction = direction / direction.length();   // normalize


        // create ray
        Ray ray(origin, direction);
        return ray;

        
    }
};

#endif //CAMERA_H
