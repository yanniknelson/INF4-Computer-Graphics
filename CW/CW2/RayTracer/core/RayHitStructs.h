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
	float time;
	mutable float tMax;

	Ray() : tMax(std::numeric_limits<float>::infinity()), time(0.f) {};
	Ray(const Vec3f& o, const Vec3f& d, float tMax = std::numeric_limits<float>::infinity(), float time = 0.f) : o(o), d(d), tMax(tMax), time(time) {};

	Vec3f operator()(float t) const {return o + d * t;}
};


struct Hit{

	Vec3f point; //point where ray hits a shape
	//----------Hit variables to be filled------

	bool valid = false;
	float tValue = std::numeric_limits<float>::max();
	Vec3f normal;

	//void pointer due to circular includes
	void* object;
	
	float u;
	float v;

};

}


#endif /* CORE_RAYHITSTRUCTS_H_ */
