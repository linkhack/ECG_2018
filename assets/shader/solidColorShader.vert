#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;

void main() {
	gl_Position = viewProjectionMatrix * modelMatrix * vec4(position,1.0f);
}