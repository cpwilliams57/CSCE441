#pragma once
#include <glm/glm.hpp>
#include "Ray.h"

class Shape
{
public:
	Shape(void);
	~Shape(void);

	//Attributes that all shapes share
	glm::vec3 ka;
	glm::vec3 kd;
	glm::vec3 ks;
	glm::vec3 km;
	float s; 
	
private:	
	
};


