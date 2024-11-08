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
	srand((unsigned int)time(0));

	glm::ivec2 screenSize = WindowController::GetInstance().GetScreenSize();
	camera = Camera(Resolution(screenSize.x, screenSize.y, 45.f));
	camera.position = glm::vec3(0.f, 3.f, 3.f);
}

void GameController::RunGame()
{
	diffuseShader = Shader();
	diffuseShader.LoadShaders("Diffuse.vertexshader", "Diffuse.fragmentshader");

	colorShader = Shader();
	colorShader.LoadShaders("Color.vertexshader", "Color.fragmentshader");

	fontShader = Shader();
	fontShader.LoadShaders("Font.vertexshader", "Font.fragmentshader");

	Mesh* mesh = new Mesh();
	std::string modelName = "Monkey";
	mesh->Create(&diffuseShader, "./Assets/Models/" + modelName + ".obj", GL_REPEAT);
	mesh->scale = glm::vec3(.5f);

	meshes.push_back(mesh);

	std::vector<glm::vec3> colors = { {1.f, 1.f, 1.f}, {1.f, 0.f, 0.f} , {0.f, 1.f, 0.f} , {0.f, 0.f, 1.f} };
	std::vector<LightType> lightTypes{ LightType::Directional, LightType::Point, LightType::Directional, LightType::Spot };
	for (int i = 0; i < LIGHT_COUNT; i++)
	{
		Mesh* lightMesh = new Mesh();
		lightMesh->Create(&colorShader, "./Assets/Models/Sphere.obj", GL_REPEAT);
		float xDirection = i % 2 == 0 ? 0.f : ((i + 1) % 4 == 0 ? 1.f : -1.f);
		float zDirection = i % 2 == 0 ? (i % 4 == 0 ? 1.f : -1.f) : 0.f;
		lightMesh->position = glm::vec3(xDirection, 0.f, zDirection);
		lightMesh->eulerAngles = glm::vec3(0.f, glm::radians(-90.f * i), 0.f);
		lightMesh->scale = glm::vec3(.1f);
		lightMesh->color = colors[i % colors.size()];
		lightMesh->lightType = lightTypes[i % lightTypes.size()];
		lightMesh->isEnabled = i == 0;

		lightMeshes.push_back(lightMesh);
	}

	Font* arialFont = new Font();
	arialFont->Create(&fontShader, "./Assets/Fonts/arial.ttf", 100);

	GLFWwindow* window = WindowController::GetInstance().GetWindow();
	double lastTime = glfwGetTime();
	float timeSinceLightChange = 0;
	int enabledLightIndex = 0;
	do
	{
		glfwPollEvents();
		float deltaTime = (float)(glfwGetTime() - lastTime);
		lastTime += deltaTime;
		timeSinceLightChange += deltaTime;
		if (timeSinceLightChange > 2.f)
		{
			enabledLightIndex = (enabledLightIndex + 1) % lightMeshes.size();
			timeSinceLightChange = 0.f;
			for (int i = 0; i < lightMeshes.size(); i++) lightMeshes[i]->isEnabled = enabledLightIndex == i;
		}

		glm::vec3 rotationAxis = { 0.f, 1.f, 0.f };
		camera.RotateAround(meshes[0]->position, rotationAxis, -45.f * deltaTime);
		camera.LookAt(meshes[0]->position, rotationAxis);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 vp = camera.GetProjection() * camera.GetView();
		for (auto mesh : meshes) mesh->Render(vp, camera.position, lightMeshes);
		for (auto lightMesh : lightMeshes) lightMesh->Render(vp, camera.position, lightMeshes);

		arialFont->RenderText("Nicholas", 10.f, 50.f, .5f, { 1.f, 1.f, 1.f });
		
		arialFont->RenderText("Enabled Light:", 10.f, 150.f, .5f, {1.f, 1.f, 1.f});
		arialFont->RenderText("Type: " + lightMeshes[enabledLightIndex]->GetLightTypeName(), 10.f, 200.f, .5f, {1.f, 1.f, 1.f});
		arialFont->RenderText("Color: R - " + std::to_string(lightMeshes[enabledLightIndex]->color.x) + ", G - " + std::to_string(lightMeshes[enabledLightIndex]->color.y) + ", B - " + std::to_string(lightMeshes[enabledLightIndex]->color.z), 10.f, 250.f, .5f, {1.f, 1.f, 1.f});

		arialFont->RenderText("Model: " + modelName, 10.f, 350.f, .5f, {1.f, 1.f, 1.f});

		glfwSwapBuffers(window);

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	diffuseShader.Cleanup();
	colorShader.Cleanup();
	fontShader.Cleanup();
	for (auto boxMesh : meshes) delete boxMesh;
	meshes.clear();
	for (auto lightMesh : lightMeshes) delete lightMesh;
	lightMeshes.clear();
	delete arialFont;
}