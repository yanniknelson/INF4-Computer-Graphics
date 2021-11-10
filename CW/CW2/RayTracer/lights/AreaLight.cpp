/*
 * AreaLight.cpp
 *
 *
 */
#include "AreaLight.h"
#include "core/Scene.h"


#define NUM_OF_SAMPLES 100
#define ROOT_SAMPLES sqrt(NUM_OF_SAMPLES)

#define LIGHTJITTERING



namespace rt {


	Vec3f AreaLight::SampleLight(Hit hit, void * scene) {
		float dist = (hit.point - position).length();
		float shadowContribution = Obstruction(hit, scene);
		//std::cout << shadowContribution << std::endl;
		return (is / (4 * M_PI * dist * dist)) * shadowContribution;
	}

	float AreaLight::Obstruction(Hit hit, void* scene) {
		float hits = 0;
#ifndef LIGHTJITTERING
		for (int i = 0; i < NUM_OF_SAMPLES; i++) {
			Vec3f pointOnLight = ((float)std::rand() / RAND_MAX) * basis1length * basis1 + ((float)std::rand() / RAND_MAX) * basis2length * basis2 + lowestCorner;
			Vec3f L = (pointOnLight - hit.point).normalize();
			Ray lightRay = { hit.point, L };
			Hit shadowhit = ((Scene*)scene)->VolumeRoot->Intersect(lightRay);
			if (!shadowhit.valid) {
				hits += 1.f;
			}
		}
#endif // !LIGHTJITTERING

#ifdef LIGHTJITTERING
		float b1 = basis1length / ROOT_SAMPLES;
		float b2 = basis2length / ROOT_SAMPLES;
		for (int i = 0; i < ROOT_SAMPLES; i++) {
			for (int j = 0; j < ROOT_SAMPLES; j++) {
				Vec3f section = i*b1*basis1 + j*b2*basis2 + lowestCorner;
				Vec3f pointOnLight = ((float)std::rand() / RAND_MAX) * b1 * basis1 + ((float)std::rand() / RAND_MAX) * b2 * basis2 + section;
				Vec3f L = (pointOnLight - hit.point).normalize();
				Ray lightRay = { hit.point, L };
				Hit shadowhit = ((Scene*)scene)->VolumeRoot->Intersect(lightRay);
				if (!shadowhit.valid) {
					hits += 1.f;
				}
			}
		}
#endif // LIGHTJITTERING


		
		return hits / (float)NUM_OF_SAMPLES;
	}


} //namespace rt
