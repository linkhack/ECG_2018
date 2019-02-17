#version 430 core

#define _DIRECTIONAL_LIGHTS_COUNT 64
#define _POINT_LIGHTS_COUNT 64
#define _SPOT_LIGHT_COUNT 64

const float PI = 3.1415926535;

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
	vec3 baseColor;
	float ambient;
	float metallic;
	float specular;
	float specularTint;
	float roughness;
	float sheen;
	float sheenTint;
	float clearcoat;
	float clearcoatGloss;
};

in struct VertexData {
	vec3 worldPosition;
	vec3 normal;
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

out vec4 fragmentColor;

float SchlickFresnel(float u){
	float r = clamp(1.0f-u,0.0f,1.0f);
	float r2 = r * r;
	return r2*r2*r; //(1-u)^5
	
}

//for clearcoat
float GTR1(float NdotH, float alpha){
	float alpha2 = alpha * alpha;
	float denumUnnorm = 1 + (alpha2 - 1) * NdotH * NdotH;
	return (alpha2-1)/(PI*log(alpha2)*denumUnnorm);

}

//for normal
float GTR2(float NdotH, float alpha){
	float alpha2 = alpha * alpha;
	float denum = 1+(alpha2-1)*NdotH*NdotH;
	return alpha2/(PI * denum * denum);
}

float smithG(float NdotV, float alpha){
	float a = alpha*alpha;
	float b = NdotV*NdotV;
	return 1.0f/(NdotV+sqrt(a+b-a*b));
}

vec3 diffuse(float LdotH,float LdotN, float VdotN, vec3 linearBaseColor){
	float FD90 = 0.5f + 2.0f * LdotH*LdotH*materialCoefficients.roughness;   
	float Fl = SchlickFresnel(LdotN);
	float Fv = SchlickFresnel(VdotN);
	return linearBaseColor/PI * mix(1.0f,FD90,Fl)*mix(1.0f,FD90,Fv)*LdotN;
}

vec3 specular(float NdotH, float LdotH, float NdotV, float NdotL, vec3 specColor, vec3 cSheen){
	//Normal
	float alpha = max(0.001,materialCoefficients.roughness * materialCoefficients.roughness);
	//Specular D from microfacet distribution
	float DH = GTR2(NdotH,alpha);
	//Specular Fresnel Term
	float FH = SchlickFresnel(LdotH); //(1-cos(Omega_d))^5
	vec3 Fs = mix(specColor,vec3(1.0f),FH);
	//G term: Geometric attenuation / shadowing
	alpha = (0.5 + 0,5 * materialCoefficients.roughness);
	alpha*=alpha;
	float G = smithG(NdotV,alpha);

	G *= smithG(NdotL,alpha);
	
	//Clearcoat F0 = 0.04
	float Dc = GTR1(NdotH,mix(0.1,0.001,materialCoefficients.clearcoatGloss));
	vec3 Fc = mix(vec3(0.04f),vec3(1.0f),FH);
	float Gc = smithG(NdotV,0.25f)*smithG(NdotL,0.25f);
	
	//sheen
	vec3 sheen = FH * materialCoefficients.sheen * cSheen;
	
	return G*Fs*DH + 0.25 * materialCoefficients.clearcoat*Dc*Fc*Gc + (1-materialCoefficients.metallic)*sheen;
	
}

vec3 addPointLight(vec3 normal, vec3 viewingDir, vec3 lightDir, vec3 cSheen, vec3 cLinear, vec3 specColor){
	vec3 color = vec3(0.0f);
	vec3 h = normalize(lightDir+viewingDir);
	float NdotH = dot(normal,h);
	float NdotV = dot(normal,viewingDir);
	float NdotL = dot(normal,lightDir);
	float LdotH = dot(lightDir,h);
	if (NdotL < 0 || NdotV < 0) return vec3(0.0f);
	//diffuse
	color +=(1.0f-materialCoefficients.metallic)*diffuse(LdotH,NdotL,NdotV,cLinear);
	
	color += specular(NdotH, LdotH, NdotV, NdotL, specColor, cSheen);
	
	return color;
}

vec3 addDirectionalLight(vec3 normal, vec3 viewingDir,  DirectionalLight light, vec3 cSheen, vec3 cLinear, vec3 specColor){
	vec3 color = vec3(0.0f);
	vec3 h = normalize(-light.direction+viewingDir);
	float NdotH = dot(normal,h);
	float NdotV = dot(normal,viewingDir);
	float NdotL = dot(normal,-light.direction);
	float LdotH = dot(-light.direction,h);
	if (NdotL < 0 || NdotV < 0) return vec3(0.0f);
	color+=(1.0f-materialCoefficients.metallic)*diffuse(LdotH,NdotL,NdotV,cLinear);
	
	color += specular(NdotH, LdotH, NdotV, NdotL, specColor, cSheen);
	
	return color;
}

vec3 gammaCorrection(vec3 color){
	return pow(color,vec3(1.0f/2.2f));
}

vec3 toLinearColorspace(vec3 color){
	return pow(color,vec3(2.2f));
}

vec3 addSpotLight(vec3 normal, vec3 viewingDir, vec3 lightDir, SpotLight light, vec3 cSheen, vec3 cLinear, vec3 specColor){
	vec3 color = vec3(0.0f);
	float outerCos = cos(light.outerOpeningAngle);
	float innerCos = cos(light.innerOpeningAngle);
	float directionCos = dot(lightDir,-light.direction);
	float interpolationT = 1.0f-smoothstep(innerCos,outerCos,directionCos);
	
	vec3 h = normalize(lightDir+viewingDir);
	float NdotH = dot(normal,h);
	float NdotV = dot(normal,viewingDir);
	float NdotL = dot(normal,lightDir);
	float LdotH = dot(lightDir,h);
	if (NdotL < 0 || NdotV < 0) return vec3(0);
	color += (1.0f-materialCoefficients.metallic)*diffuse(LdotH,NdotL,NdotV,cLinear);
	
	color += specular(NdotH, LdotH, NdotV, NdotL, specColor, cSheen);
	
	color *= interpolationT;

	return color;
}

void main() {
	
	vec3 worldPosition = vert.worldPosition;
	
	vec3 v = normalize(cameraPosition - worldPosition);
	
	vec3 normalWorld = normalize(vert.normal);
	
	//Interpolation of Colors
	vec3 linearBaseColor = toLinearColorspace(materialCoefficients.baseColor);
	float luminance = 0.2*linearBaseColor.r + 0.7*linearBaseColor.g + 0.1 * linearBaseColor.b;
	
	vec3 lumNormColor = luminance>0?linearBaseColor/luminance:vec3(1.0f); //only hue and sat
	vec3 specColor = mix(materialCoefficients.specular*0.08*mix(vec3(1.0f),lumNormColor,materialCoefficients.specularTint), 
							linearBaseColor,
							materialCoefficients.metallic);
	vec3 sheenColor = mix(vec3(1.0f),lumNormColor,materialCoefficients.sheenTint);
	
	//Ambient Lights	
	vec3 color = vec3(pow(materialCoefficients.ambient,2.2)*linearBaseColor); // to make it consistant with base models
	
	for(int i = 0; i<nrPointLight; ++i){
		PointLight light = pointLights[i];
		vec3 l = light.position - worldPosition.xyz;
		float d = length(l);
		l = normalize(l);
		float attenuation = 1/(light.attenuation.x*d*d+light.attenuation.y*d+light.attenuation.z);
		color+=addPointLight(normalWorld, v, l, sheenColor, linearBaseColor, specColor)*attenuation*light.color;
	}
	
	for(int i = 0; i< nrDirLight; ++i){
		DirectionalLight light = directionalLights[i];
		color += addDirectionalLight(normalWorld,v,light, sheenColor, linearBaseColor, specColor)*light.color;
	}
	for(int i = 0; i < nrSpotLight; ++i){
		SpotLight light = spotLights[i];
		vec3 l = light.position - worldPosition.xyz;
		float d = length(l);
		l = normalize(l);
		float attenuation = 1/(light.attenuation.x*d*d+light.attenuation.y*d+light.attenuation.z);
		color+=addSpotLight(normalWorld, v, l, light, sheenColor, linearBaseColor,specColor)*attenuation*light.color;
	}
	
	fragmentColor = vec4(gammaCorrection(color),1);
}