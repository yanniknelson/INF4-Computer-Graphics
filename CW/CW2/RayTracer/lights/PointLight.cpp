/*
 * PointLight.cpp
 *
 *
 */
#include "PointLight.h"



namespace rt{


	Vec3f PointLight::SampleLight(Vec3f point) {
	
		float dist = (point - position).length();
		return is / (4 * M_PI * dist * dist);

	}


} //namespace rt
