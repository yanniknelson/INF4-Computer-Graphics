/*
 * PointLight.cpp
 *
 *
 */
#include "PointLight.h"
#include "core/Scene.h"



namespace rt{


	Vec3f PointLight::SampleLight(Hit hit, void * scene) {
		
		Vec3f L = (position - hit.point).normalize();
		Ray lightRay = { hit.point, L };
		Hit shadowhit = ((Scene*)scene)->VolumeRoot->IntersectShadow(lightRay, hit.object);
		if (shadowhit.valid) {
			return 0.f;
		}
		float dist = (hit.point - position).length();
		return (is / (4 * M_PI * dist * dist));

	}

} //namespace rt
