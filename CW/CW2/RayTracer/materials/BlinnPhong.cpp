/*
 * BlinnPhong.cpp
 *
 *
 */

#include "BlinnPhong.h"
#include "Transform.h"
#include "core/Scene.h"

#define STB_IMAGE_IMPLEMENTATION
#include "parsers/stb.h"




namespace rt{

	BlinnPhong::BlinnPhong(float ks, float kd , float kr , float spec , Vec3f Diffuse , std::string tPath, int tWidth , int tHeight ) : ks(ks), kd(kd), kr(kr), specular(spec), Diffuse(Diffuse), tPath(tPath), tWidth(tWidth), tHeight(tHeight) {
		if (tWidth > 0) {
			//std::wstring wstr = L"" + tPath;

			int x;
			int y;
			int num_chan;

			const char* path = tPath.c_str();

			stbi_uc* img = stbi_load(path, &x, &y, &num_chan, 3);

			if (img == NULL) {
				return;
			}

			int resolution = tWidth * tHeight;
			texture = new Vec3f[resolution];

			for (int i = 0; i < tHeight; i++) {
				for (int j = 0; j < tWidth; j++) {
					int index = (i * tWidth * 3) + (j * 3);
					texture[i * tWidth + j] = { (float)img[index] / 255.f, (float)img[index + 1] / 255.f, (float)img[index + 2] / 255.f };
				}
			}

			stbi_image_free(img);

		}
	}


	Vec3f BlinnPhong::GetShading(Hit h, void * scene, Vec3f eye, int bounce) const {

		LightSource* light = ((Scene *)scene)->getLight(0);

		Vec3f L = (light->position - h.point);
		float dist = L.length() * L.length();
		L.normalize();

		Vec3f V = (eye - h.point).normalized();
		Vec3f H = (L + V).normalized();

		Vec3f spec = pow(H.dotProduct(h.normal), specular) * ks * light->SampleLight(h.point); //pow(R.dotProduct(V), specular) * ks * light->SampleLight(h.point);
		float LdotN = Transform::Clamp(L.dotProduct(h.normal), 0.f, 1.f);
		Vec3f diff = LdotN * kd * light->SampleLight(h.point);

		Vec3f BP = diff + spec;

		Ray lightRay = { h.point, L };
		for (int i = 0; i < ((Scene*)scene)->GetNumShapes(); i++) {
			Hit hit = ((Scene*)scene)->getShape(i)->intersect(lightRay);
			if (hit.valid && hit.object != h.object) {
				BP = 0;
			}
		}

		if (bounce > 0 && kr != 0) {
			Vec3f reflec = ((Scene*)scene)->backgroundColour;
			Vec3f R = (2 * (L.dotProduct(h.normal)) * h.normal - L).normalized();
			Ray reflectionRay = { h.point, R };
			std::vector<Hit> hits;
			for (int i = 0; i < ((Scene*)scene)->GetNumShapes(); i++) {
				Hit t = ((Scene*)scene)->getShape(i)->intersect(reflectionRay);
				if (t.object != h.object) {
					hits.push_back(t);
				}
			}
			Hit closest = Hit{};
			for (int i = 0; i < hits.size(); i++) {
				if (hits[i].tValue < closest.tValue && hits[i].valid) closest = hits[i];
			}
			if (closest.valid) {
				reflec = ((Shape*)closest.object)->getMaterial()->GetShading(closest, scene, eye, bounce - 1);
			}
			reflec *= kr;
			BP = BP + reflec;
		}

		if (tPath.length() > 0) {
			int index = ((int)(h.v * tHeight) * tWidth) + (int)(h.u * tWidth);
			Vec3f textureSample = texture[index];
			return textureSample * BP;
			//std::cout << Vec3f(h.u * tWidth, h.v * tHeight, 0) << std::endl;
			//return { h.u, h.v, 0 };
		}
		
		return  Diffuse * BP;

	}


} //namespace rt
