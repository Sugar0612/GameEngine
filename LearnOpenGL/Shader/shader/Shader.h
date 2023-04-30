#pragma once

#include "../../Engine/base.h"

#include "data.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class Shader
{
public:
	Shader(const char* vPath, const char* fPath);
	void UseProgram();
	void GameLoop();
	void DeleteShader();
	void DrawArrays(int cnt);
	GLuint GetVAO() { return VAO; }
	void BindVAO(GLuint idx);
	void BindVBO(GLint idx);

	void SetUniform1f(const GLchar* name, float f);
	void SetUniform4f(const GLchar* name, glm::vec4 v4);
	void SetUniform3f(const GLchar* name, glm::vec3 v3);
	void SetUniform1i(const GLchar* name, GLuint x);
	void SetUniformMatx4(const GLchar* name, glm::mat4 trans);
private:
	void ReadShaderSource(const char* path, std::string& SourceStr);

	void CreateVAO();
	void CreateVBO();
	void CreateEBO();

	void CreateVertShader();
	void CreateFragShader();
	void CreateShaderPrg();

	void DeleteShader(GLuint shader);
	void check(GLuint Target, std::string Type);
private:
	GLuint VAO = -1;
	GLuint VBO = -1;
	GLuint EBO = -1;

	GLuint vertexShader = -1;
	GLuint fragShader = -1;
	GLuint ShaderProgram = -1;
private:
	std::string vertexStr{};
	std::string fragStr{};

	const GLchar* vertexSource = "";
	const GLchar* fragSource = "";
};

