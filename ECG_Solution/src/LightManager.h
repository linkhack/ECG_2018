#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Light.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "Spotlight.h"
class LightManager
{
private:
	std::vector<DirectionalLight> directionalLights;
	std::vector<PointLight> pointLights;
	std::vector<Spotlight> spotLights;
	int pointLightNumber;
	int directionalLightNumber;
public:
	LightManager();
	~LightManager();
	
	const static int maxDirectionalLights;
	const static int maxPointLights;

	void createPointLight(glm::vec3 color, glm::vec3 position, glm::vec3 attenuation);
	void createDirectionalLight(glm::vec3 color, glm::vec3 direction);

	void setUniforms(const std::vector<std::shared_ptr<Shader>>& shaders);
};

