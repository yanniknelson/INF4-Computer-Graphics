/*
 * Triangle.h
 *
 *
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "core/Shape.h"

namespace rt{

class Triangle: public Shape{

public:

	virtual ~Triangle();

	Triangle(Vec3f v0 = {}, Vec3f v1 = {}, Vec3f v2 = {}, Material *mat = nullptr) : v0(v0), v1(v1), v2(v2) { 
		std::cout << "Triangle" << std::endl; 
		material = mat;
		Vec3f avg = ((v0 + v1 + v2)/3).normalized();
		std::cout << intersect(Ray(Vec3f{}, avg)).tValue << std::endl;

	};

	Hit intersect(Ray ray);

private: 
	Vec3f v0;
	Vec3f v1;
	Vec3f v2;
};



} //namespace rt




#endif /* TRIANGLE_H_ */
