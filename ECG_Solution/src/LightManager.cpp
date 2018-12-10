#include "LightManager.h"

const int LightManager::maxDirectionalLights = 3;
const int LightManager::maxPointLights = 3;

LightManager::LightManager()
{
}


LightManager::~LightManager()
{
}


void LightManager::createPointLight(glm::vec3 color, glm::vec3 position, glm::vec3 attenuation)
{
	if (pointLightNumber < maxPointLights) {
		pointLights.emplace_back(PointLight(color, position, attenuation));
		++pointLightNumber;
	}
	else 
	{
		std::cout << "Already maximum number of lights defined" << std::endl;
	}
}

void LightManager::createDirectionalLight(glm::vec3 color, glm::vec3 direction)
{
	if (directionalLightNumber < maxDirectionalLights)
	{
		directionalLights.emplace_back(DirectionalLight(color, direction));
		++directionalLightNumber;
	}
	else
	{
		std::cout << "Already maximum number of lights defined" << std::endl;
	}
}

void LightManager::setUniforms(const std::vector<std::shared_ptr<Shader>>& shaders)
{
	for (std::shared_ptr<Shader> shader : shaders)
	{
		shader->use();
		shader->setUniform("nrPointLight", pointLightNumber);
		shader->setUniform("nrDirLight", directionalLightNumber);
	}
	for (int i = 0; i < directionalLights.size(); ++i) {
		directionalLights[i].setUniforms(shaders, i);
	}
	for (int i = 0; i < pointLights.size(); ++i) {
		pointLights[i].setUniforms(shaders, i);
	}
}
