#include "Camera.h"

Camera::Camera(Resolution resolution)
{
	float aspect = (float)resolution.width / resolution.height;
	projection = glm::perspective(glm::radians(45.f), aspect, .1f, 1000.f);

	glm::vec3 cameraPosition(4.f, 3.f, 3.f);
	glm::vec3 targetPosition(0.f, 0.f, 0.f);
	glm::vec3 upDirection(0.f, 1.f, 0.f);
	view = glm::lookAt(cameraPosition, targetPosition, upDirection);
}