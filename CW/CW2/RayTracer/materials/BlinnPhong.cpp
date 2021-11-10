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


	Vec3f BlinnPhong::GetShading(Hit h, void * scene, Vec3f eye, int bounce) const {
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

			if (tPath.length() > 0) {
				int index = ((int)(h.v * (tHeight - 1)) * tWidth) + (int)(h.u * tWidth);
				Vec3f textureSample = texture[index];
				BP = textureSample * (diff + spec) * light->SampleLight(h, scene);
				//std::cout << Vec3f(h.u * tWidth, h.v * tHeight, 0) << std::endl;
				//return { h.u, h.v, 0 };
			}
			else {
				BP = Diffuse * (diff + spec) * light->SampleLight(h, scene);
			}
		
			
			if (bounce > 0 && kr != 0) {
				Vec3f reflec = ((Scene*)scene)->backgroundColour;
				Vec3f R = (2 * (L.dotProduct(h.normal)) * h.normal - L).normalized();
				Ray reflectionRay = { h.point, R };
				Hit t = ((Scene*)scene)->VolumeRoot->Intersect(reflectionRay);

				if (t.valid) {
					Shape* s = ((Shape*)t.object);
					if (s->name.compare("face") == 0) {
						s = ((Shape*)t.TrimeshTri);
					}
					reflec = s->getMaterial()->GetShading(t, scene, h.point, bounce - 1);
				}
				BP = BP + kr * reflec;
			}

			//if (h.interior) std::cout << "interirorbefore" << std::endl;
			

			if (refractiveIndex > 0 && bounce > 0) {
				Vec3f refracComp = ((Scene*) scene) ->backgroundColour;

				float n = 1.f / refractiveIndex;
				float ncosthetain = h.normal.dotProduct(-V);
				float sine = 1 - (n * n * (1 - ncosthetain * ncosthetain));
				if (sine < 0) { break; }
				Vec3f refrac = (n * ncosthetain - sqrt(sine)) * h.normal - n * (-V);

				Ray refractionRay = { h.point, refrac };
				refractionRay.medium = refractiveIndex;

				Hit out = ((Shape*)h.object)->intersect(refractionRay);

				n = refractiveIndex / 1.f;
				ncosthetain = (-out.normal).dotProduct(-refrac);
				sine = 1 - (n * n * (1 - ncosthetain * ncosthetain));
				if (sine < 0) { break; }
				refrac = (n * ncosthetain - sqrt(sine)) * (-out.normal) - n * (-refrac);
				refractionRay = { out.point, refrac };

				Hit t = ((Scene*)scene)->VolumeRoot->IntersectLeaveOut(refractionRay, h);
				if (t.valid) {
					Shape* s = ((Shape*)t.object);
					if (s->name.compare("face") == 0) {
						s = ((Shape*)t.TrimeshTri);
					}
					t.interior = !h.interior;
					refracComp = s->getMaterial()->GetShading(t, scene, h.point, bounce - 1);
				}
				BP = BP + (1-kr) * refracComp;
			}

		}
		
		return  BP;

	}


} //namespace rt
