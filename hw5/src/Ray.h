//Ray class
class Ray{
public:

	//Ray attributes
	glm::vec3 orgin; 
	glm::vec3 direction;
	
	//ray initilization funciton
	Ray(glm::vec3 o, glm::vec3 d){
	 	orgin = o; 
	 	direction = d;
	}
};
