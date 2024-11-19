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
	camera.LookAt({ 2.f, 2.f, 2.f }, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
}

void GameController::RunGame()
{
	std::string modelDirectory = "./Assets/Models/";
	std::string textureDirectory = "./Assets/Textures/";
	std::string fontDirectory = "./Assets/Fonts/";

#pragma region Skybox
	skyboxShader = Shader();
	skyboxShader.LoadShaders("Skybox.vertexshader", "Skybox.fragmentshader");

	skybox = new Skybox();
	skybox->Create(&skyboxShader, modelDirectory + "Skybox.obj",
		{ textureDirectory + "Skybox/right.jpg",
		  textureDirectory + "Skybox/left.jpg",
		  textureDirectory + "Skybox/top.jpg",
		  textureDirectory + "Skybox/bottom.jpg",
		  textureDirectory + "Skybox/front.jpg",
		  textureDirectory + "Skybox/back.jpg" });
#pragma endregion

#pragma region Meshes
	diffuseShader = Shader();
	diffuseShader.LoadShaders("Diffuse.vertexshader", "Diffuse.fragmentshader");

	Mesh* mesh = new Mesh();
	mesh->Create(&diffuseShader, modelDirectory + "Fighter.obj", true, GL_REPEAT);
	mesh->scale = glm::vec3(2e-3f);
	meshes.push_back(mesh);

	colorShader = Shader();
	colorShader.LoadShaders("Color.vertexshader", "Color.fragmentshader");

	std::vector<LightType> lightTypes{ LightType::Point };
	for (int i = 0; i < LIGHT_COUNT; i++)
	{
		Mesh* lightMesh = new Mesh();
		lightMesh->Create(&colorShader, modelDirectory + "Sphere.obj", false, GL_REPEAT);
		lightMesh->position = glm::vec3(0.f, .8f, 1.f);
		lightMesh->scale = glm::vec3(.1f);
		lightMesh->lightType = lightTypes[i % lightTypes.size()];

		lightMeshes.push_back(lightMesh);
	}
#pragma endregion

#pragma region Font
	fontShader = Shader();
	fontShader.LoadShaders("Font.vertexshader", "Font.fragmentshader");

	Font* arialFont = new Font();
	arialFont->Create(&fontShader, fontDirectory + "arial.ttf", 100);
#pragma endregion


	GLFWwindow* window = WindowController::GetInstance().GetWindow();
	double lastTime = glfwGetTime();
	int enabledLightIndex = 0;
	do
	{
		glfwPollEvents();
		float deltaTime = (float)(glfwGetTime() - lastTime);
		lastTime += deltaTime;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 vp = camera.GetProjection() * camera.GetView();
		//skybox->Render(vp);
		for (auto lightMesh : lightMeshes) lightMesh->Render(vp, camera.GetPosition(), lightMeshes);
		for (auto mesh : meshes) mesh->Render(vp, camera.GetPosition(), lightMeshes);

		arialFont->RenderText("Nicholas", 10.f, 50.f, .5f, { 0.f, 0.f, 0.f });

		glfwSwapBuffers(window);

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

#pragma region Cleanup
	skyboxShader.Cleanup();
	delete skybox;

	diffuseShader.Cleanup();
	for (auto boxMesh : meshes) delete boxMesh;
	meshes.clear();

	colorShader.Cleanup();
	for (auto lightMesh : lightMeshes) delete lightMesh;
	lightMeshes.clear();

	fontShader.Cleanup();
	delete arialFont;
#pragma endregion
}