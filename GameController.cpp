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
	srand(time(0));

	glm::ivec2 screenSize = WindowController::GetInstance().GetScreenSize();
	camera = Camera(Resolution(screenSize.x, screenSize.y, 45.f));
	camera.LookAt(glm::vec3(2.f, 2.f, 2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
}

void GameController::RunGame()
{
	diffuseShader = Shader();
	diffuseShader.LoadShaders("Diffuse.vertexshader", "Diffuse.fragmentshader");
	
	colorShader = Shader();
	colorShader.LoadShaders("Color.vertexshader", "Color.fragmentshader");

	GLenum textureWrapModes[] = { GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER };

	for (int row = 0; row < 10; row++)
	{
		for (int column = 0; column < 10; column++)
		{
			Mesh* boxMesh = new Mesh();
			boxMesh->Create(&diffuseShader, textureWrapModes[0]);
			boxMesh->position = glm::vec3(0.f, row / 10.f, column / 10.f);
			boxMesh->scale = glm::vec3(.1f);

			boxMeshes.push_back(boxMesh);
		}
	}

	for (int i = 0; i < LIGHT_COUNT; i++)
	{
		Mesh* lightMesh = new Mesh();
		lightMesh->Create(&colorShader, textureWrapModes[0]);
		lightMesh->position = glm::vec3(.5f, .5f, .2f * i);
		lightMesh->eulerAngles = glm::vec3(0.f, glm::radians(90.f), 0.f);
		lightMesh->scale = glm::vec3(.1f);

		lightMeshes.push_back(lightMesh);
	}
	

	GLFWwindow* window = WindowController::GetInstance().GetWindow();
	double lastTime = glfwGetTime();
	bool changeCameraPressed = false, changeResolutionPressed = false;
	do
	{
		glfwPollEvents();
		float deltaTime = glfwGetTime() - lastTime;
		lastTime += deltaTime;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) lightMeshes[0]->position -= .5f * deltaTime * lightMeshes[0]->GetRight();
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) lightMeshes[0]->position += .5f * deltaTime * lightMeshes[0]->GetRight();
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) lightMeshes[0]->position -= .5f * deltaTime * lightMeshes[0]->GetForward();
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) lightMeshes[0]->position += .5f * deltaTime * lightMeshes[0]->GetForward();

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) lightMeshes[0]->eulerAngles.x -= 2.f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) lightMeshes[0]->eulerAngles.x += 2.f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) lightMeshes[0]->eulerAngles.y += 2.f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) lightMeshes[0]->eulerAngles.y -= 2.f * deltaTime;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 vp = camera.GetProjection() * camera.GetView();
		for (auto boxMesh : boxMeshes) boxMesh->Render(vp, camera.position, lightMeshes, {1.f, 1.f, 1.f});
		for (auto lightMesh : lightMeshes) lightMesh->Render(vp, camera.position, lightMeshes, glm::vec3(1.f));
		glfwSwapBuffers(window);

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	diffuseShader.Cleanup();
	colorShader.Cleanup();
	for (auto boxMesh : boxMeshes) delete boxMesh;
	boxMeshes.clear();
	for (auto lightMesh : lightMeshes) delete lightMesh;
	lightMeshes.clear();
}