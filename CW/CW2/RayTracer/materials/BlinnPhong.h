/*
 * BlinnPhong.h
 *
 *
 */

#ifndef BLINNPHONG_H_
#define BLINNPHONG_H_

#include "core/Material.h"

namespace rt{

class BlinnPhong: public Material{

public:

	BlinnPhong(float ks = 0.f, float kd = 0.f, float kr = 0.f, float spec = 0.f, float refractiveIndex = 0.f, Vec3f Diffuse = Vec3f{ 1 }, std::string tPath = "", int tWidth = 0, int tHeight = 0);

	float ks;
	float kd;
	float kr;
	float refractiveIndex;
	float specular;
	Vec3f Diffuse;
	std::string tPath;
	int tWidth;
	int tHeight;
	Vec3f* texture;

	Vec3f GetShading(Hit h, void* scene, Vec3f eye, int bounce, bool print) const;

	Vec3f GetDiffuse() const {
		return Diffuse;
	}

};



} //namespace rt



#endif /* BLINNPHONG_H_ */
