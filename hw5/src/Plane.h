#pragma once
#include "Shape.h"

class Plane : public Shape
{
public:
	Plane();
	~Plane();

	//Member attributes
	glm::vec3 center;
	glm::vec3 normal;

	//plane intersect function
	bool intersect(Ray &ray, float &t){
		float denom = dot(ray.direction,normal);
		if(abs(denom) > 0){
			t = dot((center-ray.orgin),normal)/denom;
			if(t >= 0) return true;
		}
		return false;
	}


private:


};
