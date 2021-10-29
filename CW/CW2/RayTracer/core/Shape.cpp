/*
 * Shape.cpp
 *
 */
#include "Shape.h"
#include "shapes/Sphere.h"
#include "shapes/Triangle.h"
#include "shapes/Plane.h"



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

		Shape* Shape::createShape(Value& shapeSpecs) {

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
			return new Sphere(center, r, Material::createMaterial((Value&)shapeSpecs["material"]));
		}
		else if (shapeType.compare("triangle") == 0) {
			Vec3f v0{ shapeSpecs["v0"][0].GetFloat(), shapeSpecs["v0"][1].GetFloat(), shapeSpecs["v0"][2].GetFloat() };
			Vec3f v1{ shapeSpecs["v1"][0].GetFloat(), shapeSpecs["v1"][1].GetFloat(), shapeSpecs["v1"][2].GetFloat() };
			Vec3f v2{ shapeSpecs["v2"][0].GetFloat(), shapeSpecs["v2"][1].GetFloat(), shapeSpecs["v2"][2].GetFloat() };
			return new Triangle(v0, v1, v2, Material::createMaterial((Value&)shapeSpecs["material"]));
		} else if (shapeType.compare("plane") == 0) {
			Vec3f v0{ shapeSpecs["v0"][0].GetFloat(), shapeSpecs["v0"][1].GetFloat(), shapeSpecs["v0"][2].GetFloat() };
			Vec3f v1{ shapeSpecs["v1"][0].GetFloat(), shapeSpecs["v1"][1].GetFloat(), shapeSpecs["v1"][2].GetFloat() };
			Vec3f v2{ shapeSpecs["v2"][0].GetFloat(), shapeSpecs["v2"][1].GetFloat(), shapeSpecs["v2"][2].GetFloat() };
			Vec3f v3{ shapeSpecs["v3"][0].GetFloat(), shapeSpecs["v3"][1].GetFloat(), shapeSpecs["v3"][2].GetFloat() };
			return new Plane(v0, v1, v2, v3, Material::createMaterial((Value&)shapeSpecs["material"]));
		}

		return 0;

	}



} //namespace rt

