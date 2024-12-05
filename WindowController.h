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
		return window != nullptr ? window : NewWindow();
	}
	glm::ivec2 GetScreenSize() { return screenSize; }

	void Create(int width = -1, int height = -1);
	GLFWwindow* NewWindow(std::string name = "Sample Scene");

private:
	GLFWwindow* window;
	glm::ivec2 screenSize = glm::ivec2(1280, 720);
};

#endif // !WINDOW_CONTROLLER_H