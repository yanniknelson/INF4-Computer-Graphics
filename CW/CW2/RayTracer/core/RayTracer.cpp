/*
 * RayTracer.cpp
 *
 */
#include "RayTracer.h"


namespace rt{


/**
 * Performs ray tracing to render a photorealistic scene
 *
 * @param camera the camera viewing the scene
 * @param scene the scene to render, including objects and lightsources
 * @param nbounces the number of bounces to consider for raytracing
 *
 * @return a pixel buffer containing pixel values in linear RGB format
 */
Vec3f* RayTracer::render(Camera* camera, Scene* scene, int nbounces){

	int camHeight = camera->getHeight();
	int camWidth = camera->getWidth();

	int resolution = camWidth * camHeight;

	Vec3f* pixelbuffer=new Vec3f[resolution];

	//----------main rendering function to be filled


	for (int row = 0; row < camHeight; row++) {
		for (int column = 0; column < camWidth; column++) {
			Ray r {};
			Vec2f pFilm = Vec2f((float)column / (float)camWidth, (float)row / (float)camHeight);
			pFilm *= 2.f;
			pFilm -= 1.f;
			CameraSample s { pFilm * 400, Vec2f(0, 0), 0.f };
			camera->GenerateRay(s, &r);
			pixelbuffer[row * camHeight + column] = scene->backgroundColour;
			std::vector<Hit> hits;
			for (int i = 0; i < scene->GetNumShapes(); i++) {
				hits.push_back(scene->getShape(i)->intersect(r));
				//if (hits[i].valid) pixelbuffer[row * camHeight + column] = scene->getShape(i)->getMaterial().Diffuse;
			}
			Hit closest = Hit{};
			for (int i = 0; i < hits.size(); i++) {
				//std::cout << hits[i].valid  << "("<< hits[i].tVlaue<<")";
				if (hits[i].tVlaue < closest.tVlaue && hits[i].valid) closest = hits[i];
			}
			//std::cout << std::endl;

			if (closest.valid) pixelbuffer[row * camHeight + column] = ((Shape*)closest.object)->getMaterial().Diffuse;
		}
	}


	return pixelbuffer;
	
}

/**
 * Tonemaps the rendered image (conversion of linear RGB values [0-1] to low dynamic range [0-255]
 *
 * @param pixelbuffer the array of size width*height containing linear RGB pixel values
 *
 * @return the tonemapped image
 */
Vec3f* RayTracer::tonemap(Vec3f* pixelbuffer, int width, int height){
	
	//---------tonemapping function to be filled--------
	int resolution = width * height;
	for (int i = 0; i < resolution; i++) {
		pixelbuffer[i] *= 255.f;
	}

	return pixelbuffer;

}


} //namespace rt


