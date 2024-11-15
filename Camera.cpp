#include "Camera.h"

Camera::Camera(const Resolution& resolution, const float minDistance, const float maxDistance)
{
	projection = glm::perspective(glm::radians(resolution.fieldOfViewAngle), (float)resolution.width / resolution.height, minDistance, maxDistance);
	view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
}

void Camera::SetProjection(const Resolution& resolution)
{
	float aspect = (float)resolution.width / resolution.height;
	projection = glm::perspective(glm::radians(resolution.fieldOfViewAngle), aspect, .1f, 1000.f);
}

void Camera::RotateAround(const glm::vec3& point, const glm::vec3& axis, const float angle)
{
	glm::vec3 relativePosition = position - point;

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.f), glm::radians(angle), axis);
	glm::vec3 rotatedPosition = glm::vec3(rotationMatrix * glm::vec4(relativePosition, 1.f));

	position = point + rotatedPosition;
	view = glm::lookAt(position, point, axis);
}

void Camera::LookAt(const glm::vec3& position, const glm::vec3& lookAt, const glm::vec3& up)
{
	this->position = position;
	view = glm::lookAt(position, lookAt, up);
}