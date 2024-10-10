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
	OpenGL::ToolWindow^ toolWindow = gcnew OpenGL::ToolWindow();
	toolWindow->Show();

	diffuseShader = Shader();
	diffuseShader.LoadShaders("Diffuse.vertexshader", "Diffuse.fragmentshader");
	
	colorShader = Shader();
	colorShader.LoadShaders("Color.vertexshader", "Color.fragmentshader");

	GLenum textureWrapModes[] = { GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER };
	boxMesh = Mesh();
	boxMesh.Create(&diffuseShader, textureWrapModes[0]);

	lightMesh = Mesh();
	lightMesh.Create(&colorShader, textureWrapModes[0]);
	lightMesh.position = glm::vec3(1.f, 0.5f, 0.5f);
	lightMesh.scale = glm::vec3(.1f);

	GLFWwindow* window = WindowController::GetInstance().GetWindow();
	double lastTime = 0.;
	bool changeCameraPressed = false, changeResolutionPressed = false;
	do
	{
		System::Windows::Forms::Application::DoEvents();

		glfwPollEvents();

		double time = glfwGetTime();
		float deltaTime = (float)(time - lastTime);
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) boxMesh.position.y -= deltaTime;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) boxMesh.position.y += deltaTime;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) boxMesh.position.x -= deltaTime;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) boxMesh.position.x += deltaTime;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) boxMesh.eulerAngles.y -= deltaTime;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) boxMesh.eulerAngles.y += deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) boxMesh.eulerAngles.x -= deltaTime;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) boxMesh.eulerAngles.x += deltaTime;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 vp = camera.GetProjection() * camera.GetView();
		boxMesh.Render(vp, lightMesh.position, { 0.5f, 0.9f, 0.5f }, toolWindow->specularStrength, camera.position);
		lightMesh.Render(vp, glm::vec3(0.f), glm::vec3(1.f), 10.f, camera.position);
		glfwSwapBuffers(window);

		lastTime = time;
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	diffuseShader.Cleanup();
	colorShader.Cleanup();
	boxMesh.Cleanup();
	lightMesh.Cleanup();
}