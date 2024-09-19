#pragma once
#ifndef RESOLUTION_H
#define RESOLUTION_H

struct Resolution
{
	int width = 1280;
	int height = 768;

	Resolution(int width, int height)
	{
		this->width = width;
		this->height = height;
	}
};

#endif // !RESOLUTION_H