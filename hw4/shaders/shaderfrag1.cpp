//This file will require modificaitons
#version 120

//varaible passed from vertex program to fragment program
varying vec3 color;

void main()
{
	gl_FragColor = vec4(color, 1.0f);
}
