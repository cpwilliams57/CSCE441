#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include "MatrixStack.h"
#include "Program.h" 


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800


GLFWwindow *window;

//camera variables
glm::vec3 eye(0.0f, 0.0f, 8.0f);
glm::vec3 center(0.0f, 0.0f, 0.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);

//selection variables
int sel = 0; 
float sv[] = {0.6, 0.3, 0.3, 0.25, 0.25, 0.2, 0.2, 0.2, 0.2};

//rotation vectors
glm::vec3 rotvals[] = {
	glm::vec3 (glm::radians(0.0f),glm::radians(0.0f),glm::radians(0.0f)), //Body
	glm::vec3 (glm::radians(0.0f),glm::radians(0.0f),glm::radians(90.0f)), // up left arm
	glm::vec3 (glm::radians(0.0f),glm::radians(0.0f),glm::radians(90.0f)), // up right arm 
	glm::vec3 (glm::radians(0.0f),glm::radians(0.0f),glm::radians(0.0f)),  // up left leg
	glm::vec3 (glm::radians(0.0f),glm::radians(0.0f),glm::radians(0.0f)),  // up right leg
	glm::vec3 (glm::radians(0.0f),glm::radians(0.0f),glm::radians(90.0f)), // low left arm 
	glm::vec3 (glm::radians(0.0f),glm::radians(0.0f),glm::radians(90.0f)), // low right arm
	glm::vec3 (glm::radians(0.0f),glm::radians(0.0f),glm::radians(0.0f)),  // low left leg
	glm::vec3 (glm::radians(0.0f),glm::radians(0.0f),glm::radians(0.0f)),  // low right leg 
};

glm::vec3 trans[] = {
glm::vec3 (-2.0f, 0.5f, 0.0f), //Body
	glm::vec3 (-3.0f, 1.0f, 0.0f), // up left arm 
	glm::vec3 (-1.0f, 1.0f, 0.0f), // up right arm 
	glm::vec3 (-2.5f, -1.2f, 0.0f),  // up left leg
	glm::vec3 (-1.6f, -1.2f, 0.0f),  // up right leg
	glm::vec3 (-4.0f, 1.0f, 0.0f), // low left arm 
	glm::vec3 (0.0f, 1.0f, 0.0f), // low right arm
	glm::vec3 (-2.5f, -2.0f, 0.0f),  // low left leg
	glm::vec3 (-1.6f, -2.0f, 0.0f),  // low right leg
};


Program program;

//Global variable 
MatrixStack modelViewProjectionMatrix;

//zoom global variable
double zoom = 60; 

//mouse position global variables
double xmouse = 380; 
double ymouse = 400; 


