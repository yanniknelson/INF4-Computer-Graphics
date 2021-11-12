/*
 * Shape.cpp
 *
 */
#include "Shape.h"
#include "shapes/Sphere.h"
#include "shapes/Triangle.h"
#include "shapes/Plane.h"
#include "shapes/TriMesh.h"

#include <memory>


namespace rt{

	//Quadratic solver for intersections
	bool Shape::Quadratic(float a, float b, float c, float& t0, float& t1) {
		double discrim = (double)b * (double)b - 4 * (double)a * (double)c;
		if (discrim < 0) return false;
		double rootDiscrim = std::sqrt(discrim);
		double q;
		if (b < 0) q = -.5 * (b - rootDiscrim);
		else q = -.5 * (b + rootDiscrim);
		t0 = q / a;
		t1 = c / q;
		if (t0 > t1) std::swap(t0, t1);
		return true;
	}

	Shape::~Shape() {};

		std::shared_ptr<Shape> Shape::createShape(Value& shapeSpecs) {

		//check if cameratype is defined

		if (!shapeSpecs.HasMember("type")) {
			std::cerr << "Camera type not specified" << std::endl;
			exit(-1);
		}

		std::string shapeType = shapeSpecs["type"].GetString();

		//return camera object based on camera specs
		if (shapeType.compare("sphere") == 0) {
			Vec3f center { shapeSpecs["center"][0].GetFloat(), shapeSpecs["center"][1].GetFloat(), shapeSpecs["center"][2].GetFloat() };
			float r = shapeSpecs["radius"].GetFloat();
			auto ret = std::make_shared<Sphere>(center, r, Material::createMaterial((Value&)shapeSpecs["material"]));
			if (shapeSpecs.HasMember("id")) {
				ret->name = shapeSpecs["id"].GetString();
				std::cout << ret->name << std::endl;
			}
			return ret;
		}
		else if (shapeType.compare("triangle") == 0) {
			Vec3f v0{ shapeSpecs["v0"][0].GetFloat(), shapeSpecs["v0"][1].GetFloat(), shapeSpecs["v0"][2].GetFloat() };
			Vec3f v1{ shapeSpecs["v1"][0].GetFloat(), shapeSpecs["v1"][1].GetFloat(), shapeSpecs["v1"][2].GetFloat() };
			Vec3f v2{ shapeSpecs["v2"][0].GetFloat(), shapeSpecs["v2"][1].GetFloat(), shapeSpecs["v2"][2].GetFloat() };
			Vec3f n0;
			Vec3f n1;
			Vec3f n2;
			Vec2f uv0;
			Vec2f uv1;
			Vec2f uv2;
			std::shared_ptr<Shape> ret;
			if (shapeSpecs.HasMember("n0")) {
				n0 = { shapeSpecs["n0"][0].GetFloat(), shapeSpecs["n0"][1].GetFloat(), shapeSpecs["n0"][2].GetFloat() };
				n1 = { shapeSpecs["n1"][0].GetFloat(), shapeSpecs["n1"][1].GetFloat(), shapeSpecs["n1"][2].GetFloat() };
				n2 = { shapeSpecs["n2"][0].GetFloat(), shapeSpecs["n2"][1].GetFloat(), shapeSpecs["n2"][2].GetFloat() };
			}

			if (shapeSpecs.HasMember("uv0")) {
				uv0 = { shapeSpecs["uv0"][0].GetFloat(), shapeSpecs["uv0"][1].GetFloat() };
				uv1 = { shapeSpecs["uv1"][0].GetFloat(), shapeSpecs["uv1"][1].GetFloat() };
				uv2 = { shapeSpecs["uv2"][0].GetFloat(), shapeSpecs["uv2"][1].GetFloat() };
			}
			
			ret = (shapeSpecs.HasMember("n0")) ? ((shapeSpecs.HasMember("uv0")) ? std::make_shared<Triangle>(v0, v1, v2, n0, n1, n2, uv0, uv1, uv2, Material::createMaterial((Value&)shapeSpecs["material"])) : std::make_shared<Triangle>(v0, v1, v2, n0, n1, n2, Material::createMaterial((Value&)shapeSpecs["material"]))) :
												 ((shapeSpecs.HasMember("uv0")) ? std::make_shared<Triangle>(v0, v1, v2, uv0, uv1, uv2, Material::createMaterial((Value&)shapeSpecs["material"])) : std::make_shared<Triangle>(v0, v1, v2, Material::createMaterial((Value&)shapeSpecs["material"])));

			if (shapeSpecs.HasMember("id")) {
				ret->name = shapeSpecs["id"].GetString();
				std::cout << ret->name << std::endl;
			}
			return ret;
		} else if (shapeType.compare("plane") == 0) {
			Vec3f v0{ shapeSpecs["v0"][0].GetFloat(), shapeSpecs["v0"][1].GetFloat(), shapeSpecs["v0"][2].GetFloat() };
			Vec3f v1{ shapeSpecs["v1"][0].GetFloat(), shapeSpecs["v1"][1].GetFloat(), shapeSpecs["v1"][2].GetFloat() };
			Vec3f v2{ shapeSpecs["v2"][0].GetFloat(), shapeSpecs["v2"][1].GetFloat(), shapeSpecs["v2"][2].GetFloat() };
			Vec3f v3{ shapeSpecs["v3"][0].GetFloat(), shapeSpecs["v3"][1].GetFloat(), shapeSpecs["v3"][2].GetFloat() };
			auto ret =  std::make_shared<Plane>(v0, v1, v2, v3, Material::createMaterial((Value&)shapeSpecs["material"]));
			if (shapeSpecs.HasMember("id")) {
				ret->name = shapeSpecs["id"].GetString();
				std::cout << ret->name << std::endl;
			}
			return ret;
		} else if (shapeType.compare("model") == 0) {

			if (shapeSpecs.HasMember("modelPath")) {
				// Construct the data object by reading from file
				Vec3f center{ shapeSpecs["center"][0].GetFloat(), shapeSpecs["center"][1].GetFloat(), shapeSpecs["center"][2].GetFloat() };
				auto ret = std::make_shared<TriMesh>(shapeSpecs["modelPath"].GetString(), center, shapeSpecs["scale"].GetFloat(), Material::createMaterial((Value&)shapeSpecs["material"]));
				if (shapeSpecs.HasMember("id")) {
					ret->name = shapeSpecs["id"].GetString();
					std::cout << ret->name << std::endl;
				}
				return ret;
			} else {
				Vec3f center{ shapeSpecs["center"][0].GetFloat(), shapeSpecs["center"][1].GetFloat(), shapeSpecs["center"][2].GetFloat() };
				const Value& shapesJson = shapeSpecs["faces"];
				std::vector<std::shared_ptr<Shape>> faces;
				if (shapesJson.IsArray()) {
					for (int i = 0; i < shapesJson.Size(); i++) {
						faces.push_back(Shape::createShape((Value&)shapesJson[i]));
					}
				}
				auto ret = std::make_shared<TriMesh>(faces, center, shapeSpecs["scale"].GetFloat(), Material::createMaterial((Value&)shapeSpecs["material"]));
				if (shapeSpecs.HasMember("id")) {
					ret->name = shapeSpecs["id"].GetString();
					std::cout << ret->name << std::endl;
				}
				return ret;
			}
		}

		return 0;

	}

		const bool Shape::IntersectsBounds(Ray& ray) const {
			float hit1, hit0;
			return WorldBound.IntersectP(ray, &hit0, &hit1);
		}



} //namespace rt

