/*
 * Triangle.cpp
 *
 *
 */
#include "Plane.h"



namespace rt{

	Plane::~Plane() {};

	Hit Plane::intersect(Ray ray) {

		Hit h;
		//-----------to be implemented -------------

		h = t1.intersect(ray);
		if (!h.valid) {
			h = t2.intersect(ray);
		}

		h.object = this;
		h.normal = (v0 - v1).crossProduct(v2 - v1).normalized();

		Vec3f du = (v1 - v0);
		Vec3f dv = (v3 - v0);

		h.u =  du.ScalarProject(h.point - v0)/du.length();
		h.v =  dv.ScalarProject(h.point - v0)/dv.length();

		return h;

	}



} //namespace rt
