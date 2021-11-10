/*
 * Sphere.cpp
 *
 *
 */
#include "Sphere.h"

namespace rt{


	Sphere::~Sphere() {};

	/**
	 * Computes whether a ray hit the specific instance of a sphere shape and returns the hit data
	 *
	 * @param ray cast ray to check for intersection with shape
	 *
	 * @return hit struct containing intersection information
	 *
	 */
	Hit Sphere::intersect(Ray ray){

		Hit h;
		//-----------to be implemented -------------

		//Transform Ray to Object Space
		Ray objRay = WorldToObject.TransformRay(ray);

		//Find intersection Point
		float a = objRay.d.norm();
		float b = 2 * objRay.d.dotProduct(objRay.o);
		float c = objRay.o.norm() - radius * radius;
		float t0, t1;
		if (!Quadratic(a, b, c, t0, t1)) {
			return h;
		}

		if (t0 > ray.tMax || t1 <= 0) {
			return h;
		}

		h.tValue = t0;
		h.t1 = t1;

		if (t0 <= 1e-4) {
			if (t1 > ray.tMax) {
				return h;
			}
			h.tValue = t1;
			h.t1 = t0;
			//std::cout << "t0 ";
		} 

		h.point = objRay(h.tValue);
		h.object = this;
		h.valid = true;

		//refine the intersection point (p225, go back to find triangle intersection too)
		h.point *= radius / (h.point - Vec3f()).length();
		h.normal = h.point.normalized();

		if (h.point.x == 0 && h.point.y == 0) h.point.x = 1e-5f * radius;
		float phi = atan2(h.point.y, h.point.x);
		if (phi < 0) phi += 2 * M_PI;
		h.u = phi / (2 * M_PI);
		float theta = acos(Transform::Clamp((h.point.z/radius), -1.f, 1.f));
		h.v = theta / M_PI;

		//transform the hit point and normal into world space
		h.point  = ObjectToWorld.TransformPoint(h.point);
		
		h.normal = ObjectToWorld.TransformNormal(h.normal);
		//std::cout << h.point << std::endl;
		return h;

	}

} //namespace rt


