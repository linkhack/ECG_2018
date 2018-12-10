#include "Geometry.h"



Geometry::Geometry(glm::mat4 modelMatrix, GeometryData& geometryData, std::shared_ptr<Material> material):modelMatrix(modelMatrix), material(material), nrOfVertices(geometryData.indices.size())
{
	//Create Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//create vertex position array
	glGenBuffers(1, &vboPositions);
	glBindBuffer(GL_ARRAY_BUFFER, vboPositions);
	glBufferData(GL_ARRAY_BUFFER, geometryData.positions.size() * sizeof(glm::vec3), geometryData.positions.data(), GL_STATIC_DRAW);
	//Bind vertex positions to location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	   
	//create normals array buffer
	glGenBuffers(1, &vboNormals);
	glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
	glBufferData(GL_ARRAY_BUFFER, geometryData.normals.size() * sizeof(glm::vec3), geometryData.normals.data(), GL_STATIC_DRAW);
	//Bind vertex normals to location 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);


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

Geometry::Geometry(glm::mat4 modelMatrix, GeometryData& geometryData, std::shared_ptr<Shader> shader) : Geometry(modelMatrix, geometryData, std::make_shared<Material>(shader))
{}

Geometry::~Geometry()
{
	if (!isEmpty)
	{
		glDeleteBuffers(1, &vboIndices);
		glDeleteBuffers(1, &vboPositions);
		glDeleteBuffers(1, &vboNormals);
		glDeleteVertexArrays(1, &vao);
		std::cout << "Buffers deleted" << std::endl;
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
	std::shared_ptr<Shader> shader = material->getShader();
	//set Model Uniforms
	material->setUniforms();
	shader->use();
	shader->setUniform("modelMatrix", totalMatrix);
	shader->setUniform("normalMatrix", glm::mat3(glm::inverse(glm::transpose(totalMatrix))));
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
		glm::vec3(-width / 2.0f, height / 2.0f, depth / 2.0f), //0
		glm::vec3(-width / 2.0f, -height / 2.0f, depth / 2.0f),
		glm::vec3(width / 2.0f, -height / 2.0f, depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f, depth / 2.0f),

		//back
		glm::vec3(width / 2.0f, height / 2.0f, -depth / 2.0f), //4
		glm::vec3(width / 2.0f, -height / 2.0f, -depth / 2.0f),
		glm::vec3(-width / 2.0f, -height / 2.0f, -depth / 2.0f),
		glm::vec3(-width / 2.0f, height / 2.0f, -depth / 2.0f),

		//top
		glm::vec3(-width / 2.0f, height / 2.0f, depth / 2.0f), //8
		glm::vec3(width / 2.0f, height / 2.0f, depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f, -depth / 2.0f),
		glm::vec3(-width / 2.0f, height / 2.0f, -depth / 2.0f),

		//bottom
		glm::vec3(width / 2.0f, -height / 2.0f, -depth / 2.0f), //12
		glm::vec3(-width / 2.0f, -height / 2.0f, -depth / 2.0f),
		glm::vec3(-width / 2.0f, -height / 2.0f, depth / 2.0f),
		glm::vec3(width / 2.0f, -height / 2.0f, depth / 2.0f),

		//left
		glm::vec3(-width / 2.0f, height / 2.0f, -depth / 2.0f), //16
		glm::vec3(-width / 2.0f, -height / 2.0f, -depth / 2.0f),
		glm::vec3(-width / 2.0f, -height / 2.0f, depth / 2.0f),
		glm::vec3(-width / 2.0f, height / 2.0f, depth / 2.0f),

		//right
		glm::vec3(width / 2.0f, -height / 2.0f, depth / 2.0f), //20
		glm::vec3(width / 2.0f, height / 2.0f, depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f, -depth / 2.0f),
		glm::vec3(width / 2.0f, -height / 2.0f, -depth / 2.0f)
	};

	data.normals = {
		//front
		glm::vec3(0.0f,0.0f,1.0f),
		glm::vec3(0.0f,0.0f,1.0f),
		glm::vec3(0.0f,0.0f,1.0f),
		glm::vec3(0.0f,0.0f,1.0f),

		//back
		glm::vec3(0.0f,0.0f,-1.0f),
		glm::vec3(0.0f,0.0f,-1.0f),
		glm::vec3(0.0f,0.0f,-1.0f),
		glm::vec3(0.0f,0.0f,-1.0f),

		//top
		glm::vec3(0.0f,1.0f,0.0f),
		glm::vec3(0.0f,1.0f,0.0f),
		glm::vec3(0.0f,1.0f,0.0f),
		glm::vec3(0.0f,1.0f,0.0f),

		//bottom
		glm::vec3(0.0f,-1.0f,0.0f),
		glm::vec3(0.0f,-1.0f,0.0f),
		glm::vec3(0.0f,-1.0f,0.0f),
		glm::vec3(0.0f,-1.0f,0.0f),

		//left
		glm::vec3(-1.0f,0.0f,0.0f),
		glm::vec3(-1.0f,0.0f,0.0f),
		glm::vec3(-1.0f,0.0f,0.0f),
		glm::vec3(-1.0f,0.0f,0.0f),

		//right
		glm::vec3(1.0f,0.0f,0.0f),
		glm::vec3(1.0f,0.0f,0.0f),
		glm::vec3(1.0f,0.0f,0.0f),
		glm::vec3(1.0f,0.0f,0.0f)
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

		//bottom
		12,14,13,
		14,12,15,

		//left
		16,17,18,
		18,19,16,

		//right
		20,22,21,
		22,20,23
	};

	return data;
}

GeometryData Geometry::createSphereGeometry(float radius, unsigned int longitudeSegments, unsigned int latitudeSegments)
{
	GeometryData data;
	
	data.positions.push_back(glm::vec3(0, +radius, 0));
	data.normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	data.positions.push_back(glm::vec3(0.0f, -radius, 0.0f));
	data.normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
	//first ring indices
	for (unsigned int i = 0; i < longitudeSegments; ++i)
	{
		data.indices.push_back(0);
		data.indices.push_back(i == longitudeSegments-1? 2 : i+3);
		data.indices.push_back(i + 2);

		data.indices.push_back(1);
		data.indices.push_back((latitudeSegments - 2)*longitudeSegments + 2 + i);
		data.indices.push_back(i == longitudeSegments - 1 ? (latitudeSegments - 2)*longitudeSegments + 2 : (latitudeSegments - 2)*longitudeSegments + i + 3);
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
			//normal is in direcction of vertex but with unit length
			data.normals.push_back(glm::vec3(
				glm::sin(verticalAngle) * glm::cos(horizontalAngle),
				glm::cos(verticalAngle),
				glm::sin(verticalAngle) * glm::sin(horizontalAngle)
			));
			if (latIndex == 1) continue;
			data.indices.push_back((latIndex - 1)*longitudeSegments + longIndex + 2);
			data.indices.push_back((latIndex - 2)*longitudeSegments + longIndex + 2);
			data.indices.push_back(longIndex==longitudeSegments-1? (latIndex - 2)*longitudeSegments + 2 :(latIndex - 2)*longitudeSegments + longIndex + 3);


			data.indices.push_back((latIndex - 1)*longitudeSegments + longIndex + 2);
			data.indices.push_back(longIndex == longitudeSegments - 1 ? (latIndex - 2)*longitudeSegments + 2 : (latIndex - 2)*longitudeSegments + longIndex + 3);
			data.indices.push_back(longIndex == longitudeSegments - 1 ? (latIndex - 1)*longitudeSegments + 2 : (latIndex - 1)*longitudeSegments + longIndex + 3);
		}
	}

	return data;
}

