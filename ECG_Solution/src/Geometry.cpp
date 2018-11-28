#include "Geometry.h"



Geometry::Geometry(glm::mat4 modelMatrix, GeometryData& geometryData, std::shared_ptr<Shader> shader):modelMatrix(modelMatrix), shader(shader), nrOfVertices(geometryData.indices.size())
{
	//Create Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//create vertex position array
	glGenBuffers(1, &vboPositions);
	glBindBuffer(GL_ARRAY_BUFFER, vboPositions);
	glBufferData(GL_ARRAY_BUFFER, geometryData.positions.size() * sizeof(glm::vec3), geometryData.positions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Bind vertex positions to location 0
	glEnableVertexAttribArray(0);


	//create Index Array
	glGenBuffers(1, &vboIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometryData.indices.size() * sizeof(unsigned int), geometryData.indices.data(), GL_STATIC_DRAW);

	//Reset all bindings to 0
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//set color
	color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	isEmpty = false;
}


Geometry::~Geometry()
{
	if (isEmpty)
	{
		glDeleteBuffers(1, &vboIndices);
		glDeleteBuffers(1, &vboPositions);
		glDeleteVertexArrays(1, &vao);
	}
}

void Geometry::setColor(glm::vec3 color)
{
	this->color = color;
}


void Geometry::draw(glm::mat4 matrix)
{
	//Combined Matrices, would allow hierarchical animation later
	glm::mat4 totalMatrix = matrix * modelMatrix;
	//set Model Uniforms
	shader->use();
	shader->setUniform("modelMatrix", totalMatrix);
	shader->setUniform("materialColor", color);
	//Bind Buffers
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, nrOfVertices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

GeometryData Geometry::createCubeGeometry(float width, float height, float depth)
{
	GeometryData data;

	//Vertices position
	data.positions = {
		//front
		glm::vec3(-width / 2.0f, height / 2.0f, depth / 2.0f),
		glm::vec3(-width / 2.0f, -height / 2.0f, depth / 2.0f),
		glm::vec3(width / 2.0f, -height / 2.0f, depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f, depth / 2.0f),

		//back
		glm::vec3(width / 2.0f, height / 2.0f, -depth / 2.0f),
		glm::vec3(width / 2.0f, -height / 2.0f, -depth / 2.0f),
		glm::vec3(-width / 2.0f, -height / 2.0f, -depth / 2.0f),
		glm::vec3(-width / 2.0f, height / 2.0f, -depth / 2.0f),

		//top
		glm::vec3(-width / 2.0f, height / 2.0f, depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f, depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f, -depth / 2.0f),
		glm::vec3(-width / 2.0f, height / 2.0f, -depth / 2.0f),

		//bottom
		glm::vec3(width / 2.0f, -height / 2.0f, -depth / 2.0f),
		glm::vec3(-width / 2.0f, -height / 2.0f, -depth / 2.0f),
		glm::vec3(-width / 2.0f, -height / 2.0f, depth / 2.0f),
		glm::vec3(width / 2.0f, -height / 2.0f, depth / 2.0f),

		//left
		glm::vec3(-width / 2.0f, height / 2.0f, -depth / 2.0f),
		glm::vec3(-width / 2.0f, -height / 2.0f, -depth / 2.0f),
		glm::vec3(-width / 2.0f, -height / 2.0f, depth / 2.0f),
		glm::vec3(-width / 2.0f, height / 2.0f, depth / 2.0f),

		//right
		glm::vec3(width / 2.0f, -height / 2.0f, depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f, depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f, -depth / 2.0f),
		glm::vec3(width / 2.0f, -height / 2.0f, -depth / 2.0f)
	};

	data.indices = {
		//Front
		0,1,2,
		2,3,0,

		//Back
		4, 5, 6,
		6, 7, 4,

		//Top
		8, 9, 10,
		10, 11, 8,

		//left
		12,13,14,
		14,15,12,

		//right
		16,17,18,
		18,19,16
	};

	return data;
}

GeometryData Geometry::createSphereGeometry(float radius, unsigned int longitudeSegments, unsigned int latitudeSegments)
{
	GeometryData data;
	
	data.positions.push_back(glm::vec3(0, +radius, 0));
	data.positions.push_back(glm::vec3(0.0f, -radius, 0.0f));
	//first ring indices
	for (unsigned int i = 0; i < longitudeSegments; ++i)
	{
		data.indices.push_back(0);
		data.indices.push_back(i + 2);
		data.indices.push_back(i == longitudeSegments-1? 2 : i+3);

		data.indices.push_back(1);
		data.indices.push_back(i == longitudeSegments - 1 ? (latitudeSegments - 2)*longitudeSegments + 3 : (latitudeSegments - 2)*longitudeSegments + i + 3);
		data.indices.push_back((latitudeSegments - 2)*longitudeSegments + 2 + i);
	}

	//construct rings and vertices
	for (unsigned int latIndex = 1; latIndex < latitudeSegments; ++latIndex) {
		float verticalAngle = float(latIndex) * glm::pi<float>() / float(latitudeSegments);
		for (unsigned int longIndex = 0; longIndex < longitudeSegments; ++longIndex) {
			float horizontalAngle = 2*float(longIndex) * glm::pi<float>() / float(longitudeSegments);
			data.positions.push_back(glm::vec3(
				radius * glm::sin(verticalAngle) * glm::cos(horizontalAngle),
				radius * glm::cos(verticalAngle),
				radius * glm::sin(verticalAngle) * glm::sin(horizontalAngle)
			));
			if (latIndex == 1) continue;
			data.indices.push_back((latIndex - 1)*longitudeSegments + longIndex + 2);
			data.indices.push_back(longIndex==longitudeSegments-1? (latIndex - 2)*longitudeSegments + 2 :(latIndex - 2)*longitudeSegments + longIndex + 3);
			data.indices.push_back((latIndex - 2)*longitudeSegments + longIndex + 2);

			data.indices.push_back((latIndex - 1)*longitudeSegments + longIndex + 2);
			data.indices.push_back(longIndex == longitudeSegments - 1 ? (latIndex - 1)*longitudeSegments + 2 : (latIndex - 1)*longitudeSegments + longIndex + 3);
			data.indices.push_back(longIndex == longitudeSegments - 1 ? (latIndex - 2)*longitudeSegments + 2 : (latIndex - 2)*longitudeSegments + longIndex + 3);
		}
	}

	return data;
}

GeometryData Geometry::createCylinderGeometry(float radius, float height, unsigned int segmnets)
{
	GeometryData data;

	//bottome
	glm::vec3(0, -height / 2.0f, 0.0f);

	return GeometryData();
}
