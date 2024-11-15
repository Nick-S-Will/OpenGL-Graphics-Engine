#include "GameController.h"
#include "WindowController.h"
#include "ToolWindow.h"
#include "Font.h"

void GameController::Initialize()
{
	GLFWwindow* window = WindowController::GetInstance().GetWindow();
	M_ASSERT(glewInit() == GLEW_OK, "Failed to initialize GLEW.");
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(.2f, .2f, .2f, 0.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	srand((unsigned int)time(0));

	glm::ivec2 screenSize = WindowController::GetInstance().GetScreenSize();
	camera = Camera(Resolution(screenSize.x, screenSize.y, 45.f));
	camera.LookAt({ 0.f, 0.f, 5.f }, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
}

void GameController::RunGame()
{
#pragma region Shaders
	skyboxShader = Shader();
	skyboxShader.LoadShaders("Skybox.vertexshader", "Skybox.fragmentshader");

	diffuseShader = Shader();
	diffuseShader.LoadShaders("Diffuse.vertexshader", "Diffuse.fragmentshader");

	colorShader = Shader();
	colorShader.LoadShaders("Color.vertexshader", "Color.fragmentshader");

	fontShader = Shader();
	fontShader.LoadShaders("Font.vertexshader", "Font.fragmentshader");
#pragma endregion

#pragma region Skybox
	skybox = new Skybox();
	skybox->Create(&skyboxShader, "./Assets/Models/Skybox.obj",
		{ "./Assets/Textures/Skybox/right.jpg",
		  "./Assets/Textures/Skybox/left.jpg",
		  "./Assets/Textures/Skybox/top.jpg",
		  "./Assets/Textures/Skybox/bottom.jpg",
		  "./Assets/Textures/Skybox/front.jpg",
		  "./Assets/Textures/Skybox/back.jpg" });
#pragma endregion

#pragma region Meshes
	Mesh* mesh = new Mesh();
	mesh->Create(&diffuseShader, "./Assets/Models/Monkey.obj", GL_REPEAT);
	mesh->position = { 0.f, 0.f, 0.f };
	meshes.push_back(mesh);

	std::vector<glm::vec3> colors = { {1.f, 1.f, 1.f} };
	std::vector<LightType> lightTypes{  LightType::Point };
	for (int i = 0; i < LIGHT_COUNT; i++)
	{
		Mesh* lightMesh = new Mesh();
		lightMesh->Create(&colorShader, "./Assets/Models/Sphere.obj", GL_REPEAT);
		lightMesh->position = glm::vec3(0.f, 5.f, 0.f);
		lightMesh->scale = glm::vec3(.1f);
		lightMesh->color = colors[i % colors.size()];
		lightMesh->lightType = lightTypes[i % lightTypes.size()];

		lightMeshes.push_back(lightMesh);
	}
#pragma endregion

	Font* arialFont = new Font();
	arialFont->Create(&fontShader, "./Assets/Fonts/arial.ttf", 100);

	GLFWwindow* window = WindowController::GetInstance().GetWindow();
	double lastTime = glfwGetTime();
	int enabledLightIndex = 0;
	do
	{
		glfwPollEvents();
		float deltaTime = (float)(glfwGetTime() - lastTime);
		lastTime += deltaTime;

		camera.RotateAround({ 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }, -45.f * deltaTime);
		meshes[0]->eulerAngles.y += glm::radians(45.f * deltaTime);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 vp = camera.GetProjection() * camera.GetView();
		skybox->Render(vp);
		for (auto lightMesh : lightMeshes) lightMesh->Render(vp, camera.GetPosition(), lightMeshes);
		for (auto mesh : meshes) mesh->Render(vp, camera.GetPosition(), lightMeshes);

		arialFont->RenderText("Nicholas", 10.f, 50.f, .5f, { 0.f, 0.f, 0.f });
		
		glfwSwapBuffers(window);

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	skyboxShader.Cleanup();
	diffuseShader.Cleanup();
	colorShader.Cleanup();
	fontShader.Cleanup();
	delete skybox;
	for (auto boxMesh : meshes) delete boxMesh;
	meshes.clear();
	for (auto lightMesh : lightMeshes) delete lightMesh;
	lightMeshes.clear();
	delete arialFont;
}