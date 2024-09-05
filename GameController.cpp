#include "GameController.h"
#include "WindowController.h"

GameController::GameController()
{
	mesh = {};
}

void GameController::Initialize()
{
	GLFWwindow* window = WindowController::GetInstance().GetWindow();
	M_ASSERT(glewInit() == GLEW_OK, "Failed to initialize GLEW.");
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.f, 0.f, 0.4f, 0.f);
}

void GameController::RunGame()
{
	mesh = Mesh();
	mesh.Create();

	GLFWwindow* window = WindowController::GetInstance().GetWindow();
	do
	{
		glClear(GL_COLOR_BUFFER_BIT);
		mesh.Render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	mesh.Cleanup();
}