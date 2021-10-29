/*
 * Material.cpp
 *
 */
#include "Material.h"


namespace rt{


	Material * Material::createMaterial(Value& materialSpecs) {
	
		float r = materialSpecs["diffusecolor"][0].GetFloat();
		float g = materialSpecs["diffusecolor"][1].GetFloat();
		float b = materialSpecs["diffusecolor"][2].GetFloat();

		float ks = 0;
		float kd = 0;
		float kr = 0;
		float spec = 0;

		if (materialSpecs.HasMember("ks")) ks = materialSpecs["ks"].GetFloat();
		if (materialSpecs.HasMember("kd")) kd = materialSpecs["kd"].GetFloat();
		if (materialSpecs.HasMember("kr")) kr = materialSpecs["kr"].GetFloat();
		if (materialSpecs.HasMember("specularexponent"))spec = materialSpecs["specularexponent"].GetInt();

		return new Material(ks, kd, kr, spec, Vec3f(r,g,b));

	}


} //namespace rt


