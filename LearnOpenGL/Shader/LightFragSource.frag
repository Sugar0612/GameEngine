#version 330 core

in vec3 baseColor;

out vec4 color;

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

void main() {
	color = vec4(1.0);
	//color diffusediffuse= mix(texture(boxTexture, TexCoord), texture(faceTexture, TexCoord), 0.2);
	//color specularspecular= texture(boxTexture, TexCoord) * vec4(baseColor, 1.0);
}