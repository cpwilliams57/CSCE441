#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>


#define WINDOW_WIDTH 1800
#define WINDOW_HEIGHT 1200



float frameBuffer[WINDOW_HEIGHT][WINDOW_WIDTH][3];
bool mask[WINDOW_HEIGHT][WINDOW_WIDTH];
GLFWwindow *window;

int brush_size = 2; 

std::vector <double> xpix; 
std::vector <double> ypix;

// Color structure. Can be used to define the brush and background color.
struct color { float r, g, b; };

std::vector <color> pixcolor;

color brush_color;
color bg_color;

std::string brush_type = "square"; 

color red = {1,0,0};
color green = {0,1,0};
color blue = {1,1,0};
color yellow = {0,0,1};
color pink = {1,0,1};
color cyan = {0,1,1};
color white = {1,1,1}; 


// A function clamping the input values to the lower and higher bounds
#define CLAMP(in, low, high) ((in) < (low) ? (low) : ((in) > (high) ? (high) : in))

// Set a particular pixel of the frameBuffer to the provided color
void SetFrameBufferPixel(int x, int y, struct color lc)
{
	// Origin is upper left corner. 
	// Changes the origin from the lower-left corner to the upper-left corner
	y = WINDOW_HEIGHT - 1 - y;

	x = CLAMP(x, 0, WINDOW_WIDTH - 1);
	y = CLAMP(y, 0, WINDOW_HEIGHT - 1);

	frameBuffer[y][x][0] = lc.r;
	frameBuffer[y][x][1] = lc.g;
	frameBuffer[y][x][2] = lc.b;

}

void ClearFrameBuffer()
{
	memset(frameBuffer, 0.0f, sizeof(float) * WINDOW_WIDTH * WINDOW_HEIGHT * 3);
}

// Display frameBuffer on screen
void Display()
{	
	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, frameBuffer);
}

//make rectangle function?? 
void drawRectangle(double xpos, double ypos, color pixelcolor){

	//for loops to draw edges of rectangle
	int edge_length = 2 * brush_size;

	//left edge
	for(int i = 0; i < edge_length; i++){
		SetFrameBufferPixel(xpos - brush_size, (ypos - brush_size) + i , pixelcolor);
		xpix.push_back(xpos - brush_size);
		ypix.push_back((ypos - brush_size) + i );
		pixcolor.push_back(pixelcolor);
	}

	//right edge
	for(int i = 0; i < edge_length; i++){
		SetFrameBufferPixel(xpos + brush_size, (ypos - brush_size) + i , pixelcolor);
		xpix.push_back(xpos + brush_size);
		ypix.push_back((ypos - brush_size) + i );
		pixcolor.push_back(pixelcolor);
	}

	//bottom edge
	for(int i = 0; i < edge_length; i++){
		SetFrameBufferPixel((xpos - brush_size) + i, ypos + brush_size , pixelcolor);
		xpix.push_back((xpos - brush_size)+i );
		ypix.push_back(ypos + brush_size);
		pixcolor.push_back(pixelcolor);
	}

	//top edge
	for(int i = 0; i < edge_length; i++){
		SetFrameBufferPixel((xpos - brush_size) + i, ypos - brush_size , pixelcolor);
		xpix.push_back((xpos - brush_size)+i );
		ypix.push_back(ypos - brush_size);
		pixcolor.push_back(pixelcolor);
	}
}

void drawCircle(double xpos, double ypos, color pixelcolor){
	//draw twor half circles
	// double drawx = xpos - brush_size;

	// for(int i = 0; i < brush_size; i++){
	// 	SetFrameBufferPixel((xpos - brush_size) + i, ypos + i , pixelcolor);
	// }
	
	// // while(drawx < xpos){
	// // 	SetFrameBufferPixel((xpos - brush_size) + i, ypos + i , pixelcolor);
	// // 	i++; 
	// // }

}

