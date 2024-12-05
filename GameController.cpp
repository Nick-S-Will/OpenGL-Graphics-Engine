#include "GameController.h"
#include "WindowController.h"
#include "ToolWindow.h"
#include "Font.h"
#include "GameTime.h"

void GameController::Initialize()
{
	GameTime::GetInstance().Initialize();
	GLFWwindow* window = WindowController::GetInstance().GetWindow();
	M_ASSERT(glewInit() == GLEW_OK, "Failed to initialize GLEW.");
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(.0f, .0f, .0f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	srand((unsigned int)time(0));

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glm::ivec2 screenSize = WindowController::GetInstance().GetScreenSize();
	camera = Camera(Resolution(screenSize.x, screenSize.y, 45.f));
	camera.LookAt({ 1.5f, 1.5f, 1.5f }, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
}

void GameController::RunGame()
{
	std::string modelDirectory = "./Assets/Models/";
	std::string textureDirectory = "./Assets/Textures/";
	std::string skyboxDirectory = textureDirectory + "Ocean Skybox/";
	std::string fontDirectory = "./Assets/Fonts/";

#pragma region Skybox
	skyboxShader = Shader();
	skyboxShader.LoadShaders("Skybox.vertexshader", "Skybox.fragmentshader");

	skybox = new Skybox();
	skybox->Create(&skyboxShader, modelDirectory + "Skybox.obj",
		{ skyboxDirectory + "right.jpg",
		  skyboxDirectory + "left.jpg",
		  skyboxDirectory + "top.jpg",
		  skyboxDirectory + "bottom.jpg",
		  skyboxDirectory + "front.jpg",
		  skyboxDirectory + "back.jpg" });
#pragma endregion

#pragma region Meshes
	diffuseShader = Shader();
	diffuseShader.LoadShaders("Diffuse.vertexshader", "Diffuse.fragmentshader");

	Mesh* mesh = new Mesh();
	mesh->Create(&diffuseShader, modelDirectory + "Fighter.ase");
	mesh->eulerAngles.x = glm::radians(-90.f);
	mesh->scale = glm::vec3(1e-3f);
	meshes.push_back(mesh);

	colorShader = Shader();
	colorShader.LoadShaders("Color.vertexshader", "Color.fragmentshader");

	std::vector<LightType> lightTypes{ LightType::Spot };
	for (int i = 0; i < LIGHT_COUNT; i++)
	{
		Mesh* lightMesh = new Mesh();
		lightMesh->Create(&colorShader, modelDirectory + "Sphere.obj");
		lightMesh->position = glm::vec3(1.f, 1.f, 0.f);
		lightMesh->eulerAngles.x = glm::radians(-45.f);
		lightMesh->eulerAngles.y = glm::radians(90.f);
		lightMesh->scale = glm::vec3(1e-2f);
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

#pragma region Post Processor
	postProcessorShader = Shader();
	postProcessorShader.LoadShaders("PostProcessor.vertexshader", "PostProcessor.fragmentshader");

	postProcessor = PostProcessor();
	postProcessor.Create(&postProcessorShader);
#pragma endregion

	GLFWwindow* window = WindowController::GetInstance().GetWindow();
	do
	{
		glfwPollEvents();
		GameTime::GetInstance().Update();

#pragma region Rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 vp = camera.GetProjection() * camera.GetView();
		for (auto lightMesh : lightMeshes) lightMesh->Render(vp, camera.GetPosition(), lightMeshes);
		for (auto mesh : meshes) mesh->Render(vp, camera.GetPosition(), lightMeshes);

		arialFont->RenderText("FPS: " + std::to_string(GameTime::GetInstance().GetFramesPerSecond()), 10.f, 50.f, .5f, { 1.f, 1.f, 0.f });

		glfwSwapBuffers(window);
#pragma endregion
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

	postProcessorShader.Cleanup();
	postProcessor.Cleanup();
#pragma endregion
}