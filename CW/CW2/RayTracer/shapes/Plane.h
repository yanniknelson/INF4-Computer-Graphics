/*
 * Plane.h
 *
 *
 */

#include "core/Shape.h"

namespace rt{

class Plane: public Shape{

public:

	virtual ~Plane();

	Plane(Vec3f v0 = {}, Vec3f v1 = {}, Vec3f v2 = {}, Vec3f v3 = {}, Material *mat = nullptr) : v0(v0), v1(v1), v2(v2), v3(v3) { 
		std::cout << "Plane" << std::endl; 
		material = mat;
	};

	Hit intersect(Ray ray);

private: 
	Vec3f v0;
	Vec3f v1;
	Vec3f v2;
	Vec3f v3;

};



} //namespace rt


