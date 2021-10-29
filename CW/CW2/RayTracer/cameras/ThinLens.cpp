/*
 * ThinLens.cpp
 *
 *
 */
#include <iostream>


#include "ThinLens.h"




namespace rt{

	//
	// ThinLens constructor (example)
	//
	ThinLens::ThinLens(int width, int height, int fov, Vec3f pos, Vec3f up, Vec3f lookAt):Camera(width, height, fov, pos, up, lookAt){

		//to fill

	}

	/**
	 * Prints camera data
	 * this function implements the abstract function of the base Camera class
	 */
	void ThinLens::printCamera(){
		printf("I am a thin lens camera! \n");
		printf("width: %dpx, height: %dpx, fov:%d \n", width, height, fov);
	}

	float ThinLens::GenerateRay(const CameraSample& sample, Ray* ray) {
		return 0.f;
	}

} //namespace rt
