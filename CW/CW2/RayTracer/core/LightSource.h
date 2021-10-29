/*
 * LightSource.h
 *
 */

#ifndef LIGHTSOURCE_H_
#define LIGHTSOURCE_H_

#include "rapidjson/document.h"
#include "math/geometry.h"

using namespace rapidjson;

namespace rt{

class LightSource{
public:

	Vec3f position;
	Vec3f is;
	Vec3f id;

	LightSource(Vec3f position = {}, Vec3f is = {}, Vec3f id = {}) : position(position), is(is), id(id) {};

	static LightSource* createLightSource(Value& lightSpecs);

private:

};

} //namespace rt



#endif /* LIGHTSOURCE_H_ */