GeometryData Geometry::createCylinderGeometry(float radius, float height, unsigned int segments)
{
	GeometryData data;

	//bottome
	data.positions.push_back(glm::vec3(0, -height / 2.0f, 0.0f));
	data.normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
	//top
	data.positions.push_back(glm::vec3(0, height / 2.0f, 0.0f));
	data.normals.push_back(glm::vec3(0, 1.0f, 0.0f));


	for (unsigned int i = 0; i < segments; i++)
	{
		float angle = 2 * i * glm::pi<float>() / float(segments);
		//two vertices because of normals (one one normal for bottom/top, and one normal to side face
		//Vertices are ordered counterclockwise!!
		data.positions.push_back(glm::vec3(radius*glm::cos(angle), -height / 2.0f, radius*glm::sin(angle))); // 2 + 4i //bottom face
		data.normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
		data.positions.push_back(glm::vec3(radius*glm::cos(angle), -height / 2.0f, radius*glm::sin(angle))); // 3 + 4i //side face bottom
		data.normals.push_back(glm::vec3(glm::cos(angle), 0.0f, glm::sin(angle)));
		data.positions.push_back(glm::vec3(radius*glm::cos(angle), height / 2.0f, radius*glm::sin(angle)));  // 4 + 4i //side face top
		data.normals.push_back(glm::vec3(glm::cos(angle), 0.0f, glm::sin(angle)));
		data.positions.push_back(glm::vec3(radius*glm::cos(angle), height / 2.0f, radius* glm::sin(angle)));  // 5 + 4i //top face
		data.normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		//bottom faces
		data.indices.push_back(0);
		data.indices.push_back(2 + 4 * i);
		data.indices.push_back(i == segments - 1 ? 2 : 2 + 4 * (i + 1));
		
		//side faces
		data.indices.push_back(3 + 4 * i);
		data.indices.push_back(i == segments - 1 ? 4 : 4 + 4 * (i + 1));
		data.indices.push_back(i == segments - 1 ? 3 : 3 + 4 * (i + 1));


		data.indices.push_back(3 + 4 * i);
		data.indices.push_back(4 + 4 * i);
		data.indices.push_back(i == segments - 1 ? 4 : 4 + 4 * (i + 1));

		//top face
		data.indices.push_back(1);
		data.indices.push_back(i == segments - 1 ? 5 : 5 + 4 * (i + 1));
		data.indices.push_back(5 + 4 * i);
	}
	return data;
}

