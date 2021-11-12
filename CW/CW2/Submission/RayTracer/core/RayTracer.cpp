/*
 * RayTracer.cpp
 *
 */
#include "RayTracer.h"
#include "lights/AreaLight.h"

#define CAMERAJITTERING


namespace rt {


	Vec2f transform_to_disk(Vec2f on_square) {
		float theta = 2 * M_PI * on_square.x;
		float r = sqrt(on_square.y);
		return Vec2f(r * cos(theta), r * sin(theta));
	}

	Vec2f sampleLens() {
		Vec2f ret{ 1.f,1.f };
		//while (ret.length() > 1.f) {
			ret = Vec2f((float)std::rand() / RAND_MAX, (float)std::rand() / RAND_MAX);
		//}
		return transform_to_disk(ret);
	}

	/**
	 * Performs ray tracing to render a photorealistic scene
	 *
	 * @param camera the camera viewing the scene
	 * @param scene the scene to render, including objects and lightsources
	 * @param nbounces the number of bounces to consider for raytracing
	 *
	 * @return a pixel buffer containing pixel values in linear RGB format
	 */
	Vec3f* RayTracer::render(Camera* camera, Scene* scene, int nbounces, int nsamples) {

		std::srand(time(0));

		std::cout << nsamples << " samples" << std::endl;

		int camHeight = camera->getHeight();
		int camWidth = camera->getWidth();

		int resolution = camWidth * camHeight;

		Vec3f* pixelbuffer = new Vec3f[resolution];

		//----------main rendering function to be filled
		//int LumHist[256] = { 0 };
		//float maxLuminance = 0.f;
		//float minLum = 1.f;
		//float maxLum = 0.f;

		int NumOfSamples = 1;

		float rootSamples = sqrt(nsamples);

		float JInc = 1 / rootSamples;

		if (camera->thinLens) {
			NumOfSamples = nsamples;
		}



		int numLights = scene->GetNumLights();
		for (int i = 0; i < numLights; i++) {
			LightSource* l = scene->getLight(i);
			if (l->name.compare("arealight") == 0) {
				((AreaLight*)l)->setSamples(nsamples);
			}
		}

		int barWidth = 70;
		float progress = 0;

		for (int row = 0; row < camHeight; row++) {
			for (int column = 0; column < camWidth; column++) {

				Ray r{};
				//CameraSample s{ Vec2f((camWidth / 2) - column , row - camHeight), Vec2f(0, 0), 0.f };
				CameraSample s{ Vec2f(camWidth - (column + 1), row), Vec2f(0, 0), 0.f };
				
				for (int sampleNum = 0; sampleNum < NumOfSamples; sampleNum++) {

#ifndef CAMERAJITTERING
					s.pLens = sampleLens();
#endif // !CAMERAJITTERING
#ifdef CAMERAJITTERING
					Vec2f jitter = Vec2f((float)(sampleNum % (int)rootSamples) * JInc, (float)((int)(sampleNum / rootSamples) * JInc)) + (Vec2f((float)std::rand() / RAND_MAX, (float)std::rand() / RAND_MAX) * (1 / rootSamples));
					//std::cout << Vec2f((float)(sampleNum % (int)rootSamples) * JInc, (float)((int)(sampleNum / rootSamples)) * JInc);
					s.pLens = transform_to_disk(jitter);
#endif // CAMERAJITTERING

					
					camera->GenerateRay(s, &r);
					pixelbuffer[row * camWidth + column] += scene->backgroundColour;

					Hit h = scene->VolumeRoot->Intersect(r);

					if (h.valid) {
						Shape* s = ((Shape*)h.object);
						if (s->name.compare("face") == 0) {
							s = ((Shape*)h.TrimeshTri);
						}
						pixelbuffer[row * camWidth + column] += s->getMaterial()->GetShading(h, scene, camera->getPos(), nbounces);
					}

				}

				pixelbuffer[row * camWidth + column] /= (float)NumOfSamples;

			}
			std::cout << 100.f * (float)(row * camWidth) / (float)(camHeight * camWidth) << "%\r";
			std::cout.flush(); 
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
		pixelbuffer[i] = Transform::Clamp(pixelbuffer[i], 0.f, 1.f) * 255;
	}

	return pixelbuffer;

}


} //namespace rt


