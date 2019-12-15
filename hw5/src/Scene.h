#pragma once
#include <vector>
#include "Shape.h"
#include "Light.h"

//Going to need to include GLM Headers here
#include <glm/glm.hpp>

//Shape structs
//sphere
//plane

class Scene
{
public:
	Scene();
	~Scene();

	std::vector<Shape*> shapes;
	std::vector<Light*> lights;

private:
	
};


