#pragma once
#include <iostream>

#include "../../Engine/Manager/GLEW/GLPtrMgr.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class WindowLayer
{
public:
	WindowLayer(const GLuint width, const GLuint height);

	GLFWwindow* GetWindowobj();
	void GameLoop();
	void SwapBuffers();
	void Terminate();

	const GLfloat GetScreenWidth() { return ScreenWidth; }
	const GLfloat GetScreenHeight() { return ScreenHeight; }

private:
	GLfloat ScreenWidth{}, ScreenHeight{};
	GLFWwindow* window{};
	GLPtrMgr* glew{};
};

