/*
 * Plane.h
 *
 *
 */

#include "core/Shape.h"
#include "shapes/Triangle.h"
#include "shapes/BVH.h"

namespace rt{

class Plane: public Shape{

public:

	virtual ~Plane();

	Plane(Vec3f v0 = {}, Vec3f v1 = {}, Vec3f v2 = {}, Vec3f v3 = {}, Material *mat = nullptr) : v0(v0), v1(v1), v2(v2), v3(v3) { 
		std::cout << "Plane" << std::endl; 
		material = mat;

		if (v0.x != v1.x && v0.y != v1.y) {
			std::swap(v1, v2);
		} else if (v0.x != v3.x && v0.y != v3.y) {
			std::swap(v3, v2);
		}

		t1 = Triangle(v0, v1, v2, mat);
		t2 = Triangle(v0, v3, v2, mat);


		ObjectBound = Bounds3f(v0, v2);
		WorldBound = ObjectBound;


		centroid = (v0 + v2) / 2;

		name = "plane";
	};

	Hit intersect(Ray ray);

	Vec3f GetBasis(bool axis) {
		return (axis) ? (v1 - v0) : (v3 - v0);
	}

private: 
	Triangle t1;
	Triangle t2;
	Vec3f v0;
	Vec3f v1;
	Vec3f v2;
	Vec3f v3;

};



} //namespace rt


