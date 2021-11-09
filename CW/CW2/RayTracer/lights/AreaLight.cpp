/*
 * AreaLight.cpp
 *
 *
 */
#include "AreaLight.h"
#include "core/Scene.h"


#define NUM_OF_SAMPLES 100

namespace rt {


	Vec3f AreaLight::SampleLight(Hit hit, void * scene) {
		float dist = (hit.point - position).length();
		float shadowContribution = Obstruction(hit, scene);
		//std::cout << shadowContribution << std::endl;
		return (is / (4 * M_PI * dist * dist)) * shadowContribution;
	}

	float AreaLight::Obstruction(Hit hit, void* scene) {
		float hits = 0;
		for (int i = 0; i < NUM_OF_SAMPLES; i++) {
			Vec3f pointOnLight = Vec3f((float)std::rand() / RAND_MAX, (float)std::rand() / RAND_MAX, (float)std::rand() / RAND_MAX) * diag + lowestCorner;
			Vec3f L = (pointOnLight - hit.point).normalize();
			Ray lightRay = { hit.point, L };
			Hit shadowhit = ((Scene*)scene)->VolumeRoot->IntersectShadow(lightRay, hit.object);
			if (!shadowhit.valid) {
				hits += 1.f;
			}
		}
		return hits / (float)NUM_OF_SAMPLES;
	}


} //namespace rt
