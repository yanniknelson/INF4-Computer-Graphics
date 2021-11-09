/*
 * AreaLight.h
 *
 *
 */

#ifndef AREALIGHT_H_
#define AREALIGHT_H_

#include "core/LightSource.h"
#include "shapes/Plane.h"

#include <stdlib.h>
#include <time.h> 

namespace rt{

class AreaLight:public LightSource{

public:

	AreaLight(Vec3f is, Vec3f id, std::shared_ptr<Shape> area) : area(area), LightSource(Vec3f(), is, id) {
		diag = area->getWorldBounds().Diagonal();
		lowestCorner = area->getWorldBounds().pMin;
		std::srand(time(NULL));
		position = lowestCorner + (diag / 2.f);

		std::cout << "area light " << lowestCorner << " " << diag << std::endl;
	};

	Vec3f SampleLight(Hit point, void * scene);

	std::shared_ptr<Shape> area;

	Vec3f diag = {};
	Vec3f lowestCorner = {};

	float Obstruction(Hit hit, void* scene);

};



} //namespace rt




#endif /* AREALIGHT_H_ */
