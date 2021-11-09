/*
 * RayTracer.cpp
 *
 */
#include "RayTracer.h"


namespace rt {


	int colorToBin(Vec3f hdrColor) {
		Vec3f LuminanceVector = { 0.2125, 0.7154, 0.0721 };
		float lum = hdrColor.dotProduct(LuminanceVector);

		// Avoid taking the log of zero
		if (lum < std::numeric_limits<float>::min()) {
			return 0;
		}

		// Calculate the log_2 luminance and express it as a value in [0.0, 1.0]
		// where 0.0 represents the minimum luminance, and 1.0 represents the max.
		float logLum = Transform::Clamp((log2(lum) - -3) * (1.f/2.9f), 0.f, 1.f);

		// Map [0, 1] to [1, 255]. The zeroth bin is handled by the epsilon check above.
		return int(logLum * 254.0 + 1.0);
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
	Vec3f* RayTracer::render(Camera* camera, Scene* scene, int nbounces) {

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

		if (camera->thinLens) {
			NumOfSamples = 100;
		}

		int barWidth = 70;
		float progress = 0;

		for (int row = 0; row < camHeight; row++) {
			for (int column = 0; column < camWidth; column++) {

				Ray r{};
				//CameraSample s{ Vec2f((camWidth / 2) - column , row - camHeight), Vec2f(0, 0), 0.f };
				CameraSample s{ Vec2f(camWidth - (column + 1), row), Vec2f(0, 0), 0.f };
				
				for (int sampleNum = 0; sampleNum < NumOfSamples; sampleNum++) {
					camera->GenerateRay(s, &r);
					pixelbuffer[row * camWidth + column] += scene->backgroundColour;

					/*std::vector<Hit> hits;
					for (int i = 0; i < scene->GetNumShapes(); i++) {
						std::shared_ptr<Shape>  s = scene->getShape(i);
						Hit t = scene->getShape(i)->intersect(r);
						bool test = false;
						if (s->IntersectsBounds(r)) {
							//if (s->name.compare("Trimesh") == 0) std::cout << "hit model" << std::endl;
							hits.push_back(scene->getShape(i)->intersect(r));
							test = true;
						}
					}
					Hit closest = Hit{};
					for (int i = 0; i < hits.size(); i++) {
						if (hits[i].tValue < closest.tValue && hits[i].valid) closest = hits[i];
					}


					if (closest.valid) {
						pixelbuffer[row * camWidth + column] = ((Shape*)closest.object)->getMaterial()->GetShading(closest, scene, camera->getPos(), nbounces, false);
					}*/

					Hit h = scene->VolumeRoot->Intersect(r);

					if (h.valid) {
						pixelbuffer[row * camWidth + column] += ((Shape*)h.object)->getMaterial()->GetShading(h, scene, camera->getPos(), nbounces, false);
					}

				}

				pixelbuffer[row * camWidth + column] /= (float)NumOfSamples;
				//
				//Vec3f LuminanceVector = { 0.2125, 0.7154, 0.0721 };
				//float t = LuminanceVector.dotProduct(pixelbuffer[row * camWidth + column]);
				//if (t < minLum) minLum = t;
				//if (t > maxLum) maxLum = t;
				//LumHist[colorToBin(pixelbuffer[row * camWidth + column])] += 1;

				///*if (pixelbuffer[row * camWidth + column].x > max) max = pixelbuffer[row * camWidth + column].x;
				//if (pixelbuffer[row * camWidth + column].y > max) max = pixelbuffer[row * camWidth + column].y;
				//if (pixelbuffer[row * camWidth + column].z > max) max = pixelbuffer[row * camWidth + column].z;*/

			}
			std::cout << 100.f * (float)(row * camWidth) / (float)(camHeight * camWidth) << "%\r";
			std::cout.flush(); 
		}
		/*for (int i = 0; i < 256; i++) {
			std::cout << LumHist[i] << " ";
		}

		std::cout << std::endl;

		std::cout << minLum << " " << maxLum << std::endl;*/

		//averageLuminace /= resolution;

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

	Matrix33f RayTracer::GetRGB2XYZ(Vec2f Rxy, Vec2f Gxy, Vec2f Bxy, Vec2f Wxy) {
		Vec3f Rxyz = { Rxy.x, Rxy.y, 1 - Rxy.x - Rxy.y };
		Vec3f Gxyz = { Gxy.x, Gxy.y, 1 - Gxy.x - Gxy.y };
		Vec3f Bxyz = { Bxy.x, Bxy.y, 1 - Bxy.x - Bxy.y };
		Vec3f Wxyz = { Wxy.x, Wxy.y, 1 - Wxy.x - Wxy.y };

		Vec3f WXYZ = Wxyz / Wxyz.y;

		Matrix33f primaryMatrix = { Rxyz.x, Gxyz.x, Bxyz.x, Rxyz.y, Gxyz.y, Bxyz.y, Rxyz.z, Gxyz.z, Bxyz.z };
		Vec3f Scalars;

		primaryMatrix.inverse().multVecMatrix(WXYZ, Scalars);

		Matrix33f ScalarMatrix = { Scalars.x, 0, 0, 0, Scalars.y, 0, 0, 0,Scalars.z };

		return primaryMatrix * ScalarMatrix;

	}

	float RayTracer::sRGBExpandGama(float nonlinear)
	{
		return   (nonlinear <= 0.04045f) ? (nonlinear / 12.92f) : (powf((nonlinear + 0.055f) / 1.055f, 2.4f));
	}


	float RayTracer::sRGBCompressGama(float linear)
	{
		return   (linear <= 0.0031308f) ? (12.92f * linear) : (1.055f * powf(linear, 1.0f / 2.4f) - 0.055f);
	}


	Vec3f RayTracer::sRGBExpandGama(Vec3f colour)
	{
		return { sRGBExpandGama(colour.x), sRGBExpandGama(colour.y), sRGBExpandGama(colour.z) };
	}

	Vec3f RayTracer::sRGBCompressGama(Vec3f colour)
	{
		return { sRGBCompressGama(colour.x), sRGBCompressGama(colour.y), sRGBCompressGama(colour.z) };
	}

	Vec3f RayTracer::sRGB2Yxy(Matrix33f RGB2XYZ, Vec3f colour) {
		Vec3f XYZ;
		RGB2XYZ.multVecMatrix(colour, XYZ);
		return {XYZ.y, XYZ.x/(XYZ.x + XYZ.y + XYZ.z), XYZ.y / (XYZ.x + XYZ.y + XYZ.z) };
	}

	Vec3f RayTracer::Yxy2sRGB(Matrix33f XYZ2RGB, Vec3f colour) {
		Vec3f XYZ;
		Vec3f ret;

		XYZ.y = colour.x;
		XYZ.x = XYZ.y * colour.y/colour.z;
		XYZ.z = XYZ.x/colour.y - XYZ.x - XYZ.y;

		XYZ2RGB.multVecMatrix(XYZ, ret);
		return ret;
	}

/**
 * Tonemaps the rendered image (conversion of linear RGB values [0-1] to low dynamic range [0-255]
 *
 * @param pixelbuffer the array of size width*height containing linear RGB pixel values
 *
 * @return the tonemapped image
 */
Vec3f* RayTracer::tonemap(Vec3f* pixelbuffer, int width, int height){

	Matrix33f sRGB2XYZ = GetRGB2XYZ(Vec2f{ 0.7347, 0.2653 }, Vec2f{ 0.2738, 0.7174 }, Vec2f{ 0.1666, 0.0089 }, Vec2f{ 1.f/3.f, 1.f/3.f });
	Matrix33f XYZ2sRGB = sRGB2XYZ.inverse();

	//---------tonemapping function to be filled--------
	int resolution = width * height;
	for (int i = 0; i < resolution; i++) {
		pixelbuffer[i] = Transform::Clamp(pixelbuffer[i], 0.f, 1.f) * 255;//sRGBCompressGama(pixelbuffer[i]) * 255;//
	}

	return pixelbuffer;

}


} //namespace rt


