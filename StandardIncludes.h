#pragma once
#ifndef STANDARD_INCLUDES_H
#define STANDARD_INCLUDES_H

#include <vector>
#include <string>
#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#define M_ASSERT(condition, message) \
	if (!(condition)) { OutputDebugStringA(message); std::abort(); glfwTerminate(); }
#endif // !_WIN32

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Singleton.h"

#endif // !STANDARD_INCLUDES_H