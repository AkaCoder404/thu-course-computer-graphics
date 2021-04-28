#pragma once

#include <image.hpp>
#include <algorithm>
#include <queue>
#include <cstdio>

// custom

class Element {
public:
    virtual void draw(Image &img) = 0;
    virtual ~Element() = default;
};

class Line : public Element {

public:
    int xA, yA;
    int xB, yB;
    Vector3f color;
    void draw(Image &img) override {
        // TODO: Implement Bresenham Algorithm
        if (abs(yB - yA) < abs(xB - xA)) {
            if (xA > xB) plotLineLow(xB, yB, xA, yA, img);
            else plotLineLow(xA, yA, xB, yB, img);
        }
        else {
            if (yA > yB) plotLineHigh(xB, yB, xA, yA, img);
            else plotLineHigh(xA, yA, xB, yB, img);
        }

        printf("Draw a line from (%d, %d) to (%d, %d) using color (%f, %f, %f)\n", xA, yA, xB, yB,
                color.x(), color.y(), color.z());
    }

    // decreasing slope
    void plotLineLow(int x0, int y0, int x1, int y1, Image &img) {
        int dx = x1 - x0;
        int dy = y1 - y0;
        int yi = 1;
        yi = dy < 0 ? -1 : yi;
        dy = dy < 0 ? -dy : dy;
        int D = (2 * dy) - dx;
        int y = y0;

        for (int x = x0; x <= x1; x++) {
            img.SetPixel(x, y, color);
            if (D > 0) {
                y += yi;
                D += (2 * (dy - dx));
            }
            else D += 2*dy;
        }
    }
    // increasing slope
    void plotLineHigh(int x0, int y0, int x1, int y1, Image &img) {
        int dx = x1 - x0;
        int dy = y1 - y0;
        int xi = dx < 0 ? -1 : 1;
        dx = dx < 0 ? -dx : dx;
        int D = (2 * dx) - dy;
        int x = x0;
        for (int y = y0; y <= y1; y++) {
            img.SetPixel(x, y, color);
            if (D > 0) {
                x += xi;
                D += 2 * (dx - dy);
            }
            else D +=  2 * dx;
        }
    }
};

class Circle : public Element {

public:
    int cx, cy;
    int radius;
    Vector3f color;
    void draw(Image &img) override {
        // TODO: Implement Algorithm to draw a Circle
        // midpoint method
        int x = 0;
        int y = radius;
        int d = 3 - 2 * radius;
    
        
        while (x <= y) {
            img.SetPixel(cx + x, cy + y, color);
            img.SetPixel(cx - x, cy + y, color);
            img.SetPixel(cx + x, cy - y, color);
            img.SetPixel(cx - x, cy - y, color);
            img.SetPixel(cx + y, cy + x, color);
            img.SetPixel(cx - y, cy + x, color);
            img.SetPixel(cx + y, cy - x, color);
            img.SetPixel(cx - y, cy - x, color);
            x++;
            if (d > 0) {
                y--;
                d = d + 4 * (x - y) + 10;
            }
            else d = d + 4 * x + 6;
        }
        printf("Draw a circle with center (%d, %d) and radius %d using color (%f, %f, %f)\n", cx, cy, radius,
               color.x(), color.y(), color.z());
    }
};

class Fill : public Element {

public:
    int cx, cy;
    Vector3f color;
    void draw(Image &img) override {
        // TODO: Flood fill      
        floodFill(cx, cy, img, img.GetPixel(cx, cy), color);
        printf("Flood fill source point = (%d, %d) using color (%f, %f, %f)\n", cx, cy,
                color.x(), color.y(), color.z());
    }

    bool checkValidity(int x, int y, Image &img, Vector3f oldcolor, Vector3f newcolor) {
        bool outOfBounds = false;
        if (x < 0|| y < 0 || y >= img.Height() || x >= img.Width()) outOfBounds = true;
        if (!outOfBounds && img.GetPixel(x, y) == oldcolor) {
            return true;
        }
        return false;
    } 

    void floodFill(int x, int y, Image &img, Vector3f oldcolor, Vector3f newcolor) {
        std::queue<std::pair<int, int> > q;
        // std::vector<std::pair<int, int> > q;
        std::vector<std::pair<int, int> > v;
         
        
        q.push({x, y});
        v.insert(v.begin(), {x,y});
        int x1, y1;
        while(!q.empty()) { 
            x1 = q.front().first;
            y1 = q.front().second;
            // printf("(%d,%d)", x1,y1); 
            q.pop();
            img.SetPixel(x1, y1, newcolor);

            if (checkValidity(x1+1,y1, img, oldcolor, newcolor) && find(v.begin(), v.end(), std::pair<int,int>(x1+1, y1)) == v.end()){
                v.insert(v.begin(), {x1+1,y1});
                q.push({x1+1,y1});
            }
            if (checkValidity(x1-1,y1, img, oldcolor, newcolor) && find(v.begin(), v.end(), std::pair<int,int>(x1-1, y1)) == v.end()) {
                v.insert(v.begin(), {x1-1,y1}); 
                q.push({x1-1,y1});
            }
              
            if (checkValidity(x1,y1+1, img, oldcolor, newcolor) && find(v.begin(), v.end(), std::pair<int,int>(x1, y1+1)) == v.end()){
                v.insert(v.begin(), {x1,y1+1});
                q.push({x1,y1+1});
            }

              
            if (checkValidity(x1,y1-1, img, oldcolor, newcolor) && find(v.begin(), v.end(), std::pair<int,int>(x1, y1-1)) == v.end()){
                v.insert(v.begin(), {x1,y1-1});
                q.push({x1,y1-1});
            }
        }


        // bool outOfBounds = false;
        // if (x < 0|| y < 0 || y > img.Height() || x > img.Width()) outOfBounds = true;
        // if(!outOfBounds){
        //     if(img.GetPixel(x,y) == oldcolor) {
        //         img.SetPixel(x,y,newcolor);
        //         floodFill(x+1,y,img, oldcolor, newcolor);
        //         floodFill(x,y+1,img, oldcolor, newcolor);
        //         floodFill(x-1,y,img, oldcolor, newcolor);
        //         floodFill(x,y-1,img, oldcolor, newcolor); 
        //     }
        // }
    }
};