// Draw cube on screen
void DrawCube(glm::mat4& modelViewProjectionMatrix)
{
	program.SendUniformData(modelViewProjectionMatrix, "mvp");
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

class RobotElements{
private:

public:

	//robot elements constructor
	RobotElements(){
	}		

	//Constructing elements of the robot tree
	void drawHead(){//draw head
		modelViewProjectionMatrix.pushMatrix();
		modelViewProjectionMatrix.translate(-2.0f, 2.0f, 0.0f);
		modelViewProjectionMatrix.rotateX(glm::radians(0.0f));
		modelViewProjectionMatrix.rotateY(glm::radians(0.0f));
		modelViewProjectionMatrix.scale(0.2f);
		DrawCube(modelViewProjectionMatrix.topMatrix());
		modelViewProjectionMatrix.popMatrix();	
	}

	void drawLLarm(){//lower left arm
		modelViewProjectionMatrix.pushMatrix();
		modelViewProjectionMatrix.translate(trans[5]);
		modelViewProjectionMatrix.rotateX(rotvals[5].x);
		modelViewProjectionMatrix.rotateY(rotvals[5].y);
		modelViewProjectionMatrix.rotateZ(rotvals[5].z);
		modelViewProjectionMatrix.scale(sv[5]);
		DrawCube(modelViewProjectionMatrix.topMatrix());
		modelViewProjectionMatrix.popMatrix();
	}

	void drawULarm(){//upper left arm
		modelViewProjectionMatrix.pushMatrix();
		modelViewProjectionMatrix.translate(trans[1]);
		modelViewProjectionMatrix.rotateX(rotvals[1].x);
		modelViewProjectionMatrix.rotateY(rotvals[1].y);
		modelViewProjectionMatrix.rotateZ(rotvals[1].z);
		modelViewProjectionMatrix.scale(sv[1]);
		DrawCube(modelViewProjectionMatrix.topMatrix());
		modelViewProjectionMatrix.popMatrix();

		//call method to draw lower right arm
		drawLLarm();
	}

	void drawLRarm(){//lower right arm
		//robot lower right arm
		modelViewProjectionMatrix.pushMatrix();
		modelViewProjectionMatrix.translate(trans[6]);
		modelViewProjectionMatrix.rotateX(rotvals[6].x);
		modelViewProjectionMatrix.rotateY(rotvals[6].y);
		modelViewProjectionMatrix.rotateZ(rotvals[6].z);
		modelViewProjectionMatrix.scale(sv[6]);
		DrawCube(modelViewProjectionMatrix.topMatrix());
		modelViewProjectionMatrix.popMatrix();
	}

	void drawURarm(){//upper right arm
		//robot upper right arm
		modelViewProjectionMatrix.pushMatrix();
		modelViewProjectionMatrix.translate(trans[2]);
		modelViewProjectionMatrix.rotateX(rotvals[2].x);
		modelViewProjectionMatrix.rotateY(rotvals[2].y);
		modelViewProjectionMatrix.rotateZ(rotvals[2].z);
		modelViewProjectionMatrix.scale(sv[2]);
		DrawCube(modelViewProjectionMatrix.topMatrix());
		modelViewProjectionMatrix.popMatrix();

		//call method to draw lower right arm 
		drawLRarm();
	}

	void drawLLleg(){//lower left leg
		//robot lower left leg
		modelViewProjectionMatrix.pushMatrix();
		modelViewProjectionMatrix.translate(trans[7]);
		modelViewProjectionMatrix.rotateX(rotvals[7].x);
		modelViewProjectionMatrix.rotateY(rotvals[7].y);
		modelViewProjectionMatrix.rotateZ(rotvals[7].z);
		modelViewProjectionMatrix.scale(sv[7]);
		DrawCube(modelViewProjectionMatrix.topMatrix());
		modelViewProjectionMatrix.popMatrix();
	}

	void drawULleg(){//upper left leg
		//robot upper left leg
		modelViewProjectionMatrix.pushMatrix();
		modelViewProjectionMatrix.translate(trans[3]);
		modelViewProjectionMatrix.rotateX(rotvals[3].x);
		modelViewProjectionMatrix.rotateY(rotvals[3].y);
		modelViewProjectionMatrix.rotateZ(rotvals[3].z);
		modelViewProjectionMatrix.scale(sv[3]);
		DrawCube(modelViewProjectionMatrix.topMatrix());
		modelViewProjectionMatrix.popMatrix();

		//draw lower left leg
		drawLLleg();
	}

	void drawLRleg(){//lower right leg
		modelViewProjectionMatrix.pushMatrix();
		modelViewProjectionMatrix.translate(trans[8]);
		modelViewProjectionMatrix.rotateX(rotvals[8].x);
		modelViewProjectionMatrix.rotateY(rotvals[8].y);
		modelViewProjectionMatrix.rotateZ(rotvals[8].z);
		modelViewProjectionMatrix.scale(sv[8]);
		DrawCube(modelViewProjectionMatrix.topMatrix());
		modelViewProjectionMatrix.popMatrix();
	}

	void drawURleg(){//upper right leg
		//robot upper right leg
		modelViewProjectionMatrix.pushMatrix();
		modelViewProjectionMatrix.translate(trans[4]);
		modelViewProjectionMatrix.rotateX(rotvals[4].x);
		modelViewProjectionMatrix.rotateY(rotvals[4].y);
		modelViewProjectionMatrix.rotateY(rotvals[4].z);
		modelViewProjectionMatrix.scale(sv[4]);
		DrawCube(modelViewProjectionMatrix.topMatrix());
		modelViewProjectionMatrix.popMatrix();

		//call draw lower right leg
		drawLRleg();
	}

	void drawBody(){ //draw body 
		modelViewProjectionMatrix.pushMatrix();
		modelViewProjectionMatrix.translate(trans[0]);
		modelViewProjectionMatrix.rotateX(rotvals[0].x);
		modelViewProjectionMatrix.rotateY(rotvals[0].y);
		modelViewProjectionMatrix.rotateY(rotvals[0].z);
		modelViewProjectionMatrix.scale(sv[0]);
		DrawCube(modelViewProjectionMatrix.topMatrix());
		modelViewProjectionMatrix.popMatrix();

		//call methods for drawing other body parts 
		drawHead();
		drawULarm();
		drawURarm();
		drawULleg();
		drawURleg();
	}

	//robot relements destructor
	~RobotElements() {}
};

//Declare our robot 
RobotElements myRobot; 

void ConstructRobot()
{	
	//recursive call for robot construction
	myRobot.drawBody();

	//invocation of robot elemnts follows the pattern below...
	//1. 		draw Body()
		//2.		draw head()
		//3.		draw upper Left arm ()
			//4. 		draw lower left arm ()
		//5.		draw upper right arm ()
			//6. 		draw lower right arm ()
		//7.		draw upper right arm ()
			//8. 		draw lower right arm ()
		//9.		draw upper left leg ()
			//10.  		draw lower left leg ()
		//11.		draw upper right leg ()
			//12.  		draw lower right leg ()
}


//****************************************************************

//responsible for drawing cubes to the screen
void Display()
{	


	program.Bind();

	//sets the top matrix to identity and creates a copy of top
	modelViewProjectionMatrix.loadIdentity();
	modelViewProjectionMatrix.pushMatrix();

	// Setting the view and Projection matrices
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	//radians variable handles zoom in/out
	modelViewProjectionMatrix.Perspective(glm::radians(zoom), float(width) / float(height), 0.1f, 100.0f);
	modelViewProjectionMatrix.LookAt(eye, center, up);

	//implement arcball here
	//rotate the whole world
	
	//displaying our robot with variations
	ConstructRobot();

	modelViewProjectionMatrix.popMatrix();
	program.Unbind();
	
}

//Need to fill to handle input
// Mouse callback function
void MouseCallback(GLFWwindow* lWindow, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && GLFW_PRESS == action)
		std::cout << "Mouse left button is pressed." << std::endl;
}

