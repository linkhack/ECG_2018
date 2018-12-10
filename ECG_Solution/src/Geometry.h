#pragma once
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <Gl/glew.h>
#include <glm/gtc/constants.hpp>

#include "Shader.h"
#include "Material.h"


using namespace std;

struct GeometryData {
	//Vertex data
	vector<glm::vec3> positions;
	vector<glm::vec3> normals;
	//Indices
	vector<unsigned int> indices;
};

class Geometry
{
private:
	bool isEmpty;
	//Buffers
	GLuint vao;

	GLuint vboPositions;
	GLuint vboNormals;
	GLuint vboIndices;

	//nrOfVertices
	int nrOfVertices;

	//Matrices
	glm::mat4 modelMatrix;

	//Shader and Material stuff like color
	std::shared_ptr<Material> material;
	glm::vec3 color;

public:
	Geometry(glm::mat4 modelMatrix, GeometryData& geometryData, std::shared_ptr<Material> material);
	Geometry(glm::mat4 modelMatrix, GeometryData& geometryData, std::shared_ptr<Shader> shader);

	~Geometry();

	void setColor(glm::vec3 color);

	void draw(glm::mat4 matrix = glm::mat4(1.0f));

	//Construction helper
	static GeometryData createCubeGeometry(float width, float height, float depth);
	static GeometryData createSphereGeometry(float radius, unsigned int longitudeSegments, unsigned int latitudeSegments);
	static GeometryData createCylinderGeometry(float radius, float height, unsigned int segmnets);
	static GeometryData createTorusGeometry(float bigRadius, float smallRadius, unsigned int tubeSections, unsigned int circleSections);
};

