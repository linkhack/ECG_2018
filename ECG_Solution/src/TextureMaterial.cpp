#include "TextureMaterial.h"




TextureMaterial::TextureMaterial(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture)
	:TextureMaterial(shader,0.05f,0.8f,0.2f,10.0f,texture)
{
}

TextureMaterial::TextureMaterial(std::shared_ptr<Shader> shader, float ambient, float diffuse, float specular, float specularCoefficient, std::shared_ptr<Texture> texture):Material(shader)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->specularCoefficient = specularCoefficient;
	this->texture = texture;
}


TextureMaterial::~TextureMaterial()
{
}

void TextureMaterial::setUniforms(int textureUnit)
{
	shader->use();
	texture->activateTexture(textureUnit);
	shader->setUniform("materialCoefficients.diffuseTexture", textureUnit);
	shader->setUniform("materialCoefficients.ambient", ambient);
	shader->setUniform("materialCoefficients.diffuse", diffuse);
	shader->setUniform("materialCoefficients.specular", specular);
	shader->setUniform("materialCoefficients.specularCoefficient", specularCoefficient);
	shader->unuse();
}
