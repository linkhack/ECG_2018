#pragma once
#include "Material.h"
class LambertMaterial :
	public Material
{
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float specularCoefficient;
public:
	LambertMaterial(std::shared_ptr<Shader> shader, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float specularCoefficient);
	LambertMaterial(std::shared_ptr<Shader> shader);
	virtual void setUniforms();
	virtual ~LambertMaterial();
};

