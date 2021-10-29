/*
 * Material.h
 *
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "math/geometry.h"
#include "rapidjson/document.h"

using namespace rapidjson;

namespace rt{

class Material{
public:

	Material(float ks = 0.f, float kd = 0.f, float kr = 0.f, float spec = 0.f, Vec3f Diffuse = Vec3f{ 1 }, std::string tPath = "", int tWidth = 0, int tHeight = 0) : ks(ks), kd(kd), kr(kr), specular(spec), Diffuse(Diffuse), tPath(tPath), tWidth(tWidth), tHeight(tHeight) {
	
	}

	static Material * createMaterial(Value& materialSpecs);

	float ks;
	float kd;
	float kr;
	float specular;
	Vec3f Diffuse;
	std::string tPath;
	int tWidth;
	int tHeight;

private:
	
};


} //namespace rt



#endif /* MATERIAL_H_ */
