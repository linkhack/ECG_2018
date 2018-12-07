#include "Light.h"



Light::Light()
{
}


Light::~Light()
{
}

void Light::setUniforms(const std::vector<std::shared_ptr<Shader>>& shaders)
{
	for (std::shared_ptr<Shader> shader : shaders) 
	{
		setUniform(shader);
	}
}
