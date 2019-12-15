//This file will require modifications
#version 120


//data provided through calling program.sendAttributeData
attribute vec3 vPositionModel; // in object space
attribute vec3 vNormalModel; // in object space

//constant for all verticies
//passed to shader through program.SendUniformData
uniform mat4 model; //model matrix
uniform mat4 view;	//view matrix
uniform mat4 projection; //projection matrix

uniform mat4 normalMatrix;//helps to obtain the vNormalWorld

struct lightStruct
{
	vec3 position; //light position
	vec3 color; //light color
};

#define NUM_LIGHTS 2

uniform lightStruct lights[NUM_LIGHTS];

// required to calculate the color of each pixel or vertex based on the Phong shading model
uniform vec3 ka; //ambient reflectivity
uniform vec3 kd; //Diffuse reflectivity
uniform vec3 ks; //specular reflectivity
uniform float s; //shininess 

uniform vec3 eye; //adding eye vector for calculation

uniform int light_num;

varying vec3 color;

//convert vPositionModel and vNormalModel from object to world space?

void main()
{	

	//Transform vPositionModel from model to world space
	vec3 vPositionWorld = (model * vec4(vPositionModel, 1.0)).xyz;
	//Transform vNormalModel from Model to world space
	//Normal must be transposed then inversed
	//N = normal at the point after being translated 
	vec3 N = normalize(normalMatrix * vec4(vNormalModel,0.0)).xyz;
	//E = normalize(eye position - vertex position)
	vec3 E = normalize(eye - vPositionWorld);
	//L = point at light source subtracted by point at postition after being translated
	vec3 L = normalize(lights[light_num].position - vPositionWorld);
	//R = Reflected vector caclulated from L and N
	vec3 R = normalize(-reflect(L,N));
	//calculate diffuse 
	vec3 diffuse = kd * max(dot(L,N),0.0);
	//calculate specular
	vec3 specular = ks * pow(max(dot(R,E), 0.0), s); 
	//final equation will be in form
	color = ka + diffuse + specular; 
	gl_Position = projection * view * model * vec4(vPositionModel,1.0);

}




