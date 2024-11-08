#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "StandardIncludes.h"
#include "WindowController.h"

class Camera
{
public:
	glm::vec3 position = {};

	Camera() = default;
	Camera(const Resolution& resolution, const float minDistance = 0.1f, const float maxDistance = 1000.f);
	virtual ~Camera() = default;

	glm::mat4 GetProjection() const { return projection; }
	glm::mat4 GetView() const { return view; }

	void SetProjection(const Resolution& resolution);
	void RotateAround(const glm::vec3& point, const glm::vec3& axis, const float angle);
	void LookAt(const glm::vec3& lookAt, const glm::vec3& up);

private:
	glm::mat4 projection = {};
	glm::mat4 view = {};
};

#endif // !CAMERA_H