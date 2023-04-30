#include "Camera.h"

Camera::Camera()
{
	CameraPos = glm::vec3(0.f, 0.f, 3.f);
	CameraFront = glm::vec3(0.f, 0.f, -1.f);
	CameraUp = glm::vec3(0.f, 1.f, 0.f);
}

void Camera::SetCameraView(glm::vec3 posOffset, glm::vec3 targetOffset, glm::vec3 upOffset)
{
	CameraPos += posOffset;
	CameraFront += targetOffset;
	CameraUp += upOffset;
	CameraView = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
}

void Camera::SetCameraFront(float xOffset, float yOffset)
{
	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.f) pitch = 89.f;
	if (pitch < -89.f) pitch = -89.f;

	CameraFront.x = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
	CameraFront.y = glm::sin(glm::radians(pitch));
	CameraFront.z = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
	CameraFront = glm::normalize(CameraFront);
}

void Camera::SetFov(float yoffset) {
	if (fov >= 1.f && fov <= 60.f) fov -= yoffset;
	else if (fov < 1.f) fov = 1.f;
	else if (fov > 45.f) fov = 45.f;
}