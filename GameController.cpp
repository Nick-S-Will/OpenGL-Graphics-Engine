#include "GameController.h"
#include "WindowController.h"
#include "ToolWindow.h"

void GameController::Initialize()
{
	GLFWwindow* window = WindowController::GetInstance().GetWindow();
	M_ASSERT(glewInit() == GLEW_OK, "Failed to initialize GLEW.");
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glEnable(GL_CULL_FACE);

	glm::ivec2 screenSize = WindowController::GetInstance().GetScreenSize();
	for (int i = 0; i < CAMERA_COUNT; i++)
	{
		resolutions[i] = Resolution(screenSize.x, screenSize.y, 40.f + i * 20.f);
	}

	glm::vec3 positions[CAMERA_COUNT] = { glm::vec3(100.f, 100.f, 100.f), glm::vec3(-4.f, 3.f, 3.f), glm::vec3(4.f, 3.f, 3.f) };
	for (int i = 0; i < CAMERA_COUNT; i++)
	{
		cameras[i] = Camera(resolutions[0]);
		cameras[i].LookAt(positions[i], glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	}
}

void GameController::RunGame()
{
	shader = Shader();
	shader.LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	GLenum textWrapModes[] = { GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER };
	mesh = Mesh();
	mesh.Create(&shader, textWrapModes[0]);

	GLFWwindow* window = WindowController::GetInstance().GetWindow();
	glm::vec3 eulerAngles(0);
	double lastTime = 0.;
	bool changeCameraPressed = false, changeResolutionPressed = false;
	do
	{
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !changeCameraPressed) ChangeCamera();
		changeCameraPressed = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;
		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS && !changeResolutionPressed) ChangeResolution();
		changeResolutionPressed = glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS;
		
		double time = glfwGetTime();
		float deltaTime = (float)(time - lastTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) eulerAngles.y -= deltaTime;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) eulerAngles.y += deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) eulerAngles.x -= deltaTime;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) eulerAngles.x += deltaTime;

		glClear(GL_COLOR_BUFFER_BIT);

		float scale = PingPong((float)time / 2.f, 2.f - 0.01f) + 0.01f;
		mesh.Render(cameras[cameraIndex].GetProjection() * cameras[cameraIndex].GetView(), eulerAngles, scale);
		glfwSwapBuffers(window);

		lastTime = time;
	}
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	shader.Cleanup();
	mesh.Cleanup();
}

void GameController::ChangeCamera()
{
	cameraIndex = (cameraIndex + 1) % CAMERA_COUNT;
	cameras[cameraIndex].SetProjection(resolutions[resolutionIndex]);
}

void GameController::ChangeResolution()
{
	resolutionIndex = (resolutionIndex + 1) % CAMERA_COUNT;
	cameras[cameraIndex].SetProjection(resolutions[resolutionIndex]);
}