/*
 * Shape.h
 *
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include "math/Transform.h"
#include "core/RayHitStructs.h"
#include "core/Material.h"

#include "rapidjson/document.h"

using namespace rapidjson;

namespace rt{

class Shape{
public:

	//
	// Constructors
	//
	Shape() {};

	//
	// Destructor (must be overriden in subclass)
	//
	virtual ~Shape();

	//
	// Shape abstract methods (to be implemented by subclasses)
	//
	virtual Hit intersect(Ray)=0;

	static Shape* createShape(Value& lightSpecs);

	bool Quadratic(float a, float b, float c, float& t0, float& t1);

	template<typename T>
	T Clamp(T a, T b, T c) {
		if (a > c) return c;
		if (a < b) return b;
		return a;
	}

	const Material getMaterial() { return (material) ? *material : Material{}; };


protected:

	Material * material = nullptr;

	Transform WorldToObject;
	Transform ObjectToWorld;

};


} //namespace rt


#endif /* SHAPE_H_ */
