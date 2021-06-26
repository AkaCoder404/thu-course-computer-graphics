#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>
#include <cstdio>


// TODO: Implement Group - add data structure to store a list of Object*
class Group : public Object3D {

public:

    Group() { }

    explicit Group (int num_objects) : objects(num_objects) {
        // printf("Group Size: %d\n", num_objects);
    }

    ~Group() override { }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        bool did_intersect = false;
        for (auto i : objects) {
            if (i->intersect(r, h, tmin))
                did_intersect = true;
        }
        return did_intersect;
    }

    void addObject(int index, Object3D *obj) {
        objects[index] = obj;
    }

    int getGroupSize() {
        return objects.size();
    }

private:
    std::vector<Object3D*> objects;

};

#endif
	