//look ata a
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

	if (yoffset < 0){
		zoom = zoom + 1;
		std::cout << "zoom in: " << zoom << std::endl; 
		Display();
	}
	else{
		zoom = zoom - 1;
		std::cout << "zoom out: " << zoom << std::endl; 
		//Display(); 
	}
}

// Need to fill to handle input
// Mouse position callback function
void CursorPositionCallback(GLFWwindow* lWindow, double xpos, double ypos)
{

	//This is where camera movement about center will be implemented
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS){

		for(int i = 0; i < 9; i++){
			rotvals[i].x = glm::radians(ypos);
			rotvals[i].y = glm::radians(xpos);
		}
	}

	//This is where camera translation will be implemented
	int lstate = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	if (lstate == GLFW_PRESS){
		
		//Xtranslation
		if( xpos > xmouse){
			//if mouse is moving right, increase x translate
			center[0] = center[0] + .1; 
		}
		else{
			//if mouse is moving left, decrease x translate
			center[0] = center[0] - .1; 
		}
		//set xmouse to xpos
		xmouse = xpos;
		
		//Y translation
		if( ypos < ymouse){
			//if mouse is moving up, increaase y translate
			center[1] = center[1] + .1; 
		}
		else{
			//if mouse is moving down, decrease y translate
			center[1] = center[1] - .1; 
		}
		ymouse = ypos;
	}

}

