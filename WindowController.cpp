#include "WindowController.h"

WindowController::WindowController()
{
	window = nullptr;
}

WindowController::~WindowController()
{
	if (window == nullptr) return;

	glfwTerminate();
	window = nullptr;
}

void WindowController::Create(int width, int height)
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if (width == -1) width = mode->width;
	if (height == -1) height = mode->height;

	screenSize = glm::ivec2(width, height);
}

GLFWwindow* WindowController::NewWindow(std::string name)
{
	M_ASSERT(glfwInit(), "Failed to initialize GLFW");

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	window = glfwCreateWindow(screenSize.x, screenSize.y, name.c_str(), NULL, NULL);
	M_ASSERT(window != nullptr, "Failed to open GLFW window.");

	glfwMakeContextCurrent(window);

	return window;
}