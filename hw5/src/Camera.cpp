#include "Camera.h"
#include <iostream>


Camera::Camera()
{
}

Camera::~Camera()
{
}

//Re-definined camera args to accomadate for additional values
//added eye, lookat, up, and fovy to args
Camera::Camera(int widthRes, int heightRes, glm::vec3 eye, glm::vec3 lookat, glm::vec3 up, float fovy)
{	
	this->widthRes = widthRes; //width of frame
	this->heightRes = heightRes; //height of frame
	renderedImage = new float[widthRes * heightRes * 3];

	//Pre-calculate attributes for pixel  center
	this->w = normalize(eye - lookat);	//pointing backward, view direction
	this->u = normalize(cross(up,w)); 	//pointing rightward
	this->v = normalize(cross(w,u)); 	//pointing upward
	this->eye = eye;

	//I think these calculations are correct, check with hang ** 
	this->ly = 2 * fl * tan(fovy/2); 
	this->lx = ly * widthRes/heightRes;
	this->pw = ly/heightRes;
	this->viewDir = normalize(lookat - eye); 
	this->lc = eye + viewDir * fl;
	this->o = lc - lx/2.0f*u - ly/2.0f*v; 
}

//**********************************************************************
//used to compute S at point i,j, this seems to be right, check with hang
Ray Camera::PrimaryRay(int x, int y){
	//Calculate pixel center / direction for the given x,y coordinate
	glm::vec3 pc = o + (x + 0.5f) * pw * u + (y + 0.5f) * pw * v;
	//calculate direction, make sure to normalize d
	glm::vec3 d = normalize(pc - eye);
	//Create and return a ray with the appropriate information
	Ray ray = Ray(eye,d);
	return ray;
}

void reflection(Ray reflection_ray, int depth){

}


//**********************************************************************
//used to compute colr of pixel belonging to hit object
glm::vec3 Camera::getColor(int index, float t, Scene *scene, Ray& primary_ray, int depth){
	glm::vec3 color;

	//if the index is less than 4, get a sphere color
	if(index < 4){
		//initialize sphere pointer
		Shape* pointer = scene->shapes[index];
		Sphere* sphere = static_cast<Sphere*>(pointer);
		color = sphere->ka;
		glm::vec3 phit = eye + primary_ray.direction*t;
		glm::vec3 sN = normalize(phit - sphere->position);


		//include a for loop here to support both lights
		for(int i = 0; i < 2; i++){
			//calculate color as the sum of both lights
			Light* light = scene->lights[i];
			glm::vec3 E = normalize(eye-phit);
			glm::vec3 L = normalize(light->position - phit);
			glm::vec3 R = normalize(-reflect(L,sN));
			glm::vec3 diffuse = sphere->kd * fmax(dot(L,sN),0.0f);
			glm::vec3 specular = sphere->ks * pow(fmax(dot(R,E), 0.0f), sphere->s);
			color = color + light->color*diffuse + light->color*specular;

			// //recursive ray reflection
			glm::vec3 dr = -2 * dot(primary_ray.direction, sN) * sN + primary_ray.direction;
			Ray reflection_ray = Ray(phit,dr);
			float trs,trp;

			//Recalculate intersection
			int index1 = hitSpheres(scene,reflection_ray,trs);
			int index2 = hitPlanes(scene,reflection_ray,trp);

			if(depth!=3){
				if((trp > 999 && trs < 999)|| (trp < 999 && trs < 999 && trs < trp))
				{
					//set to color to sphere intersection color
					color =  color + sphere->km * getColor(index1,trs,scene,reflection_ray,depth+1);
				}
				else if (trp < 999)
				{//check if there is a plane intersection
					//setcolor to plane intersection color
					color =  color + sphere->km * getColor(index2,trp,scene,reflection_ray,depth+1);
				}

				//color = color + sphere->ks * getColor(index,tr,scene, reflection_ray,depth+1);
			}
		}
		
	}
	else{//inf index is 4 or greater, get plane color
		Shape* pointer = scene->shapes[index];
		Plane* plane = static_cast<Plane*>(pointer);
		color = plane->ka;
		glm::vec3 sN = normalize(plane->normal);
		glm::vec3 phit = eye + primary_ray.direction*t;//point of ray hit

		
		//include a for loop here to support both lights
		for(int i = 0; i< 2; i ++){
			Light* light = scene->lights[i]; //current light 
			
			//create a shadow ray
			Ray shadow_ray = Ray(phit, normalize(light->position - phit));
			float tsd;
			
			//see if the shadow ray lies in shadow
			if(hitSpheres(scene,shadow_ray,tsd) !=10 && tsd < length(phit - light->position)){
				//if it does, return ambient
				color = color + plane->ka * light->color;
			}
			else{
				//if it does not, calculate local lighting
				sN = normalize(plane->normal);
				glm::vec3 E = normalize(-primary_ray.direction);
				glm::vec3 L = normalize(light->position-phit);
				glm::vec3 diffuse = plane->kd * fmax(dot(L,sN),0.0f);
				glm::vec3 R = normalize(-reflect(L,sN));
				glm::vec3 specular = plane->ks * pow(fmax(dot(R,E), 0.0f), plane->s);
				color = color + light->color*diffuse + light->color*specular;
			}

			//recursive ray reflectionreflection from planes
			//compute direction of reflection ray
			// glm::vec3 dr = -2 * dot(primary_ray.direction, sN) * sN + primary_ray.direction;
			// Ray reflection_ray = Ray(phit,dr);
			// float tr;

			// if(depth!=3){
			// 	color = color + plane->ks * getColor(index,tr,scene, reflection_ray,depth+1);
			// }
		}		
	}

	return color;
}

