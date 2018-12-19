#include "LambertMaterial.h"



LambertMaterial::LambertMaterial(std::shared_ptr<Shader> shader, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float specularCoefficient):Material(shader),ambient(ambient),diffuse(diffuse),specular(specular),specularCoefficient(specularCoefficient)
{
}

LambertMaterial::LambertMaterial(std::shared_ptr<Shader> shader):LambertMaterial(shader,glm::vec3(0.05f),glm::vec3(0.9f),glm::vec3(0.3f),10.0f)
{
}

void LambertMaterial::setUniforms()
{
	shader->use();
	shader->setUniform("materialCoefficients.ambient", ambient);
	shader->setUniform("materialCoefficients.diffuse", diffuse);
	shader->setUniform("materialCoefficients.specular", specular);
	shader->setUniform("materialCoefficients.specularCoefficient", specularCoefficient);
	shader->unuse();
}

LambertMaterial::~LambertMaterial()
{
}
