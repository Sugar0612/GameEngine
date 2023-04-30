#version 330 core

in vec2 TexCoords;
in vec3 baseColor;
in vec3 Normal;
in vec3 worldObjPos; // FragPos(立方体顶点世界坐标.

out vec4 color;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
#define NR_POINT_LIGHT 4
uniform PointLight pointlights[NR_POINT_LIGHT];

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
uniform SpotLight spotlight;

uniform Material material;
uniform vec3 CameraPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
	//ambient
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	// diffuse
	vec3 LightDir = normalize(-light.direction);
	float diff = max(dot(LightDir, normal), 0.0);
	vec3 diffuse = diff * light.diffuse * texture(material.diffuse, TexCoords).rgb;

	// specular
	vec3 reflectDir = normalize(reflect(-LightDir, normal));
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 specular = spec * light.specular * texture(material.specular, TexCoords).rgb;

	vec3 result = ambient + diffuse + specular;
	return result;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	// 衰减
	float Distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * Distance + light.quadratic * (Distance * Distance));

	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb * attenuation;

	vec3 LightDir = normalize(light.position - fragPos);
	float diff = max(dot(LightDir, normal), 0.0);
	vec3 diffuse = diff * texture(material.diffuse, TexCoords).rgb * light.diffuse * attenuation;

	vec3 reflectDir = normalize(reflect(-LightDir, normal));
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 specular = spec * texture(material.specular, TexCoords).rgb * light.specular * attenuation;

	vec3 result = ambient + diffuse + specular;
	return result;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 LightDir = normalize(light.position - fragPos);

	float theta = dot(-LightDir, light.direction);
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.cutOff) / epsilon, 0.0, 1.0);
	
	float Distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * Distance + light.quadratic * (Distance * Distance));

	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb * attenuation;

	float diff = max(dot(LightDir, normal), 0.0);
	vec3 diffuse = diff * texture(material.diffuse, TexCoords).rgb * attenuation * intensity;

	vec3 reflectDir = normalize(reflect(-LightDir, normal));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * texture(material.specular, TexCoords).rgb * attenuation * intensity;

	vec3 result = ambient + diffuse + specular;
	return result;
}

void main() {
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(CameraPos - worldObjPos);

 	vec3 result = CalcDirLight(dirLight, norm, viewDir);

	for (int i = 0; i < NR_POINT_LIGHT; ++i) {
		result += CalcPointLight(pointlights[i], norm, worldObjPos, viewDir);
	}

	result += CalcSpotLight(spotlight, norm, worldObjPos, viewDir);

	color = vec4(result, 1.0);
}