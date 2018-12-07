#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;
uniform vec3 materialColor;
uniform vec3 materialCoefficients;

out vec4 colorVertex;

float diffuse(){
	return vec3(0.0f,0.0f,0.0f);
}

float specular(){
	return vec3(0.0f,0.0f,0.0f);
}

void main() {
	gl_Position = viewProjectionMatrix * modelMatrix * vec4(position,1.0f);
}