void drawSpray(double xpos, double ypos, color pixelcolor){
	for(int i = 0; i < brush_size * 2; i ++){
		for(int j = 0; j < brush_size * 2; j ++){
			if(j%3 == 0){
				xpix.push_back((xpos - brush_size)+i );
				ypix.push_back(ypos - brush_size);
				pixcolor.push_back(pixelcolor);
				SetFrameBufferPixel(xpos - i,ypos - j, pixelcolor);
			}
		}
	}
}

// Mouse position callback function
void CursorPositionCallback(GLFWwindow* lWindow, double xpos, double ypos)
{
	//lines added to accomidate mac retinaß
	xpos *= 2;
	ypos *= 2;
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

	if (state == GLFW_PRESS)
	{
		std::cout << "Mouse position is: x - " << xpos << ", y - " << ypos << std::endl;

		//set a frame buffer value
		if(brush_type == "square"){
			drawRectangle(xpos,ypos,brush_color);
		}
		else if(brush_type == "circle"){
			drawCircle(xpos,ypos,brush_color);
		}
		else if(brush_type == "spray"){
			drawSpray(xpos,ypos,brush_color);
		}

	}
}

// Mouse callback function
void MouseCallback(GLFWwindow* lWindow, int button, int action, int mods)
{
	
	//Draw square of specified sixe to the window
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) 
	{
		std::cout << "left click." << std::endl;
	}

	//Clear the window
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) 
	{
		std::cout << "Window Cleared." << std::endl;
		for(int i = 0; i < WINDOW_WIDTH; i ++){
			for(int j = 0; j < WINDOW_HEIGHT; j ++){
				xpix.clear();
				ypix.clear();
				pixcolor.clear();
				SetFrameBufferPixel(i,j,bg_color);
			}
		}
	}

}


