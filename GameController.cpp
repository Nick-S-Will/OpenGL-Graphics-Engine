#include "GameController.h"
#include "WindowController.h"
#include "ToolWindow.h"

void GameController::Initialize()
{
	GLFWwindow* window = WindowController::GetInstance().GetWindow();
	M_ASSERT(glewInit() == GLEW_OK, "Failed to initialize GLEW.");
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glEnable(GL_DEPTH_TEST);

	glm::ivec2 screenSize = WindowController::GetInstance().GetScreenSize();
	resolution = Resolution(screenSize.x, screenSize.y, 45.f);

	camera = Camera(resolution);
	camera.LookAt(glm::vec3(2.f, 2.f, 2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
}

void GameController::RunGame()
{
	shader = Shader();
	shader.LoadShaders("Diffuse.vertexshader", "Diffuse.fragmentshader");

	GLenum textWrapModes[] = { GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER };
	mesh = Mesh();
	mesh.Create(&shader, textWrapModes[0]);

	GLFWwindow* window = WindowController::GetInstance().GetWindow();
	double lastTime = 0.;
	bool changeCameraPressed = false, changeResolutionPressed = false;
	do
	{
		glfwPollEvents();

		double time = glfwGetTime();
		float deltaTime = (float)(time - lastTime);
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) mesh.position.y -= deltaTime;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) mesh.position.y += deltaTime;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) mesh.position.x -= deltaTime;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) mesh.position.x += deltaTime;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) mesh.eulerAngles.y -= deltaTime;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) mesh.eulerAngles.y += deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) mesh.eulerAngles.x -= deltaTime;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) mesh.eulerAngles.x += deltaTime;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mesh.Render(camera.GetProjection() * camera.GetView());
		glfwSwapBuffers(window);

		lastTime = time;
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	shader.Cleanup();
	mesh.Cleanup();
}