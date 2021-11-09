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

	BlinnPhong::BlinnPhong(float ks, float kd , float kr , float spec , float refractiveIndex, Vec3f Diffuse , std::string tPath, int tWidth , int tHeight ) : ks(ks), kd(kd), kr(kr), specular(spec), refractiveIndex(refractiveIndex), Diffuse(Diffuse), tPath(tPath), tWidth(tWidth), tHeight(tHeight) {
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


	Vec3f BlinnPhong::GetShading(Hit h, void * scene, Vec3f eye, int bounce, bool print) const {
		//if (h.interior) std::cout << "interiror" << std::endl;
		int numLights = ((Scene*)scene)->GetNumLights();
		Vec3f BP{};

		for (int l = 0; l < numLights; l++) {
			LightSource* light = ((Scene*)scene)->getLight(l);

			Vec3f L = (light->position - h.point);
			float dist = L.length() * L.length();
			L.normalize();

			Vec3f V = (eye - h.point).normalized();
			Vec3f H = (L + V).normalized();

			Vec3f spec = pow(Transform::Clamp(H.dotProduct(h.normal), 0.f, 1.f), specular) * ks;
			float LdotN = Transform::Clamp(L.dotProduct(h.normal), 0.f, 1.f);
			Vec3f diff = LdotN * kd;

			

			BP = BP + (diff + spec) * light->SampleLight(h, scene);
		
			
			if (bounce > 0 && kr != 0) {
				Vec3f reflec = ((Scene*)scene)->backgroundColour;
				Vec3f R = (2 * (L.dotProduct(h.normal)) * h.normal - L).normalized();
				Ray reflectionRay = { h.point, R };
				Hit t = ((Scene*)scene)->VolumeRoot->IntersectReflect(reflectionRay, h.object);

				if (t.valid) {
					reflec = ((Shape*)t.object)->getMaterial()->GetShading(t, scene, eye, bounce - 1, print);
				}
				BP = BP + kr * reflec;
			}

			//if (h.interior) std::cout << "interirorbefore" << std::endl;
			

			if (refractiveIndex > 0) {
				//if (h.interior) std::cout << "interirorafter" << std::endl;
				Vec3f refracComp = ((Scene*)scene)->backgroundColour;
				float n = (h.interior)? refractiveIndex/ 1.f : 1.f / refractiveIndex;
				//if (h.interior) std::cout << "interiror" << std::endl;
				float ncosthetai = h.normal.dotProduct(-V);
				float sine = 1 - (n * n * (1 - ncosthetai * ncosthetai));
				Vec3f refrac = (n * ncosthetai - sqrt(sine)) * h.normal - n * (-V);
				//std::cout << -V << " " << refrac << std::endl;
				Ray refractionRay = { h.point, refrac };
				refractionRay.medium = refractiveIndex;
				Hit t = ((Scene*)scene)->VolumeRoot->IntersectRefraction(refractionRay);
				if (t.valid) {
					refracComp = ((Shape*)t.object)->getMaterial()->GetShading(t, scene, eye, bounce - 1, print);
				}
				BP = BP + (1-kr) * refracComp;
			}

		}

		if (tPath.length() > 0) {
			int index = ((int)(h.v * (tHeight - 1)) * tWidth) + (int)(h.u * tWidth);
			Vec3f textureSample = texture[index];
			return textureSample * BP;
			//std::cout << Vec3f(h.u * tWidth, h.v * tHeight, 0) << std::endl;
			//return { h.u, h.v, 0 };
		}
		
		return  Diffuse * BP;

	}


} //namespace rt
