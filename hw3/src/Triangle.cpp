#include "Triangle.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>
#include <vector>


// A function clamping the input values to the lower and higher bounds
#define CLAMP(in, low, high) ((in) < (low) ? (low) : ((in) > (high) ? (high) : in))

//This will need to be modified to support changing the color of the image
Triangle::Triangle(glm::vec3 &v0, glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &c0, glm::vec3 &c1, glm::vec3 &c2)
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;

	c[0] = c0;
	c[1] = c1;
	c[2] = c2;
}

// Rendering the triangle using OpenGL
void Triangle::RenderOpenGL(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix) 
{

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(modelViewMatrix));

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projectionMatrix));

	glBegin(GL_TRIANGLES);
		glColor3f(c[0].x, c[0].y, c[0].z);
		glVertex3f(v[0].x, v[0].y, v[0].z);

		glColor3f(c[1].x, c[1].y, c[1].z);
		glVertex3f(v[1].x, v[1].y, v[1].z);

		glColor3f(c[2].x, c[2].y, c[2].z);
		glVertex3f(v[2].x, v[2].y, v[2].z);
	glEnd();
}

//***********************************************************************
//convert object coordiante screen coordiante 
glm::vec3 objToScreen(glm::vec3 objCord3, glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix){
	
	//viewport matrix
	glm::mat4 viewportMatrix(
		500, 0, 0, 500,
		0, 500, 0, 500, 
		0, 0, 1, 0,
		0, 0, 0, 1
	); 

	//Change input vec from vec3 to vec4
	//adding 1 to the end of the vertex position
	glm::vec4 objCord4(objCord3, 1.0);
	//Apply modelview, projection, and viewport transformation
	glm::vec4 screenVec = projectionMatrix * modelViewMatrix * objCord4 * viewportMatrix;
	//divide everything by the 4th value (w value), x&y coordinates will be x&y vals
	screenVec[0] = screenVec[0]/screenVec[3];
	screenVec[1] = screenVec[1]/screenVec[3];
	//palce baack into a vec3
	glm::vec3 screenCord(screenVec[0],screenVec[1],screenVec[2]);

	
	//return the new vector
	return screenCord;
}


//***********************************************************************
//Helper function finding trinangle min and max
std::vector<float> triangleBox(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, Buffer &frameBuffer ){
	//initialize variables for iteration of vertex values
	glm::vec3 tmp[3];
	tmp[0] = v1;
	tmp[1] = v2; 
	tmp[2] = v3; 

	//initialize x&y max & min values
	float xmin = 5000; 
	float xmax = -1;
	float ymin = 5000;
	float ymax = -1; 

	//Find true max and min values
	for(int i = 0; i < 3; i++){
		//compare xmin
		if(tmp[i][0] < xmin) xmin = tmp[i][0];
		//compare xmax
		if(tmp[i][0] > xmax) xmax = tmp[i][0];
		//compare ymin
		if(tmp[i][1] < ymin) ymin = tmp[i][1];
		//compare ymax
		if(tmp[i][1] > ymax) ymax = tmp[i][1];
	}

	//return our box values as a vector
	std::vector<float> boxvals;
	boxvals.push_back(xmin);
	boxvals.push_back(xmax);
	boxvals.push_back(ymin);
	boxvals.push_back(ymax);
	return boxvals;
}
//***********************************************************************
//Helper function finding area of triangle
//calculates area of the triangle from pizel to vertex
float area(int x1, int y1, int x2, int y2, int x3, int y3){ 
   return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0); 
} 

//***********************************************************************
void boundedRasterization(int xmin, int xmax, int ymin, int ymax, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, Buffer &frameBuffer,
	glm::vec3 c1, glm::vec3 c2, glm::vec3 c3){
	
	//retrieve values from input structures
	int x1 = v1[0];
	int y1 = v1[1];
	int x2 = v2[0];
	int y2 = v2[1];
	int x3 = v3[0];
	int y3 = v3[1];

	//Calculate area of triangle ABC 
    float A = area (x1, y1, x2, y2, x3, y3);

    //loop over the pixels in the bounded rasterization triangle
    for(int i = xmin; i < xmax; i++){
		for(int j = ymin; j < ymax; j++){
			
			//calculate triangle areas for selected pixel
			float A1 = area (i, j, x2, y2, x3, y3);
			float A2 = area (x1, y1, i, j, x3, y3);
			float A3 = area (x1, y1, x2, y2, i, j); 

			//color the pixel if the areas areas are equal
			if(A == A1 + A2 + A3){

				//3. Interpolation step************************
				//calc aplha, beta, gamma
				//calc barycentric coordinates
				float alpha = A1 / A; 
				float beta =  A2 / A; 
				float gamma = 1 - alpha - beta;  

				//4. calculate z value*************************
				float z1 = v1[2];
				float z2 = v2[2];
				float z3 = v3[2];
				float z = alpha*z1 + beta*z2 + gamma*z3; 

				//if z value is less than depth buffer at pixel 
				if(z < frameBuffer.depth[i][j]){
				
				//update depth buffer value
				frameBuffer.depth[i][j] = z;
				
				//color the triangles		
				frameBuffer.color[j][i][0] = alpha*c1[0] + beta*c2[0] + gamma*c3[0];
				frameBuffer.color[j][i][1] = alpha*c1[1] + beta*c2[1] + gamma*c3[1];
				frameBuffer.color[j][i][2] = alpha*c1[2] + beta*c2[2] + gamma*c3[2];
				}
			}
		}
	}
}

//***********************************************************************
// I need to know if I am on the right track here...
void Triangle::RenderCPU(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix, Buffer &frameBuffer){
	
	//1. Transform the triangle to the screen ***************************
	//Load ModelView Matrix 
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(modelViewMatrix));
	//Load projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projectionMatrix));
	//structure for holding our new vertexes
	glm::vec3 screenVertex[3];
	//For each vertex in the triangle
	for(int i = 0; i < 3; i++){
		//convert obj coordinates to view coordinates
		screenVertex[i] = objToScreen(this->v[i], modelViewMatrix, projectionMatrix);
	}

	//2. Rasterize the triangle *****************************************
	//find x&y, max&min of the triangle
	std::vector<float> boxvals = triangleBox(screenVertex[0],screenVertex[1],screenVertex[2], frameBuffer);

	//call the rasterization function to perform bounded rasterization 
	boundedRasterization(boxvals[0], boxvals[1], boxvals[2], boxvals[3], screenVertex[0], 
		screenVertex[1], screenVertex[2], frameBuffer, this->c[0], this->c[1], this->c[2]);

	//3.Interpolate the color of the pixel using Barycentric coordinates 
	//This occurs within the boundedRasterization funciton


	//4.Implement the z-buffer algorithm 
	//This occurs within the boundedRasterization function 

}
