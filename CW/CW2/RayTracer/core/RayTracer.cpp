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

	float max = 0.f;
	for (int row = 0; row < camHeight; row++) {
		for (int column = 0; column < camWidth; column++) {
			Ray r {};
			CameraSample s{ Vec2f((camWidth/2) -column , row - camHeight), Vec2f(0, 0), 0.f };
			camera->GenerateRay(s, &r);
			pixelbuffer[row * camWidth + column] = scene->backgroundColour;
			std::vector<Hit> hits;
			for (int i = 0; i < scene->GetNumShapes(); i++) {
				hits.push_back(scene->getShape(i)->intersect(r));
				//if (hits[i].valid) pixelbuffer[row * camHeight + column] = scene->getShape(i)->getMaterial().Diffuse;
			}
			Hit closest = Hit{};
			for (int i = 0; i < hits.size(); i++) {
				//std::cout << hits[i].valid  << "("<< hits[i].tValue<<")";
				if (hits[i].tValue < closest.tValue && hits[i].valid) closest = hits[i];
			}
			//std::cout << std::endl;
			
			if (closest.valid) {
				pixelbuffer[row * camWidth + column] = Transform::Clamp(((Shape*)closest.object)->getMaterial()->GetShading(closest, scene, camera->getPos(), nbounces), 0.f, 1.f);
			} 
			if (pixelbuffer[row * camWidth + column].x > max) max = pixelbuffer[row * camWidth + column].x;
			if (pixelbuffer[row * camWidth + column].y > max) max = pixelbuffer[row * camWidth + column].y;
			if (pixelbuffer[row * camWidth + column].z > max) max = pixelbuffer[row * camWidth + column].z;

		}
	}

	/*if (max > 1.f) {
		std::cout << "Scaling Triggered" << std::endl;
		for (int row = 0; row < camHeight; row++) {
			for (int column = 0; column < camWidth; column++) {
				pixelbuffer[row * camHeight + column] /= max;
			}
		}
	}*/


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


