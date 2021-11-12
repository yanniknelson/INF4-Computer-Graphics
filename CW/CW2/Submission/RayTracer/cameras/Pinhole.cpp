/*
 * Pinhole.cpp
 *
 *
 */
#include <iostream>

#include "Pinhole.h"
#include "shapes/Bounds.h"



namespace rt {

	//
	// Pinhole constructor (example)
	//
	Pinhole::Pinhole(int width, int height, int fov, Vec3f pos, Vec3f up, Vec3f lookAt): Camera(width, height, fov, pos, up, lookAt) {
		
	}

	float Pinhole::GenerateRay(const CameraSample& sample, Ray* ray) {
		Vec3f pFilm = Vec3f(sample.pFilm.x, sample.pFilm.y, 0);
		Vec3f pCamera = RasterToCamera.TransformPoint(pFilm);
		*ray = Ray(Vec3f{}, pCamera.normalized());
		*ray = CameraToWorld.TransformRay(*ray);
		return 1;
	}


	/**
	 * Prints camera data
	 * this function implements the abstract function of the base Camera class
	 */
	void Pinhole::printCamera(){
		printf("I am a pinhole camera! \n");
		printf("width: %dpx, height: %dpx, fov:%d \n", width, height, fov);
	}



} //namespace rt

