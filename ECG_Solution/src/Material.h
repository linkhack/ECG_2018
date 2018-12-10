#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
class Material
{
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float specularCoefficient; 
	std::shared_ptr<Shader> shader;
public:
	Material(std::shared_ptr<Shader> shader,glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,float specularCoefficient);
	Material(std::shared_ptr<Shader> shader);
	~Material();
	void setUniforms();
	std::shared_ptr<Shader> getShader();
};

