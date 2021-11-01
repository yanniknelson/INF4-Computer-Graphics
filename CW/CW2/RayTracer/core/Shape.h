/*
 * Shape.h
 *
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#pragma once
#include "math/Transform.h"
#include "core/RayHitStructs.h"
#include "core/Material.h"
#include "materials/BlinnPhong.h"
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

	const Material* getMaterial() { return (material) ? material: nullptr; };


protected:

	Material * material = nullptr;

	Transform WorldToObject;
	Transform ObjectToWorld;

};


} //namespace rt


#endif /* SHAPE_H_ */