GeometryData Geometry::createTorusGeometry(float bigRadius, float smallRadius, unsigned int tubeSections, unsigned int circleSections)
{
	GeometryData data;
	for (unsigned int tubeIndex = 0; tubeIndex < tubeSections; ++tubeIndex)
	{
		float tubeAngle = 2 * tubeIndex*glm::pi<float>() / float(tubeSections);
		for (unsigned int circleIndex = 0; circleIndex < circleSections; ++circleIndex)
		{
			float circleAngle = 2 * circleIndex * glm::pi<float>() / float(circleSections);
			data.positions.push_back(glm::vec3( (bigRadius + smallRadius * glm::cos(circleAngle))*glm::cos(tubeAngle),
												(bigRadius + smallRadius * glm::cos(circleAngle))*glm::sin(tubeAngle),
												 smallRadius * glm::sin(circleAngle)));
			glm::vec3 tangentTubeAngle = glm::vec3(-glm::sin(tubeAngle), glm::cos(tubeAngle), 0.0f);
			glm::vec3 tangentCircleAngle = glm::vec3(-smallRadius * glm::cos(tubeAngle)*glm::sin(circleAngle), -smallRadius * glm::sin(tubeAngle)*glm::sin(circleAngle), smallRadius * glm::cos(circleAngle));
			data.normals.push_back(glm::normalize(glm::cross(tangentTubeAngle, tangentCircleAngle)));
			data.indices.push_back(circleIndex == circleSections - 1 ? tubeIndex * circleSections : circleIndex + tubeIndex * circleSections + 1);
			data.indices.push_back(circleIndex + tubeIndex * circleSections);
			if (tubeIndex == tubeSections - 1) 
			{
				data.indices.push_back(circleIndex == circleSections - 1 ? 0 : circleIndex+1);
			}
			else 
			{
				data.indices.push_back(circleIndex == circleSections - 1 ? (tubeIndex+1) * circleSections : circleIndex + (tubeIndex+1) * circleSections + 1);
			}
			
			if (tubeIndex == tubeSections - 1)
			{
				data.indices.push_back(circleIndex == circleSections - 1 ? 0 : circleIndex + 1);
			}
			else
			{
				data.indices.push_back(circleIndex == circleSections - 1 ? (tubeIndex + 1) * circleSections : circleIndex + (tubeIndex + 1) * circleSections + 1);
			}
			data.indices.push_back(circleIndex + tubeIndex * circleSections);
			data.indices.push_back(tubeIndex == tubeSections-1? circleIndex :  circleIndex + (tubeIndex+1) * circleSections);
		}
	}
	return data;
}
