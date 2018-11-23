#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <Gl/glew.h>
#include "Shader.h"

using namespace std;

struct GeometryData {
	vector<glm::vec3> positions;
	vector<unsigned int> indices;
};

class Geometry
{
private:

	//Buffers
	GLuint vao;

	GLuint vboPositions;
	GLuint vboIndices;

	//nrOfVertices
	int nrOfVertices;

	//Matrices
	glm::mat4 modelMatrix;

	//Shader and Material stuff like color
	std::shared_ptr<Shader> shader;
	glm::vec3 color;

public:
	Geometry(glm::mat4 modelMatrix, GeometryData& geometryData, std::shared_ptr<Shader> shader);
	~Geometry();

	void setColor(glm::vec3 color);

	void draw(glm::mat4 matrix = glm::mat4(1.0f));

	//Construction helper
	static GeometryData createCubeGeometry(float width, float height, float depth);
};

