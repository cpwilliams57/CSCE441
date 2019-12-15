#include "MatrixStack.h"

#include <stdio.h>
#include <cassert>
#include <vector>
#include <iostream>
#include <cmath>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 

MatrixStack::MatrixStack()
{
	mstack = make_shared< stack<glm::mat4> >();
	mstack->push(glm::mat4(1.0));
}

MatrixStack::~MatrixStack()
{
}

void MatrixStack::pushMatrix()
{
	const glm::mat4 &top = mstack->top();
	mstack->push(top);
	assert(mstack->size() < 100);
}

void MatrixStack::popMatrix()
{
	assert(!mstack->empty());
	mstack->pop();
	// There should always be one matrix left.
	assert(!mstack->empty());
}

void MatrixStack::loadIdentity()
{
	glm::mat4 &top = mstack->top();
	top = glm::mat4(1.0);
}

//translates a matrix on x,y,z dimensions
void MatrixStack::translate(const glm::vec3 &t)
{	
	//glm mat4 translatonMatrix(1.0f);

	//create a new translation matrix from x,y,z vals from t
	glm::mat4 translationMatrix(
		1.0f, 0.0f, 0.0f, 0.0f,
    	0.0f, 1.0f, 0.0f, 0.0f,
    	0.0f, 0.0f, 1.0f, 0.0f,
    	t.x,  t.y,  t.z,  1.0f 
    );

	// Need to comment out the following line and write your own version
	//translationMatrix = glm::translate(glm::mat4(1.0f), t);

	//pass the matrix to mult matrix
	multMatrix(translationMatrix);
}

//enlarges or shrinks scale of a matrix
void MatrixStack::scale(const glm::vec3 &s)
{
	//glm::mat4 scaleMatrix(1.0f);
	glm::mat4 scaleMatrix(
		s.x, 0.0f, 0.0f, 0.0f,
    	0.0f, s.y, 0.0f, 0.0f,
    	0.0f, 0.0f, s.z, 0.0f,
    	0.0f, 0.0f, 0.0, 1.0f 
    );

	// Need to comment out the following line and write your own version
	//scaleMatrix = glm::scale(glm::mat4(1.0f), s);

	multMatrix(scaleMatrix);
}

//rotates angle on x axis 
//used http://mathworld.wolfram.com/RotationMatrix.html for reference
void MatrixStack::rotateX(float angle)
{
	//glm::mat4 rotationMatrix(1.0f);
	glm::mat4 rotationMatrix(
		1.0f, 0.0f, 0.0f, 0.0f,
    	0.0f, cos(angle), sin(angle), 0.0f,
    	0.0f, -sin(angle), cos(angle), 0.0f,
    	0.0f, 0.0f, 0.0, 1.0f 
    );

	// Need to comment out the following line and write your own version
	//rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));

	multMatrix(rotationMatrix);
}

//rotates andle on y axis
//used http://mathworld.wolfram.com/RotationMatrix.html for reference
void MatrixStack::rotateY(float angle)
{
	//glm::mat4 rotationMatrix(1.0f);
	glm::mat4 rotationMatrix(
		cos(angle), 0.0f, -sin(angle), 0.0f,
    	0.0f, 1.0f, 0.0f, 0.0f,
    	sin(angle), 0.0f, cos(angle), 0.0f,
    	0.0f, 0.0f, 0.0, 1.0f 
    );

	// Need to comment out the following line and write your own version
	//rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));

	multMatrix(rotationMatrix);
}

//rotates angle on z axis
//used http://mathworld.wolfram.com/RotationMatrix.html for reference
void MatrixStack::rotateZ(float angle)
{
	//glm::mat4 rotationMatrix(1.0f);
	glm::mat4 rotationMatrix(
		cos(angle), sin(angle), 0.0f, 0.0f,
    	-sin(angle), cos(angle), 0.0f, 0.0f,
    	0.0f, 0.0f, 1.0f, 0.0f,
    	0.0f, 0.0f, 0.0, 1.0f 
    );

	// Need to comment out the following line and write your own version
	//rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));

	multMatrix(rotationMatrix);
}

