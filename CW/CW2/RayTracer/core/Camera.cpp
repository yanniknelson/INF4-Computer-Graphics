/*
 * Camera.cpp
 *
 */
#include <iostream>

#include "Camera.h"
#include "cameras/Pinhole.h"
#include "cameras/ThinLens.h"

namespace rt{

	Camera::~Camera(){};



/**
 * Factory function that returns camera subclass based on camera specifications
 *
 * @param cameraSpecs camera specifications json object
 *
 * @return camera subclass instance
 *
 */
Camera* Camera::createCamera(Value& cameraSpecs){

	//check if cameratype is defined

	if (!cameraSpecs.HasMember("type")){
		std::cerr<<"Camera type not specified"<<std::endl;
		exit(-1);
	}

	std::string cameraType=cameraSpecs["type"].GetString();

	Vec3f position{ cameraSpecs["position"][0].GetFloat(), cameraSpecs["position"][1].GetFloat(), cameraSpecs["position"][2].GetFloat() };
	Vec3f up{ cameraSpecs["up"][0].GetFloat(), cameraSpecs["up"][1].GetFloat(), cameraSpecs["up"][2].GetFloat() };
	Vec3f lookAt{ cameraSpecs["lookat"][0].GetFloat(), cameraSpecs["lookat"][1].GetFloat(), cameraSpecs["lookat"][2].GetFloat() };

	//return camera object based on camera specs
	if (cameraType.compare("pinhole")==0){
		return new Pinhole(cameraSpecs["width"].GetInt(),
				cameraSpecs["height"].GetInt(),
				cameraSpecs["fov"].GetInt(), position, up, lookAt);

	}else if (cameraType.compare("thinlens")==0){
		return new ThinLens();
	}

	return 0;

}



} //namespace rt
