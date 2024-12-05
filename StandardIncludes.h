#pragma once
#ifndef STANDARD_INCLUDES_H
#define STANDARD_INCLUDES_H

#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#ifdef _WIN32
#include <Windows.h>
#define M_ASSERT(condition, message) \
	if (!(condition)) { OutputDebugStringA(message); std::abort(); glfwTerminate(); }
#endif // !_WIN32

// Free type font
#include <ft2build.h>
#include FT_FREETYPE_H

// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Singleton.h"
#include "Resolution.h"

#endif // !STANDARD_INCLUDES_H