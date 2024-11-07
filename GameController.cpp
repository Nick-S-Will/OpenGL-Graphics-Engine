#include "GameController.h"
#include "WindowController.h"
#include "ToolWindow.h"
#include "Font.h"

void GameController::Initialize()
{
	GLFWwindow* window = WindowController::GetInstance().GetWindow();
	M_ASSERT(glewInit() == GLEW_OK, "Failed to initialize GLEW.");
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

	fontShader = Shader();
	fontShader.LoadShaders("Font.vertexshader", "Font.fragmentshader");

	GLenum textureWrapModes[] = { GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER };

	Mesh* boxMesh = new Mesh();
	boxMesh->Create(&diffuseShader, "./Assets/Models/Cube.obj", textureWrapModes[0]);
	boxMesh->position = glm::vec3(0.f, 0.f, 0.f);
	boxMesh->scale = glm::vec3(.1f);

	boxMeshes.push_back(boxMesh);

	for (int i = 0; i < LIGHT_COUNT; i++)
	{
		Mesh* lightMesh = new Mesh();
		lightMesh->Create(&colorShader, "./Assets/Models/Sphere.obj", textureWrapModes[0]);
		lightMesh->position = glm::vec3(.5f, 0.f, 0.f);
		lightMesh->eulerAngles = glm::vec3(0.f, glm::radians(90.f), 0.f);
		lightMesh->scale = glm::vec3(.1f);

		lightMeshes.push_back(lightMesh);
	}

	Font* arialFont = new Font();
	arialFont->Create(&fontShader, "./Assets/Fonts/arial.ttf", 100);

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
		for (auto boxMesh : boxMeshes) boxMesh->Render(vp, camera.position, lightMeshes, { 1.f, 1.f, 1.f });
		for (auto lightMesh : lightMeshes) lightMesh->Render(vp, camera.position, lightMeshes, glm::vec3(1.f));
		arialFont->RenderText("Hello World", 10, 500, .5f, { 1.f, 1.f, 0.f });
		glfwSwapBuffers(window);

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	diffuseShader.Cleanup();
	colorShader.Cleanup();
	fontShader.Cleanup();
	for (auto boxMesh : boxMeshes) delete boxMesh;
	boxMeshes.clear();
	for (auto lightMesh : lightMeshes) delete lightMesh;
	lightMeshes.clear();
	delete arialFont;
}