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

	static Vec3f* render(Camera* camera, Scene* scene, int nbounces);
	static Vec3f* tonemap(Vec3f* pixelbuffer, int width, int height);
	static Matrix33f RayTracer::GetRGB2XYZ(Vec2f Rxy, Vec2f Gxy, Vec2f Bxy, Vec2f Wxy);
	static float sRGBExpandGama(float nonlinear);
	static float sRGBCompressGama(float linear);
	static Vec3f sRGBExpandGama(Vec3f colour);
	static Vec3f sRGBCompressGama(Vec3f colour);
	static Vec3f sRGB2Yxy(Matrix33f RGB2XYZ, Vec3f colour);
	static Vec3f Yxy2sRGB(Matrix33f XYZ2RGB, Vec3f colour);


private:


};




} //namespace rt



#endif /* RAYTRACER_H_ */