//implement
void MatrixStack::multMatrix(glm::mat4 &matrix)
{	
	//get matrix from the top of the stack
	glm::mat4 &top = mstack->top();
	
	//declaring a temporary matrix 
	glm::mat4 tmp(1.0f); 

	//multiply top and matrix together, store in temp
	//**Column major order 
	for (int i=0; i<4; i++){
	    tmp[0][i] =     (top[0][i] * matrix[0][0]) +
	                    (top[1][i] * matrix[0][1]) +
	                    (top[2][i] * matrix[0][2]) +
	                    (top[3][i] * matrix[0][3]) ;

	    tmp[1][i] =     (top[0][i] * matrix[1][0]) +
	                    (top[1][i] * matrix[1][1]) +
	                    (top[2][i] * matrix[1][2]) +
	                    (top[3][i] * matrix[1][3]) ;

	    tmp[2][i] =     (top[0][i] * matrix[2][0]) +
	                    (top[1][i] * matrix[2][1]) +
	                    (top[2][i] * matrix[2][2]) +
	                    (top[3][i] * matrix[2][3]) ;

	    tmp[3][i] =     (top[0][i] * matrix[3][0]) +
	                    (top[1][i] * matrix[3][1]) +
	                    (top[2][i] * matrix[3][2]) +
	                    (top[3][i] * matrix[3][3]) ;
    }

    //set top equal to temp
    top = tmp;

}

//implement 
//Generates a perspective projection matrix
void MatrixStack::Perspective(float fovy, float aspect, float near, float far)
{
	//calc values to be stored
	float a = 1.0 / (aspect*tanf(fovy/2));
	float b = 1.0 / tanf(fovy/2);
	float c = (near+far)/(far-near);
	float d = - 2.0 * near*far/(far-near);
	float e = - 1.0;

	//create the projection matrix, column major 
	glm::mat4 projectionMatrix(
		a, 0.0f, 0.0f, 0.0f,
    	0.0f, b, 0.0f, 0.0f,
    	0.0f, 0.0f, c, e,
    	0.0f, 0.0f, d, 1.0f 
    );

	// Need to comment out the following line and write your own version
	//projectionMatrix = glm::perspective(fovy, aspect, near, far);

	multMatrix(projectionMatrix);
}

//implement larter
void MatrixStack::LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{	
	//declare the matrix 
	glm::mat4 viewMatrix(1.0f);

	//perform preliminary calcualtions
	glm::vec3  f = glm::normalize(center - eye);
    glm::vec3  u = glm::normalize(up);
    glm::vec3  s = glm::normalize(cross(f, u));
    u = glm::cross(s, f);

    //insert calculated values into the appropriate matrix positions
    viewMatrix[0][0] = s.x;
    viewMatrix[1][0] = s.y;
    viewMatrix[2][0] = s.z;
    viewMatrix[0][1] = u.x;
    viewMatrix[1][1] = u.y;
    viewMatrix[2][1] = u.z;
    viewMatrix[0][2] =-f.x;
    viewMatrix[1][2] =-f.y;
    viewMatrix[2][2] =-f.z;
    viewMatrix[3][0] =-dot(s, eye);
    viewMatrix[3][1] =-dot(u, eye);
    viewMatrix[3][2] = dot(f, eye);
    
	// Need to comment out the following line and write your own version
	//viewMatrix = glm::lookAt(eye, center, up);

	multMatrix(viewMatrix);
}


void MatrixStack::translate(float x, float y, float z)
{
	translate(glm::vec3(x, y, z));
}

void MatrixStack::scale(float x, float y, float z)
{
	scale(glm::vec3(x, y, z));
}

void MatrixStack::scale(float s)
{
	scale(glm::vec3(s, s, s));
}

glm::mat4 &MatrixStack::topMatrix()
{
	return mstack->top();
}

void MatrixStack::print(const glm::mat4 &mat, const char *name)
{
	if(name) {
		printf("%s = [\n", name);
	}
	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j) {
			// mat[j] returns the jth column
			printf("%- 5.2f ", mat[j][i]);
		}
		printf("\n");
	}
	if(name) {
		printf("];");
	}
	printf("\n");
}

void MatrixStack::print(const char *name) const
{
	print(mstack->top(), name);
}
