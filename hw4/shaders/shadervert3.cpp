//This file will require modifications
#version 120


//data provided through calling program.sendAttributeData
attribute vec3 vPositionModel; // in object space
attribute vec3 vNormalModel; // in object space

//to be passed to the fragment shader
varying vec3 vPositionWorld;
varying vec3 N;

//constant for all verticies
//passed to shader through program.SendUniformData
uniform mat4 model; //model matrix
uniform mat4 view;	//view matrix
uniform mat4 projection; //projection matrix
uniform mat4 normalMatrix;//helps to obtain the vNormalWorld


void main()
{	

	//Transform vPositionModel from model to world space
	vPositionWorld = (model * vec4(vPositionModel, 1.0)).xyz;
	
	//Transform vNormalModel from Model to world space
	//N = normal at the point after being translated 
	N = normalize(normalMatrix * vec4(vNormalModel,0.0)).xyz;

	gl_Position = projection * view * model * vec4(vPositionModel, 1.0);;

}


