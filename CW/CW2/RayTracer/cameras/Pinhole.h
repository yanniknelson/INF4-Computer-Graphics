/*
 * Pinhole.h
 *
 *
 */

#ifndef PINHOLE_H_
#define PINHOLE_H_

#include "core/Camera.h"

namespace rt{

class Pinhole:public Camera{
public:

	//
	// Constructors
	//
	Pinhole();
	Pinhole(int width, int height, int fov, Vec3f pos, Vec3f up, Vec3f lookAt);

	//
	//Destructor
	//
	~Pinhole(){};

	//
	// print function (implementing abstract function of base class)
	//
	void printCamera();
	float GenerateRay(const CameraSample& sample, Ray* ray);

};



} //namespace rt




#endif /* PINHOLE_H_ */
