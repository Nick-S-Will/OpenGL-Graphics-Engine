#include "Camera.h"

Camera::Camera(const Resolution& resolution, const float minDistance, const float maxDistance)
{
	projection = glm::perspective(glm::radians(resolution.fieldOfViewAngle), (float)resolution.width / resolution.height, minDistance, maxDistance);
	view = glm::lookAt(glm::vec3(1, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void Camera::SetProjection(const Resolution& resolution)
{
	float aspect = (float)resolution.width / resolution.height;
	projection = glm::perspective(glm::radians(resolution.fieldOfViewAngle), aspect, .1f, 1000.f);
}

void Camera::LookAt(const glm::vec3& position, const glm::vec3& lookAt, const glm::vec3& up)
{
	this->position = position;
	view = glm::lookAt(position, lookAt, up);
}