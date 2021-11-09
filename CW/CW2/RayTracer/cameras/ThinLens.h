/*
 * ThinLens.h
 *
 */

#ifndef THINLENS_H_
#define THINLENS_H_

#include "core/Camera.h"
#include <stdlib.h>
#include <time.h> 


namespace rt{

class ThinLens:public Camera{
public:

	//
	// Constructors
	//
	ThinLens():Camera(){};
	ThinLens(int width, int height, int fov, float lensRadius, float focalDistance, Vec3f pos, Vec3f up, Vec3f lookAt);

	Vec2f sampleLens();

	//
	//Destructor
	//
	~ThinLens(){};

	//
	// print function (implementing abstract function of base class)
	//
	void printCamera();

	float GenerateRay(const CameraSample& sample, Ray* ray);

	float lensRadius;
	float focalDistance;

};



} //namespace rt




#endif /* THINLENS_H_ */
