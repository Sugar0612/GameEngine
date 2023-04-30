#pragma once
#include "../../../Editor/Layer/WindowLayer.h"
#include "../../../Shader/shader/Shader.h"
#include "../../../Shader/texture/texture.h"
#include "../../../Shader/camera/Camera.h"

class CollectionMgr
{
public:
	CollectionMgr();
	void Start();

private:
	WindowLayer* layer{};
	Shader* cubeShader{};
	Shader* lightShader{};
	Texture* texture{};
	Camera* camera{};

private:
	void proessInput(GLFWwindow* window);
	float deltaTime = 0.f; // 与上一帧的时间差.
	float lastFrame = 0.f; // 上一帧的时间.

	void _mouse_callback();
	void _scroll_callback();
};

