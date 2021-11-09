/*
 * Material.cpp
 *
 */



#include "Material.h"
#include "materials/BlinnPhong.h"


namespace rt{


	Material * Material::createMaterial(Value& materialSpecs) {
	
		float r = materialSpecs["diffusecolor"][0].GetFloat();
		float g = materialSpecs["diffusecolor"][1].GetFloat();
		float b = materialSpecs["diffusecolor"][2].GetFloat();

		float ks = 0;
		float kd = 0;
		float kr = 0;
		float refractiveIndex = 0;
		float spec = 0;

		std::string tPath;
		int tWidth = 0;
		int tHeight = 0;

		if (materialSpecs.HasMember("ks")) ks = materialSpecs["ks"].GetFloat();
		if (materialSpecs.HasMember("kd")) kd = materialSpecs["kd"].GetFloat();
		if (materialSpecs.HasMember("kr")) kr = materialSpecs["kr"].GetFloat();
		if (materialSpecs.HasMember("refractiveIndex")) refractiveIndex = materialSpecs["refractiveIndex"].GetFloat();
		if (materialSpecs.HasMember("specularexponent"))spec = materialSpecs["specularexponent"].GetInt();

		if (materialSpecs.HasMember("tPath")) {
			tPath = materialSpecs["tPath"].GetString();
			tWidth = materialSpecs["tWidth"].GetInt();
			tHeight = materialSpecs["tHeight"].GetInt();
		}

		return new BlinnPhong(ks, kd, kr, spec, refractiveIndex, Vec3f(r,g,b), tPath, tWidth, tHeight);

	}


} //namespace rt


