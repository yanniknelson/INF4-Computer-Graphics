/*
 * Scene.h
 *
 */

#ifndef SCENE_H_
#define SCENE_H_

#pragma once
#include <vector>
#include "rapidjson/document.h"
#include "core/LightSource.h"
#include "core/Shape.h"
#include "math/geometry.h"



using namespace rapidjson;

namespace rt{

	class Scene {
	public:

		Scene() {};

		void createScene(Value& scenespecs);

		Vec3f backgroundColour{};

		int GetNumLights() {
			return lightSources.size();
		}

		int GetNumShapes() {
			return shapes.size();
		}

		Shape* getShape(int i) {
			if (i < shapes.size()) {
				return shapes[i];
			}
		}

		LightSource* getLight(int i) {
			if (i < lightSources.size()) {
				return lightSources[i];
			}
		}

private:

	std::vector<LightSource*> lightSources;
	std::vector<Shape*> shapes;

};

} //namespace rt



#endif /* SCENE_H_ */
