/*
 * BVH.h
 *
 *
 */

#ifndef BVH_H_
#define BVH_H_

#include "core/Shape.h"

#include <algorithm>
#include <vector>

namespace rt{

class BVH: public Shape{


public:
    BVH();

	BVH(Shape* shapes, int NumOfShapes) {
		
	};

	struct Node {
		Bounds3f bounds;
		Node* children[2];
        bool isInterior = false;
		int splitAxis, numOfShapes;
        std::vector<std::shared_ptr<Shape>> shapes;

        void makeLeaf(std::shared_ptr<Shape> shape) {
            shapes.push_back(shape);
            bounds = shape->getWorldBounds();
            numOfShapes = 1;
            children[0] = children[1] = nullptr;
        }

        void makeLeaf(std::vector<std::shared_ptr<Shape>> inputshapes, Bounds3f inputbounds) {
            shapes = inputshapes;
            bounds = inputbounds;
            numOfShapes = shapes.size();
            children[0] = children[1] = nullptr;
        }

        void makeInterior(int axis, int n, Node* c0, Node* c1) {
            children[0] = c0;
            children[1] = c1;
            bounds = bounds.Union(c0->bounds, c1->bounds);
            splitAxis = axis;
            numOfShapes = n;
            isInterior = true;
        }

        Hit Intersect(Ray r) {
            float hit1, hit2;
            Hit ret;
            if (isInterior) {
                if (bounds.IntersectP(r, &hit1, &hit2)) {
                    Hit c1 = children[0]->Intersect(r);
                    Hit c2 = children[1]->Intersect(r);
                    if (c2.valid && c2.tValue < c1.tValue) {
                        std::swap(c1, c2);
                    }
                    return c1;
                } else {
                    return ret;
                }
            } else {

                Hit ret;

                for (int i = 0; i < numOfShapes; i++) {
                    Hit t = shapes[i]->intersect(r);
                    if (t.valid && t.tValue < ret.tValue && t.tValue > 1e-5) {
                        //std::cout << ((Shape*)t.object)->name << std::endl;
                        ret = t;
                    }
                }
                return ret;
            }

            return ret;
        }

        Hit IntersectLeaveOut(Ray r, Hit h) {
            float hit1, hit2;
            Hit ret;
            if (isInterior) {
                if (bounds.IntersectP(r, &hit1, &hit2)) {
                    Hit c1 = children[0]->IntersectLeaveOut(r, h);
                    Hit c2 = children[1]->IntersectLeaveOut(r, h);
                    if (c2.valid && c2.tValue < c1.tValue) {
                        std::swap(c1, c2);
                    }
                    return c1;
                }
                else {
                    return ret;
                }
            }
            else {

                Hit ret;

                for (int i = 0; i < numOfShapes; i++) {
                    Hit t = shapes[i]->intersect(r);
                    if (t.valid && t.tValue < ret.tValue && t.object != h.object  && t.tValue > 1e-5) {
                        ret = t;
                    }
                }
                return ret;
            }

            return ret;
        }

	};

	static Node *BuildTree(std::vector<std::shared_ptr<Shape>> shapes);

};



} //namespace rt



#endif /* BVH_H_ */
