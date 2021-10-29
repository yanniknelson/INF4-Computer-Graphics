/*
 * Pinhole.cpp
 *
 *
 */
#include <iostream>

#include "Pinhole.h"



namespace rt {

	//
	// Pinhole constructor (example)
	//
	Pinhole::Pinhole(int width, int height, int fov, Vec3f pos, Vec3f up, Vec3f lookAt): Camera(width, height, fov, pos, up, lookAt) {
		CameraToScreen = Transform::Perspective(fov, 1e-2f, 1000.f);
		float aspectRatio = width / height;
		Bounds2f screenWindow{};
		if (aspectRatio > 1) {
			screenWindow.pMin.x = -aspectRatio;
			screenWindow.pMax.x = aspectRatio;
			screenWindow.pMin.y = -1;
			screenWindow.pMax.y = 1;
		}else {
			screenWindow.pMin.x = -1;
			screenWindow.pMax.x = 1;
			screenWindow.pMin.y = -1 / aspectRatio;
			screenWindow.pMax.y = 1 / aspectRatio;
		}

		ScreenToRaster =
			Transform::Scale(width, height, 1) *
			Transform::Scale(1 / (screenWindow.pMax.x - screenWindow.pMin.x), 1 / (screenWindow.pMin.y - screenWindow.pMax.y), 1) *
			Transform::Translate(Vec3f(-screenWindow.pMin.x, -screenWindow.pMax.y, 0)); //(p360)
		RasterToScreen = ScreenToRaster.Inverse();
		RasterToCamera = CameraToScreen.Inverse() * RasterToScreen;

		Vec3f pMin = RasterToCamera.TransformPoint(Vec3f(0, 0, 0));
		Vec3f pMax = RasterToCamera.TransformPoint(Vec3f(width, height, 0));
		std::cout << pMin << pMax << std::endl;

		Ray r{};
		CameraSample s{ Vec2f(0,0), Vec2f(0, 0), 0.f };
		Vec3f pFilm = Vec3f(s.pFilm.x, s.pFilm.y, 0);
		Vec3f pCamera = RasterToCamera.TransformPoint(pFilm).normalized();
		std::cout << pFilm << pCamera << std::endl;
		GenerateRay(s, &r);
		std::cout << r.o << r.d << std::endl;
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

