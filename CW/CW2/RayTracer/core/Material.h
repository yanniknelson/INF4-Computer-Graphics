/*
 * Material.h
 *
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#pragma once
#include <vector>
#include "math/geometry.h"
#include "rapidjson/document.h"
#include "core/RayHitStructs.h"

//#include <windows.h>
//#include <objidl.h>
//#include <gdiplus.h>
//using namespace Gdiplus;
//#pragma comment (lib,"Gdiplus.lib")

using namespace rapidjson;

namespace rt{

class Material{
public:

	Material() {};

	static Material* createMaterial(Value& materialSpecs);

	virtual Vec3f GetShading(Hit h, void* scene, Vec3f eye, int bounce, bool print) const = 0;

	virtual Vec3f GetDiffuse() const  = 0;

private:
	
};


} //namespace rt



#endif /* MATERIAL_H_ */
