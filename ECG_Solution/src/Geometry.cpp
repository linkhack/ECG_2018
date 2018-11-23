#include "Geometry.h"



Geometry::Geometry(glm::mat4 modelMatrix, GeometryData& geometryData, std::shared_ptr<Shader> shader):modelMatrix(modelMatrix), shader(shader)
{
	//Create Vertex Array Object
	glGenBuffers(1, &vao);
	glBindVertexArray(vao);

	//create vertex position array
	glGenBuffers(1, &vboPositions);
	glBindBuffer(GL_ARRAY_BUFFER, vboPositions);
	glBufferData(GL_ARRAY_BUFFER, nrOfVertices * sizeof(glm::vec4), geometryData.positions.data(), GL_STATIC_DRAW);

	//Bind vertex positions to location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//create Index Array
	glGenBuffers(1, &vboIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nrOfVertices * sizeof(unsigned int), geometryData.indices.data(), GL_STATIC_DRAW);

	//Reset all bindings to 0
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//set color
	color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}


Geometry::~Geometry()
{
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
		glm::vec3(-width / 2.0f, height / 2.0f, -depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f, -depth / 2.0f),

		//right
		glm::vec3(-width / 2.0f, height / 2.0f, -depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f, -depth / 2.0f),
		glm::vec3(-width / 2.0f, height / 2.0f, -depth / 2.0f),
		glm::vec3(-width / 2.0f, -height / 2.0f, -depth / 2.0f),
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
