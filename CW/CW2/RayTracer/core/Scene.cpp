/*
 * Scene.cpp
 *
 */
#include "Scene.h"




namespace rt{

/**
 * Parses json scene object to generate scene to render
 *
 * @param scenespecs the json scene specificatioon
 */
void Scene::createScene(Value& scenespecs){

	//----------parse json object to populate scene-----------

	if (scenespecs.HasMember("backgroundcolor")) {
		const Value& background = scenespecs["backgroundcolor"];
		if (background.IsArray()) {
			backgroundColour = Vec3f(background[0].GetFloat(), background[1].GetFloat(), background[2].GetFloat());
		}
	}

	if (scenespecs.HasMember("lightsources")) {
		const Value& lights = scenespecs["lightsources"];
		if (lights.IsArray()) {
			for (int i = 0; i < lights.Size(); i++) {
				lightSources.push_back(LightSource::createLightSource((Value&)lights[i]));
			}
		}
	}

	if (scenespecs.HasMember("shapes")) {
		const Value& shapesJson = scenespecs["shapes"];
		if (shapesJson.IsArray()) {
			for (int i = 0; i < shapesJson.Size(); i++) {
				shapes.push_back(Shape::createShape((Value&)shapesJson[i]));
			}
		}
	}

	VolumeRoot = BVH::BuildTree(shapes);

	std::cout << shapes.size() << " Built Tree " << VolumeRoot->numOfShapes << std::endl;
}



} //namespace rt
