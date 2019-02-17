#version 430 core

#define _DIRECTIONAL_LIGHTS_COUNT 64
#define _POINT_LIGHTS_COUNT 64
#define _SPOT_LIGHT_COUNT 64

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
	vec3 attenuation;
};


struct Material {
	float ambient;
	float diffuse;
	float specular;
	float specularCoefficient;
	sampler2D diffuseTexture;
};

in struct VertexData {
	vec3 worldPosition;
	vec3 normal;
	vec2 uvs;
} vert;

//Camera stuff
uniform vec3 cameraPosition;

//Material
uniform Material materialCoefficients;

//Lights
uniform PointLight pointLights[_POINT_LIGHTS_COUNT];
uniform DirectionalLight directionalLights[_DIRECTIONAL_LIGHTS_COUNT];
uniform SpotLight spotLights[_SPOT_LIGHT_COUNT];
uniform int nrPointLight;
uniform int nrDirLight;
uniform int nrSpotLight;

out vec4 color;

float diffuse(vec3 normal, vec3 lightDir){
	return max(0,dot(normal,lightDir));
}

float specular(vec3 normal, vec3 lightDir, vec3 viewingDir){
	vec3 r = reflect(-lightDir, normal);
	
	return max(0,pow(dot(r,viewingDir),materialCoefficients.specularCoefficient));
}

vec3 addPointLight(vec3 normal, vec3 viewingDir, vec3 lightDir,  PointLight light, vec3 diffuseColor){
	vec3 color = vec3(0.0f);
	color += materialCoefficients.diffuse * diffuseColor * light.color * diffuse(normal,lightDir);
	color += materialCoefficients.specular * light.color *specular(normal,lightDir,viewingDir);
	
	return color;
}

vec3 addDirectionalLight(vec3 normal, vec3 viewingDir,  DirectionalLight light, vec3 diffuseColor){
	vec3 color = vec3(0.0f);
	color += materialCoefficients.diffuse * diffuseColor * light.color * diffuse(normal,-light.direction);
	color += materialCoefficients.specular * light.color * specular(normal,-light.direction,viewingDir);
	
	return color;
}

vec3 addSpotLight(vec3 normal, vec3 viewingDir, vec3 lightDir, SpotLight light, vec3 diffuseColor){
	vec3 color = vec3(0.0f);
	float outerCos = cos(light.outerOpeningAngle);
	float innerCos = cos(light.innerOpeningAngle);
	float directionCos = dot(-lightDir,light.direction);
	float interpolationT = 1.0f-smoothstep(innerCos,outerCos,directionCos);
	color += interpolationT * materialCoefficients.diffuse * diffuseColor * light.color * diffuse(normal,lightDir);
	color += interpolationT * materialCoefficients.specular * light.color * specular(normal,lightDir,viewingDir);

	return color;
}

void main() {
	
	vec3 worldPosition = vert.worldPosition;
	
	vec3 v = normalize(cameraPosition - worldPosition);
	
	vec3 normalWorld = normalize(vert.normal);
	
	vec3 diffuseColor = texture(materialCoefficients.diffuseTexture,vert.uvs).rgb;
	color = vec4(materialCoefficients.ambient*diffuseColor,1);
	
	for(int i = 0; i<nrPointLight; ++i){
		PointLight light = pointLights[i];
		vec3 l = light.position - worldPosition.xyz;
		float d = length(l);
		l = normalize(l);
		float attenuation = 1/(light.attenuation.x*d*d+light.attenuation.y*d+light.attenuation.z);
		color+=vec4(addPointLight(normalWorld, v, l, light, diffuseColor)*attenuation,0.0f);
	}
	
	for(int i = 0; i< nrDirLight; ++i){
		DirectionalLight light = directionalLights[i];
		color += vec4(addDirectionalLight(normalWorld,v,light, diffuseColor),0.0f);
	}
	
	for(int i = 0; i<nrSpotLight; ++i){
		SpotLight light = spotLights[i];
		vec3 l = light.position - worldPosition.xyz;
		float d = length(l);
		l = normalize(l);
		float attenuation = 1/(light.attenuation.x*d*d+light.attenuation.y*d+light.attenuation.z);
		color+=vec4(addSpotLight(normalWorld, v, l, light, diffuseColor)*attenuation,0.0f);
	}
}