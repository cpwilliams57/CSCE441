//This file will require modifications

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "Program.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define NUM_LIGHTS 2
#define NUM_MATERIALS 3

GLFWwindow *window;

Program programs[3]; //responsible for processing shader programs

//sotre vertex position, normal and texture coordinates 
std::vector<float> posBuff;
std::vector<float> norBuff;
std::vector<float> texBuff;

glm::vec3 eye(0.0f, 0.0f, 4.0f);

int material = 0;//keeps track of material being used in the shader
int light = 0;//keeps track of thr light being used in the shader
int mode = 0; //keeps track of program mode

//stores material parameters
struct materialStruct {
	glm::vec3 ka, kd, ks;
	float s;
} 
materials[NUM_MATERIALS] = {
	{ 
		//material1 
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(0.8f, 0.7f, 0.7f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		10.0
	},
	{
		//material2
		glm::vec3(0.0f, 0.2f, 0.2f),
		glm::vec3(0.5f, 0.7f, 0.2f),
		glm::vec3(0.1f, 1.0f, 0.1f),
		100.0
	},
	{ 
		//material3 
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(0.1f, 0.3f, 0.9f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		1.0
	},

};

//stores light information
struct lightStruct {
	glm::vec3 position;
	glm::vec3 color;
} 
lights[NUM_LIGHTS] = {
	{
		//light1
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.5f, 0.5f, 0.5f)
	},
	{
		//light2
		glm::vec3(0.0f, 3.0f, 0.0f),
		glm::vec3(0.2f, 0.2f, 0.2f)
	}

};


void Display()
{		
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	//Setting projection, model and view matricies
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);
	glm::mat4 viewMatrix = glm::lookAt(eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.2f, -1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//activates shader program so it can be used for drawing
	programs[mode].Bind();
	programs[mode].SendAttributeData(posBuff, "vPositionModel");
	programs[mode].SendAttributeData(norBuff, "vNormalModel");

	//Sends uniform data to the shader programs
	//send the model, view, and projection matrices to the vertex program, 
	//so we can use them to transform the vertices to normalize device coordinate in the vertex program
	
	//apply transformations for our normal
	glm::mat4 normalMatrix = transpose(inverse((modelMatrix)));
	programs[mode].SendUniformData(normalMatrix, "normalMatrix");

	programs[mode].SendUniformData(modelMatrix, "model");
	programs[mode].SendUniformData(viewMatrix, "view");
	programs[mode].SendUniformData(projectionMatrix, "projection");

	//Pass material1 to vertex shadder
	programs[mode].SendUniformData(materials[material].ka, "ka");
	programs[mode].SendUniformData(materials[material].kd, "kd");
	programs[mode].SendUniformData(materials[material].ks, "ks");
	programs[mode].SendUniformData(materials[material].s, "s");

	//send eye to the vertex shader
	programs[mode].SendUniformData(eye, "eye");
	programs[mode].SendUniformData(light, "light_num");

	//Pass light1 to vertex shadder
	programs[mode].SendUniformData(lights[0].position, "lights[0].position");
	programs[mode].SendUniformData(lights[0].color, "lights[0].color");

	//Pass light2 to vertex shadder
	programs[mode].SendUniformData(lights[1].position, "lights[1].position");
	programs[mode].SendUniformData(lights[1].color, "lights[1].color");

	//sending
	glDrawArrays(GL_TRIANGLES, 0, posBuff.size() / 3);
	programs[mode].Unbind();

}

// Keyboard character callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	switch (key) 
	{
	case 'q':
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;

	case '1':
		mode = 0;
		std::cout << "mode: 1" << std::endl;
		break;

	case '2':
		mode = 1;
		std::cout << "mode: 2" << std::endl;
		break;

	case '3':
		mode = 2;
		std::cout << "mode: 3" << std::endl;
		break;

	//Change material
	case 'M':
		if(material > 0)
			material--;
			std::cout << "Material: " << material << std::endl;
		break;

	case 'm':
		if(material < 2)
			material++;
			std::cout << "Material: " << material << std::endl;
		break;

	//change light source
	case 'L':
		if(light > 0)
			light--;
			std::cout << "light: " << light << std::endl;
		break;

	case 'l':
		if(light < 1)
			light++;
			std::cout << "light: " << light << std::endl;
		break;

	//Change light x position
	case 'X':
		lights[light].position[0] = lights[light].position[0] - 0.1;
		std::cout << "lightx : " << lights[light].position[0] << std::endl;
		break;

	case 'x':
		lights[light].position[0] = lights[light].position[0] + 0.1;
		std::cout << "lightx : " << lights[light].position[0] << std::endl;
		break;

	//Change light y position
	case 'Y':
		lights[light].position[1] = lights[light].position[1] - 0.1;
		std::cout << "lighty : " << lights[light].position[1] << std::endl;
		break;

	case 'y':
		lights[light].position[1] = lights[light].position[1] + 0.1;
		std::cout << "lighty : " << lights[light].position[1] << std::endl;
		break;

	//Change light  position
	case 'Z':
		lights[light].position[2] = lights[light].position[2] - 0.1;
		std::cout << "lightz : " << lights[light].position[2] << std::endl;
		break;

	case 'z':
		lights[light].position[2] = lights[light].position[2] + 0.1;
		std::cout << "lightz : " << lights[light].position[2] << std::endl;
		break;


	default:
		break;
	}
}


void FrameBufferSizeCallback(GLFWwindow* lWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

void LoadModel(char* name)
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
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
					if (!attrib.normals.empty()) {
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 0]);
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 1]);
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 2]);
					}
					if (!attrib.texcoords.empty()) {
						texBuff.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
						texBuff.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
					}
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
}

void Init()
{
	glfwInit();
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment4 - Cody Williams", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetCharCallback(window, CharacterCallback);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	LoadModel("../obj/bunny.obj");
	
	//Responsible for setting up shader programs

	//sets the address of the vertex and fragment shader files so we can load and compile them next
	programs[0].SetShadersFileName("../shaders/shadervert1.cpp", "../shaders/shaderfrag1.cpp");
	programs[1].SetShadersFileName("../shaders/shadervert2.cpp", "../shaders/shaderfrag2.cpp");
	programs[2].SetShadersFileName("../shaders/shadervert3.cpp", "../shaders/shaderfrag3.cpp");

	//initialize the programs
	programs[0].Init();
	programs[1].Init();
	programs[2].Init();

}


int main()
{	
	glm::vec3 v1, v2;
    v1.x = 203;
    v1.y = 355;
    v1.z = 922;

    v2.x = 6;
    v2.y = 13;
    v2.z = 198;

	float magN = sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z); 
	float magE = sqrt(v2.x * v2.x + v2.y * v2.y + v2.z * v2.z);
	float angle = (acos(dot(v1,v2)/(magE * magN))) * (180/M_PI);

	std::cout << angle << std::endl;

	Init();
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