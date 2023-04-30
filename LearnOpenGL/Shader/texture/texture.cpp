#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image.h"

Texture::Texture()
{
	texCnt = 0;
	stbi_set_flip_vertically_on_load(true);
}

void Texture::LoadImg(const char* imgPath, std::string Type)
{
	CreateTexture();

	int width, height, nrChannels;
	unsigned char* data = stbi_load(imgPath, &width, &height, &nrChannels, 0);
	if (!data) {
		std::cout << "Failed to load texture=>[" << imgPath << "]." << std::endl;
		return;
	}

	if (Type == "JPG") {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else if (Type == "PNG") {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

void Texture::GameLoop()
{
	for (int i = 0; i < texCnt; ++i) {
		if (!texBuf[i]) break;
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texBuf[i]);
	}
}

void Texture::CreateTexture() {
	GLuint texture = -1;
	glActiveTexture(GL_TEXTURE0 + (texCnt++));
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texBuf.emplace_back(texture);
}
