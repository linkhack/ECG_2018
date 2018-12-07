#pragma once
#include "Shader.h"

class Light
{
protected:
	glm::vec3 color;
	virtual void setUniform(std::shared_ptr<Shader>& shader) =  0;
public:
	Light();
	~Light();

	virtual void setUniforms(const std::vector<std::shared_ptr<Shader>>& shaders) final;
};