// Keyboard callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	std::cout << "Key" << (char)key << " is pressed." << std::endl;
	//Read keyboard inputs and respond accordingly
	switch(key){

	//Increase and decrease size of rectangle brush	
		case '+':
			std::cout << "Increse Brush Size: " << brush_size << std::endl;
			if(brush_size < 128)
				brush_size = brush_size * 2; 
			break; 

		case '-':
			std::cout << "Decrease Brush Size: " << brush_size << std::endl;
			if (brush_size > 1)
				brush_size = brush_size / 2; 
			break; 

	//Change the Color of the brush
		// case 1 -> 100 -> red
		case '1':
			std::cout << "Brush Color Red." << std::endl;
			brush_color = red; 
			break; 

		// case 2 -> 010 -> green
		case '2':
			std::cout << "Brush Color Green." << std::endl;
			brush_color = green; 
			break;

		// case 3 -> 110 -> blue
		case '3':
			std::cout << "Brush Color yellow." << std::endl;
			brush_color = blue;
			break;

		// case 4 -> 001 -> yellow
		case '4':
			std::cout << "Brush Color blue." << std::endl;
			brush_color = yellow;
			break;

		// case 5 -> 101 -> pink
		case '5':
			std::cout << "Brush Color pink." << std::endl;
			brush_color = pink;
			break;

		// case 6 -> 011 -> cyan
		case '6':
			std::cout << "Brush Color cyan." << std::endl;
			brush_color = cyan;
			break; 

		// case 7 -> 111 -> white 
		case '7':
			std::cout << "Brush Color white." << std::endl;
			brush_color = white;
			break;

		// case 7 -> 111 -> white 
		case '8':
			std::cout << "change to spray brush." << std::endl;
			brush_type = "spray";
			break;

		// case 9 circle brush
		case '9':
			std::cout << "Change to Circle Brush." << std::endl;
			brush_type = "circle";
			break;

		// case 0 -> spray brush 
		case '0':
			std::cout << "Change to square brush." << std::endl;
			brush_type = "square";
			break;



	//Change Background color with Keyboard input
		// case ! -> 100 -> red
		case '!':

		//redraw the background color
		bg_color = red; 
			for(int i = 0; i < WINDOW_WIDTH; i ++){
				for(int j = 0; j < WINDOW_HEIGHT; j ++){
					SetFrameBufferPixel(i,j,red);
				}
			}

		//redrraw the pixels
		for(int i = 0; i < xpix.size(); i++ ){
			SetFrameBufferPixel(xpix.at(i),ypix.at(i),pixcolor.at(i));
		}

			break; 

		// case @ -> 010 -> green
		case '@':
		bg_color = green; 
			for(int i = 0; i < WINDOW_WIDTH; i ++){
				for(int j = 0; j < WINDOW_HEIGHT; j ++){
					SetFrameBufferPixel(i,j,green);
				}
			}

		//redrraw the pixels
		for(int i = 0; i < xpix.size(); i++ ){
			SetFrameBufferPixel(xpix.at(i),ypix.at(i),pixcolor.at(i));
		}
			break;

		// case # -> 110 -> blue
		case '#':
		bg_color = blue; 
			for(int i = 0; i < WINDOW_WIDTH; i ++){
				for(int j = 0; j < WINDOW_HEIGHT; j ++){
					SetFrameBufferPixel(i,j,blue);
				}
			}

		//redrraw the pixels
		for(int i = 0; i < xpix.size(); i++ ){
			SetFrameBufferPixel(xpix.at(i),ypix.at(i),pixcolor.at(i));
		}
			break;

		// case $ -> 001 -> yellow
		case '$':
		bg_color = yellow; 
			for(int i = 0; i < WINDOW_WIDTH; i ++){
				for(int j = 0; j < WINDOW_HEIGHT; j ++){
					SetFrameBufferPixel(i,j,yellow);
				}
			}

		//redrraw the pixels
		for(int i = 0; i < xpix.size(); i++ ){
			SetFrameBufferPixel(xpix.at(i),ypix.at(i),pixcolor.at(i));
		}
			break;

		// case % -> 101 -> pink
		case '%':
		bg_color = pink; 
			for(int i = 0; i < WINDOW_WIDTH; i ++){
				for(int j = 0; j < WINDOW_HEIGHT; j ++){
					SetFrameBufferPixel(i,j,pink);
				}
			}

		//redrraw the pixels
		for(int i = 0; i < xpix.size(); i++ ){
			SetFrameBufferPixel(xpix.at(i),ypix.at(i),pixcolor.at(i));
		}	
			break;

		// case ^ -> 011 -> cyan
		case '^':
		bg_color = cyan; 
			for(int i = 0; i < WINDOW_WIDTH; i ++){
				for(int j = 0; j < WINDOW_HEIGHT; j ++){
					SetFrameBufferPixel(i,j,cyan);
				}
			}

		//redrraw the pixels
		for(int i = 0; i < xpix.size(); i++ ){
			SetFrameBufferPixel(xpix.at(i),ypix.at(i),pixcolor.at(i));
		}
			break; 

		// case & -> 111 -> white 
		case '&':
		bg_color = white; 
			for(int i = 0; i < WINDOW_WIDTH; i ++){
				for(int j = 0; j < WINDOW_HEIGHT; j ++){
					SetFrameBufferPixel(i,j,white);
				}
			}

		//redrraw the pixels
		for(int i = 0; i < xpix.size(); i++ ){
			SetFrameBufferPixel(xpix.at(i),ypix.at(i),pixcolor.at(i));
		}
			break;
	}	
}

//Figure out how and where to draw the rectangles
//is it implemented as mousecallback, mousemove?
//am i on the right track?

void Init()
{
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//changed to accomidate for MAC retina
	window = glfwCreateWindow(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, "Assignment 1 - Cody Williams", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, MouseCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetCharCallback(window, CharacterCallback);
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	ClearFrameBuffer();
}


int main()
{	
	Init();
	while (glfwWindowShouldClose(window) == 0)
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