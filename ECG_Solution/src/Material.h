#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
class Material
{
protected:
	std::shared_ptr<Shader> shader;
public:
	Material(std::shared_ptr<Shader> shader);
	virtual ~Material();
	virtual void setUniforms();
	virtual std::shared_ptr<Shader> getShader() final;
};