//works
//**********************************************************************
//used to find closest sphere hit by a ray
int Camera::hitSpheres(Scene *scene, Ray& primary_ray, float &t){
	//initialize variables
	float ts = 1000;
	int smalls = 10; 
	//loop through all spheres
	for(int i = 0; i < 4; i++){
		//create a pointer to a sphere
		Shape* pointer = scene->shapes[i];
		Sphere* sphere = static_cast<Sphere*>(pointer);
		//check to see if the ray intersects the sphere
		if(sphere->intersect(primary_ray,t)){
			if(t < ts){
				ts = t;
				smalls = i;  
			}
		}
	}
	//set t to smallest value and return index
	t = ts;
	return smalls;
}
//Works
//**********************************************************************
int Camera::hitPlanes(Scene *scene, Ray& primary_ray, float &t){
	float tp = 1000;
	int smallp = 10; 
	//loop through all spheres
	for(int i = 4; i < 6; i++){
		//create a pointer to a sphere
		Shape* pointer = scene->shapes[i];
		Plane* plane = static_cast<Plane*>(pointer);
		//check to see if the ray intersects the sphere
		if(plane->intersect(primary_ray,t)){
			if(t < tp){
				tp = t;
				smallp = i;  
			}
		}
	}
	//set t to smallest value and return index
	t = tp;
	return smallp;
}

//**********************************************************************
void Camera::TakePicture(Scene *scene)
{
	//Initialize memory for render image
	memset(renderedImage, 0, sizeof(float) * widthRes * heightRes * 3);

	//variables for plane and sphere t values
	float ts;
	float tp;

	//For every pixel
	for(int y = 0; y < heightRes; y++){
		for(int x = 0; x < widthRes; x++){
			//Send a ray through the pixel
			Ray primary_ray = PrimaryRay(x,y); 
			
			//check for the closest hit
			int index1 = hitSpheres(scene,primary_ray,ts);
			int index2 = hitPlanes(scene,primary_ray,tp);
			
			//Initialize color
			glm::vec3 color(0,0,0);

			//Check if there is a sphere intersection
			if((tp > 999 && ts < 999)|| (tp < 999 && ts < 999 && ts < tp))
			{
				//set to color to sphere intersection color
				color = getColor(index1,ts,scene,primary_ray,0);
			}
			else if (tp < 999)
			{//check if there is a plane intersection
				//setcolor to plane intersection color
				color = getColor(index2,tp,scene,primary_ray,0);
			}
			//set final color for pixel
			renderedImage[(y * widthRes + x) * 3 + 0] = color[0];
			renderedImage[(y * widthRes + x) * 3 + 1] = color[1];
			renderedImage[(y * widthRes + x) * 3 + 2] = color[2];
	
		}
	}
}



//for each phit
	//for each light source
		//compute  reflective ray
		//compute local phong shading
		//if have not reached reflection depth
			//compute reflection of the ray
			 //color = ks * shade(R1,Recursion depth + 1)	
	
		//combine color of ray  with phong shading

//  Trace Shadow Ray(s) to all light(s)
//  Color = Visible ? Illumination Model : 0 ;

//  Trace Reflected Ray
//  Color += reflectivity * Color of reflected ray


