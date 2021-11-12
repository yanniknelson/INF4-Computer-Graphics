/*
 * Triangle.h
 *
 *
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "core/Shape.h"

namespace rt{

class Triangle: public Shape{

public:

	virtual ~Triangle();

	Triangle() {
		Triangle(Vec3f(), Vec3f(), Vec3f());
	}

	Triangle(Vec3f v0, Vec3f v1, Vec3f v2, Material *mat = nullptr) : v0(v0), v1(v1), v2(v2) { 
		material = mat;
		Vec3f avg = ((v0 + v1 + v2)/3).normalized();
		centroid = (v0 + v1 + v2) / 3;

		WorldBound = Bounds3f(v1, v0);
		WorldBound = WorldBound.Union(WorldBound, v2);
		name = "triangle";

	};

	Triangle(Vec3f v0, Vec3f v1, Vec3f v2, Vec3f n0, Vec3f n1, Vec3f n2, Material* mat = nullptr) : n0(n0.normalized()), n1(n1.normalized()), n2(n2.normalized()), v0(v0), v1(v1), v2(v2) {
		material = mat;
		hasNormals = true;
		Vec3f avg = ((v0 + v1 + v2) / 3).normalized();
		centroid = (v0 + v1 + v2) / 3;

		WorldBound = Bounds3f(v1, v0);
		WorldBound = WorldBound.Union(WorldBound, v2);
		name = "face";
	};

	Triangle(Vec3f v0, Vec3f v1, Vec3f v2, Vec2f t0, Vec2f t1, Vec2f t2, Material* mat = nullptr) : t0(t0), t1(t1), t2(t2), v0(v0), v1(v1), v2(v2) {
		material = mat;
		hasTextureMaps = true;
		Vec3f avg = ((v0 + v1 + v2) / 3).normalized();
		centroid = (v0 + v1 + v2) / 3;

		WorldBound = Bounds3f(v1, v0);
		WorldBound = WorldBound.Union(WorldBound, v2);
		name = "face";
	};

	Triangle(Vec3f v0, Vec3f v1, Vec3f v2, Vec3f n0, Vec3f n1, Vec3f n2, Vec2f t0, Vec2f t1, Vec2f t2, Material* mat = nullptr) : n0(n0.normalized()), n1(n1.normalized()), n2(n2.normalized()), t0(t0), t1(t1), t2(t2), v0(v0), v1(v1), v2(v2) {
		material = mat;
		hasNormals = true;
		hasTextureMaps = true;
		Vec3f avg = ((v0 + v1 + v2) / 3).normalized();
		centroid = (v0 + v1 + v2) / 3;

		WorldBound = Bounds3f(v1, v0);
		WorldBound = WorldBound.Union(WorldBound, v2);
		name = "face";
	};

	Hit intersect(Ray ray);

private: 

	bool hasNormals = false;
	bool hasTextureMaps = false;

	Vec3f v0;
	Vec3f v1;
	Vec3f v2;

	Vec3f n0;
	Vec3f n1;
	Vec3f n2;

	Vec2f t0;
	Vec2f t1;
	Vec2f t2;

};



} //namespace rt




#endif /* TRIANGLE_H_ */
