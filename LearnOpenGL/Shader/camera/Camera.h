#pragma once

#include "../../Engine/base.h"

class Camera
{
public:
	Camera();

	void SetCameraView(glm::vec3 posOffset, glm::vec3 targetOffset = glm::vec3(0.f, 0.f, 0.f), glm::vec3 upOffset = glm::vec3(0.f, 0.f, 0.f));
	void SetCameraFront(float xOffset, float yOffset);
	void SetFov(float yoffset);

	glm::mat4 GetCameraView() { return CameraView; }
	glm::vec3 GetCameraFront() { return CameraFront; }
	glm::vec3 GetCameraUp() { return CameraUp; }
	glm::vec3 GetCameraPos() { return CameraPos; }
	float GetFov() { return fov; }

public:
	float Speed = 0.f;
private:
	glm::vec3 CameraPos{};
	glm::vec3 CameraFront{};
	glm::vec3 CameraUp{};

	glm::mat4 CameraView{};
	
	float yaw = 0.f;
	float pitch = 0.f;
	float fov = 45.f; // 相机的fov角度 (视距范围.
};