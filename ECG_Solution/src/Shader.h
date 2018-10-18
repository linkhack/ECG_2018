#pragma once
#include <Gl/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

class Shader
{
private:
	GLuint handle;
	std::string vertexShader, fragmentShader;
	std::unordered_map<std::string, GLint> locations;

	void handleError(GLuint shaderId);
	GLuint loadShaders();
	bool loadShader(std::string source, GLenum shaderType, GLuint& shaderHandle);

	GLint getUniformLocation(std::string location);

	std::string readFile(std::string filePath);
public:
	Shader();
	Shader(std::string vertexShader, std::string fragmentShader);

	void setUniform(std::string uniform, const glm::vec3& value);
	void setUnifrom(GLint location, const glm::vec3& value);
	void setUniform(std::string uniform, const glm::mat4& mat);
	void setUniform(GLint location, const glm::mat4& mat);
	void use();
	void unuse();
	~Shader();

};

