#include "GameController.h"
#include "WindowController.h"
#include "ToolWindow.h"

void GameController::Initialize()
{
	GLFWwindow* window = WindowController::GetInstance().GetWindow();
	M_ASSERT(glewInit() == GLEW_OK, "Failed to initialize GLEW.");
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.f, 0.f, 0.4f, 0.f);
}

void GameController::RunGame()
{
	OpenGL::ToolWindow^ toolWindow = gcnew OpenGL::ToolWindow();
	toolWindow->Show();

	shader = Shader();
	shader.LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	mesh = Mesh();
	mesh.Create(&shader);

	GLFWwindow* window = WindowController::GetInstance().GetWindow();
	do
	{
		System::Windows::Forms::Application::DoEvents();

		GLint location = 0;
		location = glGetUniformLocation(shader.GetProgramID(), "RenderRedChannel");
		glUniform1i(location, (int)OpenGL::ToolWindow::RenderRedChannel);
		location = glGetUniformLocation(shader.GetProgramID(), "RenderGreenChannel");
		glUniform1i(location, (int)OpenGL::ToolWindow::RenderGreenChannel);
		location = glGetUniformLocation(shader.GetProgramID(), "RenderBlueChannel");
		glUniform1i(location, (int)OpenGL::ToolWindow::RenderBlueChannel);

		glClear(GL_COLOR_BUFFER_BIT);
		mesh.Render();
		glfwSwapBuffers(window);

		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	shader.Cleanup();
	mesh.Cleanup();
}