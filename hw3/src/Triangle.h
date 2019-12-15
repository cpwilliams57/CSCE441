//Modifications will occur in this file

#pragma once
#include <stdlib.h>
#include <math.h>
#include <glm/glm.hpp>


#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

//Define buffer
struct Buffer
{
	float color[WINDOW_HEIGHT][WINDOW_WIDTH][3];
	float depth[WINDOW_HEIGHT][WINDOW_WIDTH];
}; 

class Triangle {
	private:

	public:


		glm::vec3 v[3];		
		glm::vec3 c[3];

		// Constructor
		//we have a vec3 for each vertex in the triangle
		//each vec represents the x,y,and z coordinates of the particular vertex
		//how do we set the color...for each verticie
		Triangle(glm::vec3 &v0, glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &c0, glm::vec3 &c1, glm::vec3 &c2);

		// Rendering the triangle using OpenGL
		void RenderOpenGL(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix);


		// Rendering the triangle using CPU
		void RenderCPU(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix, Buffer &frameBuffer);
};
