#include "Camera.h"

Camera::Camera(glm::mat4 transform, Resolution resolution)
{
	this->transform = transform;

	SetProjection(resolution);
}

void Camera::SetProjection(Resolution resolution)
{
	float aspect = (float)resolution.width / resolution.height;
	projection = glm::perspective(glm::radians(resolution.fieldOfViewAngle), aspect, .1f, 1000.f);
}