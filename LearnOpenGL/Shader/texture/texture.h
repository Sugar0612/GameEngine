#pragma once

#include "../../Engine/base.h"

#include <vector>
#include <string>
#include <iostream>

class Texture
{
public:
	Texture();
	void LoadImg(const char* imgPath, const std::string Type = "JPG");
	void GameLoop();

private:
	void CreateTexture();

private:
	std::vector<GLuint> texBuf{};
	int texCnt;
};