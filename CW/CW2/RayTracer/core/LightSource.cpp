/*
 * LightSource.cpp
 *
 */
#include <iostream>
#include "LightSource.h"
#include "lights/PointLight.h"
#include "lights/AreaLight.h"

namespace rt{


	LightSource* LightSource::createLightSource(Value& lightSpecs) {

		//check if cameratype is defined

		if (!lightSpecs.HasMember("type")) {
			std::cerr << "Camera type not specified" << std::endl;
			exit(-1);
		}

		std::string lightType = lightSpecs["type"].GetString();

		//return camera object based on camera specs
		if (lightType.compare("pointlight") == 0) {
			Vec3f pos{ lightSpecs["position"][0].GetFloat(), lightSpecs["position"][1].GetFloat(), lightSpecs["position"][2].GetFloat() };
			Vec3f is{ lightSpecs["is"][0].GetFloat(), lightSpecs["is"][1].GetFloat(), lightSpecs["is"][2].GetFloat() };
			Vec3f id{ lightSpecs["id"][0].GetFloat(), lightSpecs["id"][1].GetFloat(), lightSpecs["id"][2].GetFloat() };
			return new PointLight(pos, is, id);
		} else if (lightType.compare("arealight") == 0) {
			return new AreaLight();
		}

		return 0;

	}


} //namespace rt

