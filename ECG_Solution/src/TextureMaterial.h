#pragma once
#include "LambertMaterial.h"
#include "Shader.h"
#include "Texture.h"
class TextureMaterial :
	public Material
{
private:
	float ambient;
	float diffuse;
	float specular;
	float specularCoefficient;
	std::shared_ptr<Texture> texture;
public:
	TextureMaterial(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture);
	TextureMaterial(std::shared_ptr<Shader> shader, float ambient, float diffuse, float specular, float specularCoefficient, std::shared_ptr<Texture> texture);
	virtual ~TextureMaterial();

	virtual void setUniforms(int textureUnit);
};

