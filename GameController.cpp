#include "GameController.h"
#include "GameTime.h"

void GameController::Initialize()
{
	GameTime::GetInstance().Initialize();
	GLFWwindow* window = WindowController::GetInstance().NewWindow("Final Project");
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
	camera.LookAt({ 0.f, 0.f, 20.f }, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
}

void GameController::LoadAssets()
{
	std::string modelDirectory = "./Assets/Models/";
	std::string textureDirectory = "./Assets/Textures/";
	std::string skyboxDirectory = textureDirectory + "Ocean Skybox/";
	std::string fontDirectory = "./Assets/Fonts/";

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

	diffuseShader = Shader();
	diffuseShader.LoadShaders("Diffuse.vertexshader", "Diffuse.fragmentshader");

	Mesh* mesh = new Mesh();
	mesh->Create(&diffuseShader, modelDirectory + "Fighter.ase");
	meshes.push_back(mesh);
	ResetTransform();

	colorShader = Shader();
	colorShader.LoadShaders("Color.vertexshader", "Color.fragmentshader");

	std::vector<LightType> lightTypes{ LightType::Point };
	for (int i = 0; i < LIGHT_COUNT; i++)
	{
		Mesh* lightMesh = new Mesh();
		lightMesh->Create(&colorShader, modelDirectory + "Sphere.obj");
		lightMesh->position = lightPosition;
		lightMesh->scale = glm::vec3(3e-1f);
		lightMesh->lightType = lightTypes[i % lightTypes.size()];

		lightMeshes.push_back(lightMesh);
	}

	fontShader = Shader();
	fontShader.LoadShaders("Font.vertexshader", "Font.fragmentshader");

	arialFont = new Font();
	arialFont->Create(&fontShader, fontDirectory + "arial.ttf", fontSize);

	postProcessorShader = Shader();
	postProcessorShader.LoadShaders("PostProcessor.vertexshader", "PostProcessor.fragmentshader");

	postProcessor = PostProcessor();
	postProcessor.Create(&postProcessorShader);
}

void GameController::RunGame()
{
	LoadAssets();

	GLFWwindow* window = WindowController::GetInstance().GetWindow();
	do
	{
		glfwPollEvents();
		GameTime::GetInstance().Update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		postProcessor.Start();

		switch (gameMode)
		{
		case GameMode::MoveLight: UpdateMoveLightScene(); break;
		case GameMode::Transform: UpdateTransformScene(); break;
		case GameMode::WaterScene:  break;
		case GameMode::SpaceScene:  break;
		}

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		std::vector<std::string> debugLines = {
			"Final Project",
			"FPS: " + std::to_string(GameTime::GetInstance().GetFramesPerSecond()),
			"Mouse Pos: " + std::to_string(mouseX) + " " + std::to_string(mouseY),
			"Left Button: " + (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) ? "Down" : "Up",
			"Right Button: " + (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) ? "Down" : "Up",
			"Fighter Position: " + Vec3ToString(meshes[0]->position),
			"Fighter Rotation: " + Vec3ToString(glm::degrees(meshes[0]->eulerAngles)),
			"Fighter Scale: " + Vec3ToString(meshes[0]->scale)
		};
		RenderLines(debugLines, glm::vec3(1.f, 1.f, 0.f));

		postProcessor.End();
		glfwSwapBuffers(window);
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	CleanupAssets();
}

void GameController::CleanupAssets()
{
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
}

void GameController::UpdateMoveLightScene()
{
	meshes[0]->eulerAngles.x += (float)GameTime::GetInstance().GetDeltaTime();

	lightMeshes[0]->position += 2.f * (float)GameTime::GetInstance().GetDeltaTime() * GetMouseDelta();

	glm::mat4 vp = camera.GetProjection() * camera.GetView();
	lightMeshes[0]->Render(vp, camera.GetPosition(), lightMeshes, specularStrength, specularColor);
	meshes[0]->Render(vp, camera.GetPosition(), lightMeshes, specularStrength, specularColor);
}

void GameController::UpdateTransformScene()
{
	glm::vec3 timeScaledMouseDelta = (float)GameTime::GetInstance().GetDeltaTime() * GetMouseDelta();
	if (transformPosition) meshes[0]->position += 2.f * timeScaledMouseDelta;
	if (transformRotation) meshes[0]->eulerAngles += 3.f * timeScaledMouseDelta;
	if (transformScale) meshes[0]->scale += 1e-2f * timeScaledMouseDelta;

	glm::mat4 vp = camera.GetProjection() * camera.GetView();
	meshes[0]->Render(vp, camera.GetPosition(), lightMeshes, specularStrength, specularColor);
}