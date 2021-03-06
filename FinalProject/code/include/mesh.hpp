#ifndef MESH_H
#define MESH_H

#include <vector>
#include "object3d.hpp"
#include "triangle.hpp"
#include "Vector2f.h"
#include "Vector3f.h"
// ray tracer
#include "box.hpp"


class Mesh : public Object3D {

public:
    Mesh(const char *filename, Material *m, Vector3f offset, Vector3f scaling);
    ~Mesh();

    struct TriangleIndex {
        TriangleIndex() {
            x[0] = 0; x[1] = 0; x[2] = 0;
        }
        int &operator[](const int i) { return x[i]; }
        // By Computer Graphics convention, counterclockwise winding is front face
        int x[3]{};
    };

    std::vector<Vector3f> v;
    std::vector<TriangleIndex> t;
    std::vector<Vector3f> n;
    bool intersect(const Ray &r, Hit &h, float tmin) override;

private:
    // Normal can be used for light estimation
    void computeNormal();
    // ray tracing
    void createBox();
    void createTree();

    Box* box;
    TriangleTree* tree;
};

#endif