// Need to fill to handle input
// Keyboard character callback function
//robot functianality will be implemented here
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	//std::cout << "Key " << (char)key << " is pressed." << std::endl;
	switch(key){
		//heirarchy forward
		case '.':
			std::cout << "heirarchy forward" << std::endl;
			sel = sel + 1; 
			sv[sel] = sv[sel] + .03;
			sv[sel-1] = sv[sel-1] - .03;  
			break; 

		//heirarchy backward
		case ',':
			std::cout << "heirarchy backward" << std::endl;
			sel = sel - 1; 
			sv[sel] = sv[sel] + .03;
			sv[sel+1] = sv[sel+1] - .03;
			break; 	

		//decrease x angle
		case 'x':
			std::cout << "decerase x angle" << std::endl;
			rotvals[sel].x = rotvals[sel].x + glm::radians(-10.0f);
			
			//rotate arms
			if(sel > 0 && sel < 3){
				rotvals[sel+4].x = rotvals[sel+4].x + glm::radians(-10.0f);
			}
			//rotate legs 
			if(sel > 2 && sel < 5){
				rotvals[sel+4].x = rotvals[sel+4].x + glm::radians(-10.0f);
				
			}
			break; 	

		//increase x angle
		case 'X':
			std::cout << "increase x angle" << std::endl;
			rotvals[sel].x = rotvals[sel].x + glm::radians(10.0f);
			
			//rotate arms
			if(sel > 0 && sel < 3){
				rotvals[sel+4].x = rotvals[sel+4].x + glm::radians(10.0f);
			}
			//rotate legs 
			if(sel > 2 && sel < 5){
				rotvals[sel+4].x = rotvals[sel+4].x + glm::radians(10.0f);
		
			}
			break;

		//decrease y angle
		case 'y':
			std::cout << "decrease y angle" << std::endl;
			rotvals[sel].y = rotvals[sel].y + glm::radians(-10.0f);
			//rotate legs 
			if(sel > 2 && sel < 5){
				rotvals[sel+4].y = rotvals[sel+4].y + glm::radians(-10.0f);
			}
			//rotate arms
			if(sel > 0 && sel < 3){
				//rotate radians in the y axis
				rotvals[sel+4].y = rotvals[sel+4].y + glm::radians(-10.0f);
			}
			break; 

		//increase y angle
		case 'Y':
			std::cout << "increase y angle" << std::endl;
			rotvals[sel].y = rotvals[sel].y + glm::radians(10.0f);
			//legs
			if(sel > 2 && sel < 5){
				rotvals[sel+4].y = rotvals[sel+4].y + glm::radians(10.0f);
			}
			//arms
			if(sel > 0 && sel < 3){
				rotvals[sel+4].y = rotvals[sel+4].y + glm::radians(10.0f);
				
			}
			break;  
		
		//decrease x angle
		case 'z':
			std::cout << "decrease z angle" << std::endl;
			rotvals[sel].z = rotvals[sel].z + glm::radians(-10.0f);

			if(sel > 2 && sel < 5){
				rotvals[sel+4].z = rotvals[sel+4].z + glm::radians(-10.0f);
			}
			//arms
			if(sel > 0 && sel < 3){
				rotvals[sel+4].z = rotvals[sel+4].z + glm::radians(-10.0f);
			
			}


			break; 	

		//increase z angle
		case 'Z':
			std::cout << " increase z angle " << std::endl;
			rotvals[sel].z = rotvals[sel].z + glm::radians(10.0f);

			//legs
			if(sel > 2 && sel < 5){
				rotvals[sel+4].z = rotvals[sel+4].z+ glm::radians(10.0f);
			}
			//arms
			if(sel > 0 && sel < 3){
				rotvals[sel+4].z = rotvals[sel+4].z + glm::radians(10.0f);
				//THIS
				// //perform translation in x,z dimension
				// trans[sel+4].x = ((trans[sel+4].x) -cos(rotvals[sel+4].y)); //some modification of x coord
				// trans[sel+4].z = ((trans[sel+4].z) -sin(rotvals[sel+4].y)); //some modification of z chord
			}

			break; 
	}

}

float cubeVerts[] = {
		// Face x-
		-1.0f,	+2.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		-1.0f,	+2.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		-1.0f,	-2.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		-1.0f,	-2.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		-1.0f,	+2.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		-1.0f,	-2.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		// Face x+
		+1.0f,	+2.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		+1.0f,	-2.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		+1.0f,	+2.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		+1.0f,	+2.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		+1.0f,	-2.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		+1.0f,	-2.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		
		// Face y-
		+1.0f,	-2.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	-2.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		+1.0f,	-2.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		+1.0f,	-2.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	-2.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	-2.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		// Face y+
		+1.0f,	+2.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		+1.0f,	+2.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	+2.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	+2.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		+1.0f,	+2.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	+2.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		

		// Face z-
		+1.0f,	+2.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		+1.0f,	-2.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	+2.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	+2.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		+1.0f,	-2.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	-2.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		// Face z+
		+1.0f,	+2.0f,	+1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	+2.0f,	+1.0f,	0.2f,	0.2f,	0.8f,
		+1.0f,	-2.0f,	+1.0f,	0.2f,	0.2f,	0.8f,
		+1.0f,	-2.0f,	+1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	+2.0f,	+1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	-2.0f,	+1.0f,	0.2f,	0.2f,	0.8f
	};


//Creates array of verticies and their colors representing a cube
void CreateCube()
{

	GLuint vertBufferID;
	glGenBuffers(1, &vertBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
	GLint posID = glGetAttribLocation(program.GetPID(), "position");
	glEnableVertexAttribArray(posID);
	glVertexAttribPointer(posID, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	GLint colID = glGetAttribLocation(program.GetPID(), "color");
	glEnableVertexAttribArray(colID);
	glVertexAttribPointer(colID, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

}

void FrameBufferSizeCallback(GLFWwindow* lWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

//Initializes window, events and shader programs
void Init()
{
	glfwInit();
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment2 - Cody Williams", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetMouseButtonCallback(window, MouseCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCharCallback(window, CharacterCallback);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	program.SetShadersFileName("../shaders/shader.vert", "../shaders/shader.frag");
	program.Init();

	//creates array of verticies and their colors representing a cube
	CreateCube();

	//Calling construct robot in init as indicated
	ConstructRobot();
}


int main()
{	
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
