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

	AreaLight(Vec3f is, Vec3f id, std::shared_ptr<Shape> a) : LightSource(Vec3f(), is, id) {
		diag = a->getWorldBounds().Diagonal();
		lowestCorner = a->getWorldBounds().pMin;
		std::srand(time(NULL));
		position = lowestCorner + (diag / 2.f);

		area = std::dynamic_pointer_cast<Plane>(a);

		basis1 = area->GetBasis(0);
		basis2 = area->GetBasis(1);

		basis1length = basis1.length();
		basis1.normalize();
		basis2length = basis2.length();
		basis2.normalize();

		std::cout << "area light " << lowestCorner << " " << diag << std::endl;
	};

	Vec3f SampleLight(Hit point, void * scene);

	std::shared_ptr<Plane> area;

	Vec3f diag = {};
	Vec3f basis1 = {};
	Vec3f basis2 = {};
	float basis1length;
	float basis2length;
	Vec3f lowestCorner = {};

	float Obstruction(Hit hit, void* scene);

};



} //namespace rt




#endif /* AREALIGHT_H_ */
