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
		std::cout << CameraToWorld.m << std::endl;
		std::cout << CameraToWorld.TransformRay(Ray(Vec3f{}, Vec3f(0,0,1))).o << CameraToWorld.TransformRay(Ray(Vec3f{}, Vec3f(0, 0, 1))).d << std::endl;

		WorldToCamera = CameraToWorld.Inverse();
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
