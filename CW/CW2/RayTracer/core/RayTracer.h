/*
 * RayTracer.h
 *
 */
#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include "math/geometry.h"
#include "core/Camera.h"
#include "core/Scene.h"

namespace rt{



/*
 * Raytracer class declaration
 */
class RayTracer {
public:

	RayTracer() {};

	static Vec3f* render(Camera* camera, Scene* scene, int nbounces, int nsamples);
	static Vec3f* tonemap(Vec3f* pixelbuffer, int width, int height);

private:


};




} //namespace rt



#endif /* RAYTRACER_H_ */

