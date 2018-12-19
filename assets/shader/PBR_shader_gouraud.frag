#version 430 core


in vec4 colorVertex;

out vec4 color;

uniform vec3 materialColor;



void main(){
	color = colorVertex;
}
