#pragma once
#include "Light.h"
class PointLight :
	public Light
{
private:
	glm::vec3 position;
	glm::vec3 attenuation; //x = quad y=linear z=constant

public:
	PointLight(glm::vec3 color; glm::vec3 position, glm::vec3 attenuation);
	~PointLight();
};

