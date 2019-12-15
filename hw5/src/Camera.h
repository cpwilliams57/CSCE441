#pragma once
#include <vector>
#include "Scene.h"
#include "Sphere.h"
#include "Plane.h" 
#include "Light.h"


//Added Libraries
#include <glm/glm.hpp>

//fill in camera class to have necessary member variables
class Camera
{
public:
	Camera();
	~Camera();

	Camera(int widthRes, int heightRes, glm::vec3 eye, glm::vec3 lookat, glm::vec3 up, float fovy);

	void TakePicture(Scene *scene);
	float* GetRenderedImage() { return renderedImage; };
	Ray PrimaryRay(int i, int j);
	Ray ShadowRay();
	glm::vec3 getColor(int index, float t, Scene *scene, Ray& primary_ray, int depth);
	int closestHit(Scene *scene, Ray& ray, float &t);
	int hitSpheres(Scene *scene, Ray& primary_ray, float &t);
	int hitPlanes(Scene *scene, Ray& primary_ray, float &t);
	void reflection(Ray reflection_ray, int depth);

private:


	int widthRes;
	int heightRes;
	float *renderedImage;


	glm::vec3 w;
	glm::vec3 u;
	glm::vec3 v; 

	glm::vec3 eye;
	float fl = 1.0;

	float ly;
	float lx;
	float pw; 
	glm::vec3 viewDir; 
	glm::vec3 lc; 
	glm::vec3 o; 


};


