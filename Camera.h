#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "StandardIncludes.h"

class Camera
{
public:
	Camera() = default;
	Camera(Resolution resolution);
	virtual ~Camera() = default;

	glm::mat4 GetProjection() const { return projection; }
	glm::mat4 GetView() const { return view; }

private:
	glm::mat4 projection = {};
	glm::mat4 view = {};
};

#endif // !CAMERA_H