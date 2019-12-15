#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

#include "Camera.h"
#include "Scene.h"

//additional includes
#include "Plane.h"
#include "Sphere.h"


#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1200

//Declaring additional globals for camera use
glm::vec3 eye(0.0f, 0.0f, 7.0f);
glm::vec3 lookat(0.0f, 0.0f, 0.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
float fovy = 45.0;

//creating spheres
Sphere sphere1;
Sphere sphere2;
Sphere sphere3;
Sphere sphere4;

//creating planes
Plane plane1;
Plane plane2;

//creating lights
Light light1;
Light light2;

float frameBuffer[WINDOW_HEIGHT][WINDOW_WIDTH][3];
GLFWwindow *window;

std::vector<Shape*> shapes;
std::vector<Light*> lights;

void ClearFrameBuffer()
{
	memset(&frameBuffer, 0, WINDOW_HEIGHT * WINDOW_WIDTH * 3 * sizeof(float));
}

void Display()
{	
	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, frameBuffer);
}

// Keyboard character callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	switch (key) 
	{
	case 'q':
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;

	default:
		break;
	}
}

//Initializes window
//creates instance of scece class
//creates instance of camera class 
void Init()
{
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment5 - Cody Williams", NULL, NULL);
	glfwSetCharCallback(window, CharacterCallback);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	ClearFrameBuffer();

	//Initialize scene
	Scene scene = Scene();

	//assign shapes and lights to scene
	scene.shapes = shapes; 
	scene.lights = lights;


	//initialize camera
	Camera camera = Camera(WINDOW_WIDTH, WINDOW_HEIGHT, eye, lookat, up, fovy);

	//takes scence as input and takes a picture
	camera.TakePicture(&scene);

	//stores as provate variable in rendered image
	float *renderedImage = camera.GetRenderedImage();

	//returns rendered image and sets to frame buffer through mem copy
	memcpy(frameBuffer, renderedImage, sizeof(float) * WINDOW_HEIGHT * WINDOW_WIDTH * 3);

}


int main()
{		
	//INITIALIZATION OF SHAPE AND LIGHT ATTRIBUTES 
	//***********************************************************************
	//Set attributes of sphere 1
	sphere1.position = glm::vec3(-1.0f, -0.7f, 3.0f);
	sphere1.radius = 0.3;
	sphere1.ka = glm::vec3(0.1, 0.1, 0.1);
	sphere1.kd = glm::vec3(0.2, 1.0, 0.2);
	sphere1.ks = glm::vec3(1.0, 1.0, 1.0);
	sphere1.km = glm::vec3(0.0, 0.0, 0.0);
	sphere1.s = 100.0;

	//Set attributes of sphere 2
	sphere2.position = glm::vec3(1.0, -0.5, 3.0);
	sphere2.radius = 0.5;
	sphere2.ka = glm::vec3(0.1, 0.1, 0.1);
	sphere2.kd = glm::vec3(0.0, 0.0, 1.0);
	sphere2.ks = glm::vec3(1.0, 1.0, 1.0);
	sphere2.km = glm::vec3(0.0, 0.0, 0.0);
	sphere2.s = 10.0;

	//Set attributes of sphere 3
	sphere3.position = glm::vec3(-1.0, 0.0, -0.0);
	sphere3.radius = 1.0;
	sphere3.ka = glm::vec3(0.0, 0.0, 0.0);
	sphere3.kd = glm::vec3(0.0, 0.0, 0.0);
	sphere3.ks = glm::vec3(0.0, 0.0, 0.0);
	sphere3.km = glm::vec3(0.8, 0.8, 0.8);
	sphere3.s = 10.0;

	// sphere3.ka = glm::vec3(0.1, 0.1, 0.1);
	// sphere3.kd = glm::vec3(0.0, 0.0, 1.0);
	// sphere3.ks = glm::vec3(1.0, 1.0, 1.0);
	// sphere3.km = glm::vec3(0.0, 0.0, 0.0);
	// sphere3.s = 10.0;

	//Set attributes of sphere 4
	sphere4.position = glm::vec3(1.0, 0.0, -1.0);
	sphere4.radius = 1.0;
	sphere4.ka = glm::vec3(0.0, 0.0, 0.0);
	sphere4.kd = glm::vec3(0.0, 0.0, 0.0);
	sphere4.ks = glm::vec3(0.0, 0.0, 0.0);
	sphere4.km = glm::vec3(0.8, 0.8, 0.8);
	sphere4.s = 0.0; 
	// sphere4.ka = glm::vec3(0.1, 0.1, 0.1);
	// sphere4.kd = glm::vec3(0.0, 0.0, 1.0);
	// sphere4.ks = glm::vec3(1.0, 1.0, 1.0);
	// sphere4.km = glm::vec3(0.0, 0.0, 0.0);
	// sphere4.s = 10.0;

	//Set attributes of plane 1
	plane1.center = glm::vec3(0.0, -1.0, 0.0);
	plane1.normal = glm::vec3(0.0, 1.0, 0.0);
	plane1.ka = glm::vec3(0.1, 0.1, 0.1);
	plane1.kd = glm::vec3(1.0, 1.0, 1.0);
	plane1.ks = glm::vec3(0.0, 0.0, 0.0);
	plane1.km = glm::vec3(0.0, 0.0, 0.0);
	plane1.s = 0.0;

	//Set attributes of plane 2
	plane2.center = glm::vec3(0.0, 0.0, -3.0);
	plane2.normal = glm::vec3(0.0, 0.0, 1.0);
	plane2.ka = glm::vec3(0.1, 0.1, 0.1);
	plane2.kd = glm::vec3(1.0, 1.0, 1.0);
	plane2.ks = glm::vec3(0.0, 0.0, 0.0);
	plane2.km = glm::vec3(0.0, 0.0, 0.0);
	plane2.s = 0.0;

	//Set attributes for light 1
	light1.position = glm::vec3(0.0, 3.0, -2.0);
	light1.color = glm::vec3(0.2, 0.2, 0.2);

	//Set attributes for light 2
	light2.position = glm::vec3(-2.0, 1.0, 4.0);
	light2.color = glm::vec3(0.5, 0.5, 0.5);
	//***********************************************************************

	//pushing shapes
	shapes.push_back(&sphere1);
	shapes.push_back(&sphere2);
	shapes.push_back(&sphere3);
	shapes.push_back(&sphere4);
	shapes.push_back(&plane1);
	shapes.push_back(&plane2);

	//pushing lights
	lights.push_back(&light1);
	lights.push_back(&light2);


	Init();
	while ( glfwWindowShouldClose(window) == 0) 
	{
		glClear(GL_COLOR_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}