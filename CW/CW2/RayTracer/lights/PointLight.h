/*
 * PointLight.h
 *
 *
 */

#ifndef POINTLIGHT_H_
#define POINTLIGHT_H_

#include "core/LightSource.h"


namespace rt{

class PointLight: public LightSource{

public:
	PointLight();
	PointLight(Vec3f position = {}, Vec3f is = {}, Vec3f id = {}):LightSource(position, is, id){}

	Vec3f SampleLight(Vec3f point);

};



} //namespace rt




#endif /* POINTLIGHT_H_ */
