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

	for (int i = 0; i < 10; i++)
	{
		Mesh* boxMesh = new Mesh();
		boxMesh->Create(&diffuseShader, textureWrapModes[0]);
		boxMesh->position = GetRandomPosition(0.f, 1.f);
		boxMesh->scale = { 0.3f, 0.3f, 0.3f };
		boxMeshes.push_back(boxMesh);
	}
	
	lightMesh = new Mesh();
	lightMesh->Create(&colorShader, textureWrapModes[0]);
	lightMesh->position = glm::vec3(1.f, 0.5f, 0.0f);
	lightMesh->scale = glm::vec3(.1f);

	GLFWwindow* window = WindowController::GetInstance().GetWindow();
	bool changeCameraPressed = false, changeResolutionPressed = false;
	do
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 vp = camera.GetProjection() * camera.GetView();
		for (auto boxMesh : boxMeshes) boxMesh->Render(vp, camera.position, lightMesh->position, { 0.5f, 0.9f, 0.5f });
		lightMesh->Render(vp, camera.position, glm::vec3(0.f), glm::vec3(1.f));
		glfwSwapBuffers(window);

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	diffuseShader.Cleanup();
	colorShader.Cleanup();
	for (auto boxMesh : boxMeshes) delete boxMesh;
	boxMeshes.clear();
	delete lightMesh;
}