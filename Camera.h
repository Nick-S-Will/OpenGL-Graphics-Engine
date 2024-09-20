#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "StandardIncludes.h"
#include "WindowController.h"

class Camera
{
public:
	Camera() = default;
	Camera(glm::mat4 transform, Resolution resolution);
	virtual ~Camera() = default;

	glm::mat4 GetTransform() const { return transform; }
	void SetTransform(glm::mat4 transform) { this->transform = transform; }
	glm::mat4 GetProjection() const { return projection; }
	void SetProjection(Resolution resolution);

private:
	glm::mat4 transform = {};
	glm::mat4 projection = {};
};

#endif // !CAMERA_H