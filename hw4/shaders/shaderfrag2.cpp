//This file will require modificaitons
//Shader for phong model, most of the shading will be done here
#version 120

//to be passed to the fragment shader
varying vec3 vPositionWorld;
varying vec3 N;

uniform vec3 ka; //ambient reflectivity
uniform vec3 kd; //Diffuse reflectivity
uniform vec3 ks; //specular reflectivity
uniform float s; //shininess

uniform vec3 eye; //adding eye vector for calculation
uniform int light_num;

struct lightStruct
{
	vec3 position; //light position
	vec3 color; //light color
};

#define NUM_LIGHTS 2

uniform lightStruct lights[NUM_LIGHTS]; 

void main()
{
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
	vec3 color = ka + diffuse + specular; 
	gl_FragColor = vec4(color, 1.0f);
}
