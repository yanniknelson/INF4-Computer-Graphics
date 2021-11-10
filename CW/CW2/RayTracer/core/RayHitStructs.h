/*
 * RayHitStructs.h
 */

#ifndef CORE_RAYHITSTRUCTS_H_
#define CORE_RAYHITSTRUCTS_H_

#include "math/geometry.h"

namespace rt{

/*
 * Ray structure definition
 */
enum RayType {PRIMARY, SECONDARY, SHADOW};

struct Ray {
public:
	RayType raytype;

	//----------Ray variables to be filled------
	Vec3f o;
	Vec3f d;
	float medium = 1.f;
	mutable float tMax;

	Ray() : tMax(std::numeric_limits<float>::infinity()) {};
	Ray(const Vec3f& o, const Vec3f& d, float tMax = std::numeric_limits<float>::infinity(), float time = 0.f) : o(o), d(d), tMax(tMax) {};

	Vec3f operator()(float t) const {return o + d * t;}
};


struct Hit{

	Vec3f point; //point where ray hits a shape
	//----------Hit variables to be filled------

	bool valid = false;
	float tValue = std::numeric_limits<float>::max();
	float t1 = std::numeric_limits<float>::max();
	Vec3f normal;
	float medium = 1.f;
	bool interior = false;

	//void pointer due to circular includes
	void* object;

	void* TrimeshTri = nullptr;
	
	float u;
	float v;

};

}


#endif /* CORE_RAYHITSTRUCTS_H_ */
