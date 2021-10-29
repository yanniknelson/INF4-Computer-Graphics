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
		} else if (!(t0 > ray.tMax || t1 <= 0)){
			if (t0 > 0) {
				h.point = ray(t0);
				h.tVlaue = t0;
			} else {
				h.point = ray(t1);
				h.tVlaue = t1;
			}
			h.object = this;
			h.valid = true;
		}
		//refine the intersection point (p225, go back to find triangle intersection too)
		h.point *= radius / (h.point - Vec3f()).length();
		if (h.point.x == 0 && h.point.y == 0) h.point.x = 1e-5f * radius;
		float phi = atan2(h.point.y, h.point.x);
		if (phi < 0) phi += 2 * M_PI;
		h.u = phi / (2 * M_PI);
		float theta = acos(Clamp((h.point.z/radius), -1.f, 1.f));
		h.v = (theta + 1.f) / 2.f;
		//still need to calculate normal

		//transform the hit point and normal into world space
		h.point  = ObjectToWorld.TransformPoint(h.point);
		h.normal = ObjectToWorld.TransformNormal(h.normal);
		return h;

	}

} //namespace rt


