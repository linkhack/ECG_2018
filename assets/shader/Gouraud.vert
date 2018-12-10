#version 430 core

#define _DIRECTIONAL_LIGHTS_COUNT 3
#define _POINT_LIGHTS_COUNT 3
#define _SPOT_LIGHT_COUNT 3

struct PointLight {
	vec3 color;
	vec3 position;
	vec3 attenuation;
};

struct DirectionalLight {
	vec3 color;
	vec3 direction;
};

struct SpotLight {
	vec3 color;
	vec3 position;
	vec3 direction;
	float innerOpeningAngle;
	float outerOpeningAngle;
	glm::vec3 attenuation;
}

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float specularCoefficient;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

//Transformation matrices
uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

//Camera stuff
uniform vec3 cameraPosition;

//Material
uniform vec3 materialColor;
uniform Material materialCoefficients;

//Lights
uniform PointLight pointLights[_POINT_LIGHTS_COUNT];
uniform DirectionalLight directionalLights[_DIRECTIONAL_LIGHTS_COUNT];
uniform SpotLight spotLights[_SPOT_LIGHT_COUNT]
uniform int nrPointLight;
uniform int nrDirLight;
uniform int nrSpotLight;

out vec4 colorVertex;

float diffuse(vec3 normal, vec3 lightDir){
	return max(0,dot(normal,lightDir));
}

float specular(vec3 normal, vec3 lightDir, vec3 viewingDir){
	vec3 r = reflect(-lightDir, normal);
	
	return max(0,pow(dot(r,viewingDir),materialCoefficients.specularCoefficient));
}

vec3 addPointLight(vec3 normal, vec3 viewingDir, vec3 lightDir,  PointLight light){
	vec3 color = vec3(0.0f);
	
	color += materialCoefficients.diffuse * materialColor * light.color * diffuse(normal,lightDir);
	color += materialCoefficients.specular * light.color *specular(normal,lightDir,viewingDir);
	
	return color;
}

vec3 addDirectionalLight(vec3 normal, vec3 viewingDir,  DirectionalLight light){
	vec3 color = vec3(0.0f);
	
	color += materialCoefficients.diffuse * materialColor * light.color * diffuse(normal,-light.direction);
	color += materialCoefficients.specular * light.color * specular(normal,-light.direction,viewingDir);
	
	return color;
}

void main() {
	colorVertex = vec4(materialCoefficients.ambient*materialColor,1);
	
	vec4 worldPosition = modelMatrix * vec4(position,1.0f);
	
	vec3 v = normalize(cameraPosition - worldPosition.xyz);
	
	vec3 normalWorld = normalize(normalMatrix * normal);
	
	for(int i = 0; i<nrPointLight; ++i){
		PointLight light = pointLights[i];
		vec3 l = light.position - worldPosition.xyz;
		float d = length(l);
		l = normalize(l);
		float attenuation = 1/(light.attenuation.x*d*d+light.attenuation.y*d+light.attenuation.z);
		colorVertex+=vec4(addPointLight(normalWorld, v, l, light)*attenuation,0.0f);
	}
	
	for(int i = 0; i< nrDirLight; ++i){
		DirectionalLight light = directionalLights[i];
		colorVertex += vec4(addDirectionalLight(normalWorld,v,light),0.0f);
	}
	//colorVertex = colorVertex/colorVertex.w;
	gl_Position = viewProjectionMatrix * worldPosition;
}