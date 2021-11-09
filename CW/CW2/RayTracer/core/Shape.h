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

	static std::shared_ptr<Shape> createShape(Value& lightSpecs);

	bool Quadratic(float a, float b, float c, float& t0, float& t1);

	const Material* getMaterial() { return (material) ? material: nullptr; };

	const bool IntersectsBounds(Ray& ray) const;

	const Bounds3f getWorldBounds() const {
		return WorldBound;
	}

	const Vec3f getCentroid() const {
		return centroid;
	}

	std::string name;
protected:

	Material * material = nullptr;

	

	Vec3f centroid = {};

	Transform WorldToObject;
	Transform ObjectToWorld;
	Bounds3f ObjectBound;
	Bounds3f WorldBound;

};


} //namespace rt


#endif /* SHAPE_H_ */
