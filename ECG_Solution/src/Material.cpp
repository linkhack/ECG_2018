#include "Material.h"


Material::Material(std::shared_ptr<Shader> shader, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float specularCoefficient):shader(shader),ambient(ambient),diffuse(diffuse),specular(specular),specularCoefficient(specularCoefficient)
{
}

Material::Material(std::shared_ptr<Shader> shader):shader(shader)
{
	ambient = glm::vec3(0.05f, 0.05, 0.05f);
	diffuse = glm::vec3(0.9f, 0.9f, 0.9f);
	specular = glm::vec3(0.1f, 0.1f, 0.1f);
	specularCoefficient = 20.0f;
}

Material::~Material()
{
}

void Material::setUniforms()
{
	shader->use();
	shader->setUniform("materialCoefficients.ambient", ambient);
	shader->setUniform("materialCoefficients.diffuse", diffuse);
	shader->setUniform("materialCoefficients.specular", specular);
	shader->setUniform("materialCoefficients.specularCoefficient", specularCoefficient);
	shader->unuse();
}

std::shared_ptr<Shader> Material::getShader()
{
	return shader;
}
