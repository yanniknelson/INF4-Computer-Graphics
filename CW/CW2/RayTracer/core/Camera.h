/*
 * Camera.h
 *
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "rapidjson/document.h"
#include "math/geometry.h"
#include "math/Transform.h"
#include "core/RayHitStructs.h"	


using namespace rapidjson;

namespace rt{

	struct CameraSample {
		Vec2f pFilm;
		Vec2f pLens;
		float time;
	};

class Camera{
public:

	//
	// Constructors
	//
	Camera(){};
	Camera(int height, int width, int fov, Vec3f pos, Vec3f up, Vec3f lookAt) :height(height), width(width), fov(fov), pos(pos) {
		CameraToWorld = Transform::LookAt(pos, lookAt, up);
		WorldToCamera = CameraToWorld.Inverse();
		CameraToScreen = Transform::Perspective(fov, 1e-2f, 1000.f);

		float aspectRatio = width / height;
		Bounds2f screenWindow{};
		if (aspectRatio > 1) {
			screenWindow.pMin.x = -aspectRatio;
			screenWindow.pMax.x = aspectRatio;
			screenWindow.pMin.y = -1;
			screenWindow.pMax.y = 1;
		}
		else {
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
	};

	//
	// Destructor
	//
	virtual ~Camera();


	//
	// factory function : returns camera instance dynamically based on camera specifications
	//
	static Camera* createCamera(Value& cameraSpecs);


	//
	// print function (to be implemented by the subclasses )
	//
	virtual void printCamera()=0;



	//
	// other camera functions (to complete)
	//

	virtual float GenerateRay(const CameraSample &sample, Ray *ray)= 0;

	//
	// Getters and Setters
	//
	int getHeight() const {
		return height;
	}

	void setHeight(int height) {
		this->height = height;
	}

	int getWidth() const {
		return width;
	}

	void setWidth(int width) {
		this->width = width;
	}

	Vec3f getPos() {
		return pos;
	}

	bool thinLens = false;


protected:

	//
	//camera members
	//
	int height;
	int width;
	int fov; //field of view
	Vec3f pos;

	Transform WorldToCamera;
	Transform CameraToWorld;
	Transform CameraToScreen;
	Transform ScreenToRaster;
	Transform RasterToScreen;
	Transform RasterToCamera;

};

} //namespace rt



#endif /* CAMERA_H_ */
