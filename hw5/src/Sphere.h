#pragma once
#include "Shape.h"
#include <iostream>


class Sphere : public Shape
{
public:
	Sphere();
	~Sphere();

	//member attributes
	glm::vec3 position;
	float radius;

	//Interset function, check to see if a ray intersects a sphere 
	bool intersect(Ray& ray, float &t){
		
		//Calculate the delta
		glm::vec3 o = ray.orgin;
		glm::vec3 d = ray.direction;
		glm::vec3 v = o - position;
		const float b = 2* dot(v,d);
		const float c = dot(v,v) - radius*radius;
		float delta = b*b - 4*c;
		
		//if delta < 0, no intersection, return false
		if(delta < 0)
			return false;

		//calculate intersection points
		delta = sqrt(delta);
		float t0 = (-b - delta) / 2.0;
		float t1 = (-b + delta) / 2.0;

		if(t0 <= 0.0001 && t1 <= 0.0001)
		{
			return false;
		}

		//Find the nearest intersection point
		if(t0 < t1){
			t = t0;
		}
		else{
			t = t1; 
		}

		return true;
	}


private:

};
