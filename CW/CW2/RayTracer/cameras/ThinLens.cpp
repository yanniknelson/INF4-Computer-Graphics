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
	ThinLens::ThinLens(int width, int height, int fov, float lensRadius, float focalDistance, Vec3f pos, Vec3f up, Vec3f lookAt) : lensRadius(lensRadius), focalDistance(focalDistance), Camera(width, height, fov, pos, up, lookAt) {
		std::srand(time(NULL));
		thinLens = true;
	};

	/**
	 * Prints camera data
	 * this function implements the abstract function of the base Camera class
	 */
	void ThinLens::printCamera(){
		printf("I am a thin lens camera! \n");
		printf("width: %dpx, height: %dpx, fov:%d \n", width, height, fov);
	}

	float ThinLens::GenerateRay(const CameraSample& sample, Ray* ray) {
		Vec3f pFilm = Vec3f(sample.pFilm.x, sample.pFilm.y, 0);
		Vec3f pCamera = RasterToCamera.TransformPoint(pFilm);
		*ray = Ray(Vec3f{}, pCamera.normalized());
		Vec2f pLens = lensRadius * sampleLens();
		float ft = focalDistance / ray->d.z;
		Vec3f pFocus = (*ray)(ft);
		ray->o = Vec3f(pLens.x, pLens.y, 0.f);
		ray->d = (pFocus - ray->o).normalize();
		*ray = CameraToWorld.TransformRay(*ray);
		return 1;
	}

	Vec2f ThinLens::sampleLens() {
		Vec2f ret{1.f,1.f};
		while (ret.length() > 1.f) {
			ret = Vec2f((float)std::rand() / RAND_MAX, (float)std::rand() / RAND_MAX);
		}
		return ret;
	}

} //namespace rt
