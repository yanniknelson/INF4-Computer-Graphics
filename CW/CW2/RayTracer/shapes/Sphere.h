/*
 * Sphere.h
 *
 *
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "math/geometry.h"
#include "core/RayHitStructs.h"
#include "core/Shape.h"

namespace rt{

class Sphere:public Shape{

public:

	//
	// Constructors
	//
	Sphere() {};
	Sphere(Vec3f center = {}, float radius = 0.f, Material *mat = nullptr) : center(center), radius(radius) {
		ObjectToWorld = Transform::Translate(center);
		WorldToObject = ObjectToWorld.Inverse();
		material = mat;
	};

	virtual ~Sphere();


	//
	// Functions that need to be implemented, since Sphere is a subclass of Shape
	//
	Hit intersect(Ray ray);

	

private:

	Vec3f center;
	float radius;

};



} //namespace rt




#endif /* SPHERE_H_ */
