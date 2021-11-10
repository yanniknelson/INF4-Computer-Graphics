/*
 * TriMesh.h
 *
 *
 */

#ifndef TRIMESH_H_
#define TRIMESH_H_

#include "core/Shape.h"
#include "shapes/Triangle.h"
#include "shapes/BVH.h"

namespace rt{

class TriMesh: public Shape{

public:

	TriMesh(std::string path, Vec3f position, float scale, Material* mat = nullptr);

	Hit intersect(Ray ray);

	Vec3f position;

	std::vector<std::shared_ptr<Shape>> faces;
	BVH::Node* VolumeRoot;

	float scale = 1.f;
};



} //namespace rt




#endif /* TRIMESH_H_ */
