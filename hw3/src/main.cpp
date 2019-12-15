//Modifications will occur in this file
//Must implement rasterization in cpu using RenderCPU

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <array>
#include <string>
#include <vector>
#include <algorithm>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Triangle.h"

#include <random>


#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

//Declare the global frameBuffer
Buffer frameBuffer;

GLFWwindow *window;

//Vector of color values for generating random colors
glm::vec3 colors[] = {
	glm::vec3(1,0,0), //red
	glm::vec3(0,1,0), //green
	glm::vec3(1,1,0), //blue
	glm::vec3(0,0,1), //yellow
	glm::vec3(1,0,1), //pink 
	glm::vec3(0,1,1), //cyan 
	glm::vec3(1,1,1), //white
};

std::vector<Triangle> triangleVector;
bool isOpenGL = true;
float eyeDistance = 5.0f;

glm::vec3 white(1.0, 1.0, 1.0);

void ClearFrameBuffer()
{
	//Initialize color buffer
	memset(frameBuffer.color, 0.0f, sizeof(float) * WINDOW_WIDTH * WINDOW_HEIGHT * 3);
	
	//Loop over each pixel in the depth buffer and set to a large number for initialization
	for(int i = 0; i < 1000; i++){
		for(int j = 0; j < 1000; j++)
			frameBuffer.depth[i][j] = 10^30; 
	}
}

//display functions 
//loop over all triangles, for each triangle, call render function
void Display()
{	
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.1f, 100.0f);
	glm::mat4 modelViewMatrix = glm::lookAt(eyeDistance * glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//draw in openGL mode
	if (isOpenGL)
	{
		for (int i = 0; i < triangleVector.size(); i++)
			triangleVector[i].RenderOpenGL(modelViewMatrix, projectionMatrix);
	}
	else //dreaw in CPU mode
	{

		//clear the frame buffer color and depth
		ClearFrameBuffer();

		for (int i = 0; i < triangleVector.size(); i++)
			//pass matricies and where to render, maybe pass the buffer here...
			triangleVector[i].RenderCPU(modelViewMatrix, projectionMatrix, frameBuffer);

		glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, frameBuffer.color);
	}
}

//Helper function for generating random colors 
glm::vec3 RandomColor(){
 	//genrate random int between 0 and 6  
 	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(0,6); // guaranteed unbiased
	auto random_integer = uni(rng);
	return colors[random_integer];
}

//Helper function for scaling the z values of the vartexes 
//Used for color mode 2 implementation
float ScaleZvalue(float zval){
	float scaled = ((zval - (-0.77594))/(0.77594 -(-0.77594))) + 1;
	return scaled; 
}

// Keyboard character callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	switch (key)
	{
		//color at all three verticies will be the same
		case '0':{
			std::cout << "Color Mode 0" << std::endl;
			//loop through all of the triangles
			for(int i = 0; i < triangleVector.size(); i++){
				//generate a single random color
				glm::vec3 randCol = RandomColor();
				
				//Assign the same color to each vertex in the triangle
				triangleVector[i].c[0] = randCol;
				triangleVector[i].c[1] = randCol; 
				triangleVector[i].c[2] = randCol;
			}

			break;
		}

		//Color Mode 1 
		case '1':{
			std::cout << "Color Mode 2" << std::endl;
			
			//loop through all of the triangles
			for(int i = 0; i < triangleVector.size(); i++){
				
				//Assign a different random color to each vertex in the triangle
				triangleVector[i].c[0] = RandomColor();
				triangleVector[i].c[1] = RandomColor(); 
				triangleVector[i].c[2] = RandomColor();
			}

			break;
		}

		//Color mode 2 
		case '2':{
			std::cout << "Color Mode 2" << std::endl;
			//loop through all of the triangles
			for(int i = 0; i < triangleVector.size(); i++){
				
				//Get all 3 z values from the verticie coordinates and scale
				float z1 = (ScaleZvalue(triangleVector[i].v[0][2])) - 1;
				float z2 = (ScaleZvalue(triangleVector[i].v[1][2])) - 1;
				float z3 = (ScaleZvalue(triangleVector[i].v[2][2])) - 1;

				//put the scaled z values into the color vector
				triangleVector[i].c[0] = glm::vec3(z1,0,0);
				triangleVector[i].c[1] = glm::vec3(z2,0,0);
				triangleVector[i].c[2] = glm::vec3(z3,0,0);
			}

			break;
		}

		//Move object view out
		case 'w':{
			eyeDistance *= (1 - 0.05);
				break;
		}
		
		//move object view in
		case 's':{
			eyeDistance *= (1 + 0.05);
				break;
		}
		
		//change between OpenGL and CPU rasterization
		case ' ':{
			isOpenGL = !isOpenGL;
				break;
		}
		
		//close window 
		case 'q':{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
		}
	}
}

//Is this where color will be created? 
void CreateTriangleVector(std::vector<glm::vec3> &vertices)
{
	for (int i = 0; i < vertices.size() / 3; i++)
	{
		Triangle myTriangle(vertices[i * 3 + 0], vertices[i * 3 + 1], vertices[i * 3 + 2], white, white, white);
		triangleVector.push_back(myTriangle);
	}
}

void LoadModel(char* name, std::vector<glm::vec3> &vertices)
{
	// Taken from Shinjiro Sueda with slight modification
	std::string meshName(name);
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	if (!rc) {
		std::cerr << errStr << std::endl;
	}
	else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					vertices.push_back(glm::vec3(attrib.vertices[3 * idx.vertex_index + 0],
												 attrib.vertices[3 * idx.vertex_index + 1],
												 attrib.vertices[3 * idx.vertex_index + 2]));
				}
				index_offset += fv;
			}
		}
	}
}

void Init(char* path)
{
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment3 - Cody Williams", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetCharCallback(window, CharacterCallback);
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	ClearFrameBuffer();
	std::vector<glm::vec3> vertices;
	
	//take path to obj file as arguement
	LoadModel(path, vertices);

	CreateTriangleVector(vertices);



	
}

int main(int argc, char *argv[])
{	
	//std::string path = "../obj/duck.obj";
	char* path =  argv[1];
	Init(path);

	while ( glfwWindowShouldClose(window) == 0) 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}