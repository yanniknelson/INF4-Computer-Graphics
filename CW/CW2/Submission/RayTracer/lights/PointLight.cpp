/*
 * PointLight.cpp
 *
 *
 */
#include "PointLight.h"
#include "core/Scene.h"



namespace rt{


	Vec2<Vec3f> PointLight::SampleLight(Hit hit, void * scene) {
		
		Vec3f L = (position - hit.point).normalize();
		Ray lightRay = { hit.point, L };
		Hit shadowhit = ((Scene*)scene)->VolumeRoot->Intersect(lightRay);
		if (shadowhit.valid) {
			return Vec2<Vec3f>(Vec3f(0.f), Vec3f(0.f));
		}
		float dist = (hit.point - position).length();
		Vec2<Vec3f> ret = { id / (4 * M_PI * dist * dist), is / (4 * M_PI * dist * dist) };
		return ret;

	}

} //namespace rt
