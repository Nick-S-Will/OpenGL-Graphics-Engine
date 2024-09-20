#pragma once
#ifndef RESOLUTION_H
#define RESOLUTION_H

struct Resolution
{
	int width;
	int height;
	float fieldOfViewAngle;

	Resolution()
	{
		width = 1920;
		height = 1080;
		fieldOfViewAngle = 45.f;
	}

	Resolution(int width, int height, float fieldOfViewAngle)
	{
		this->width = width;
		this->height = height;
		this->fieldOfViewAngle = fieldOfViewAngle;
	}
};

#endif // !RESOLUTION_H