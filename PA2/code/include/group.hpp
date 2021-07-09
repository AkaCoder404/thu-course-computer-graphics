#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>


// TODO (PA2): Implement Group - copy from PA1
class Group : public Object3D {

public:

    Group() { }

    explicit Group (int num_objects) : objList(num_objects) {
      printf("Group Size: %d\n", num_objects);
    }

    ~Group() override {  }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
      bool is_intersect = false;
      for (int i = 0; i < getGroupSize(); i++) {
        if (objList[i]->intersect(r, h, tmin))
          is_intersect = true;
      }
      return is_intersect;
    }

    void drawGL() override {
      for (int i = 0; i < getGroupSize(); i++) {
        if(objList[i]) {
          objList[i]->drawGL();
        }
      }

    }

    void addObject(int index, Object3D *obj) {
      objList[index] = obj;
    }

    int getGroupSize() { return (int)objList.size(); }

private:
  std::vector<Object3D*> objList; 

};

#endif
	
