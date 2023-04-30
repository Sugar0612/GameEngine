#include "Shader.h"

Shader::Shader(const char* vPath, const char* fPath)
{
	glEnable(GL_DEPTH_TEST);

	ReadShaderSource(vPath, vertexStr);
	ReadShaderSource(fPath, fragStr);

	vertexSource = vertexStr.c_str();
	fragSource = fragStr.c_str();

	CreateVertShader();
	CreateFragShader();
	CreateShaderPrg();

	CreateVAO();
	CreateVBO();
	CreateEBO();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Shader::UseProgram()
{
	glUseProgram(ShaderProgram);
}

void Shader::BindVAO(GLuint idx)
{
	glBindVertexArray(idx);
}

void Shader::BindVBO(GLint idx)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Shader::GameLoop()
{
	//UseProgram();

	BindVAO(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//BindVAO(0);
}

void Shader::DeleteShader()
{
	DeleteShader(VAO);
	DeleteShader(VBO);
	DeleteShader(EBO);
}

void Shader::DrawArrays(int cnt)
{
	glDrawArrays(GL_TRIANGLES, 0, cnt);
}

void Shader::SetUniform1f(const GLchar* name, float f)
{
	GLint UniformLocation = glGetUniformLocation(ShaderProgram, name);
	glUniform1f(UniformLocation, f);
}

void Shader::SetUniform4f(const GLchar* name, glm::vec4 v4)
{
	GLint UniformLocation = glGetUniformLocation(ShaderProgram, name);
	glUniform4f(UniformLocation, v4.x, v4.y, v4.z, v4.w);
}

void Shader::SetUniform3f(const GLchar* name, glm::vec3 v3)
{
	GLint UniformLocation = glGetUniformLocation(ShaderProgram, name);
	glUniform3f(UniformLocation, v3.x, v3.y, v3.z);
}

void Shader::SetUniform1i(const GLchar* name, GLuint x)
{
	GLint UniformLocation = glGetUniformLocation(ShaderProgram, name);
	glUniform1i(UniformLocation, x);
}

void Shader::SetUniformMatx4(const GLchar* name, glm::mat4 trans)
{
	GLint UniformLocation = glGetUniformLocation(ShaderProgram, name);
	glUniformMatrix4fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(trans));
}

//ifstream =>: stringstream -> string -> char*;
void Shader::ReadShaderSource(const char* path, std::string& SourceStr)
{
	std::ifstream ShaderFile;
	ShaderFile.exceptions(std::ifstream::badbit);
	try
	{
		ShaderFile.open(path);
		std::stringstream ShaderStream{};
		ShaderStream << ShaderFile.rdbuf();
		ShaderFile.close();
		SourceStr = ShaderStream.str();
	}
	catch (const std::exception&)
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		return;
	}
	return;
}

void Shader::CreateVAO() {
	glGenVertexArrays(1, &VAO);
	BindVAO(VAO);
}

void Shader::CreateVBO()
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void Shader::CreateEBO()
{
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void Shader::CreateVertShader()
{
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	check(vertexShader, "VERTEX");
}

void Shader::CreateFragShader()
{
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragSource, NULL);
	glCompileShader(fragShader);
	check(fragShader, "FRAGMENT");
}

void Shader::CreateShaderPrg()
{
	ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, vertexShader);
	glAttachShader(ShaderProgram, fragShader);
	glLinkProgram(ShaderProgram);
	check(fragShader, "PROGRAM");
}

void Shader::DeleteShader(GLuint shader)
{
	glDeleteShader(shader);
}

void Shader::check(GLuint Target, std::string Type) {
	GLint success;
	GLchar infoLog[512];
	if (Type != "ShaderProgram") {
		glGetShaderiv(Target, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::"<< Type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
			return;
		}
	}
	else {
		glGetProgramiv(Target, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(Target, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << Type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
			return;
		}
	}
}
