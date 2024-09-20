#pragma once
#ifndef WINDOW_CONTROLLER_H
#define WINDOW_CONTROLLER_H

#include "StandardIncludes.h"

class WindowController : public Singleton<WindowController>
{
public:
	WindowController();
	virtual ~WindowController();

	GLFWwindow* GetWindow()
	{
		if (window == nullptr) NewWindow();
		return window;
	}

	void NewWindow();
	glm::ivec2 GetScreenSize();

private:
	GLFWwindow* window;
};

#endif // !WINDOW_CONTROLLER_H