//This file will require modificaitons
#version 120


varying vec3 color; 
varying vec3 N; //world space Normal vector
uniform vec3 eye; //world space eyevector

void main()
{	

	//calculate angle between normal and the eye
	float magN = sqrt(N.x * N.x + N.y * N.y + N.z * N.z); 
	float magE = sqrt(eye.x * eye.x + eye.y * eye.y + eye.z * eye.z);
	float angle = (acos(dot(N,eye)/(magE * magN))) * (180/3.14);

	//if the angle is close to 90, color = while, else color black
	if(angle < 65 ){
		//color black
		vec3 color = vec3(0,0,0);
		gl_FragColor = vec4(color, 1.0f);
	}
	else{
		//color white
		vec3 color = vec3(1,1,1);
		gl_FragColor = vec4(color, 1.0f);
	}

}
