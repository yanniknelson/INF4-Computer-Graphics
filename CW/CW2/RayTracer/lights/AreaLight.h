/*
 * AreaLight.h
 *
 *
 */

#ifndef AREALIGHT_H_
#define AREALIGHT_H_

#include "core/LightSource.h"

namespace rt{

class AreaLight:public LightSource{

	Vec3f SampleLight(Vec3f point);

};



} //namespace rt




#endif /* AREALIGHT_H_ */
