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
	std::string skyboxDirectory = textureDirectory + "Space Skybox/";
	std::string fontDirectory = "./Assets/Fonts/";

	skyboxShader = Shader();
	skyboxShader.LoadShaders("Skybox.vertexshader", "Skybox.fragmentshader");

	skybox = new Skybox();
	skybox->Create(&skyboxShader, modelDirectory + "Skybox.obj",
		{ skyboxDirectory + "right.png",
		  skyboxDirectory + "left.png",
		  skyboxDirectory + "top.png",
		  skyboxDirectory + "bottom.png",
		  skyboxDirectory + "front.png",
		  skyboxDirectory + "back.png" });

	diffuseShader = Shader();
	diffuseShader.LoadShaders("Diffuse.vertexshader", "Diffuse.fragmentshader");

	Mesh* mesh = new Mesh();
	mesh->Create(&diffuseShader, modelDirectory + "Fighter.ase");
	meshes.push_back(mesh);
	ResetTransform();

	mesh = new Mesh();
	mesh->Create(&diffuseShader, modelDirectory + "Fish.ase");
	meshes.push_back(mesh);

	mesh = new Mesh();
	mesh->Create(&diffuseShader, modelDirectory + "Fish.ase", 100);
	meshes.push_back(mesh);

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

	postProcessor = new PostProcessor();
	postProcessor->Create(&postProcessorShader);
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
		postProcessor->effectType = gameMode == GameMode::WaterScene ? EffectType::Wave : EffectType::None;
		postProcessor->Start();

		if (gameMode != GameMode::SpaceScene) camera.LookAt({ 0.f, 0.f, 20.f }, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
		switch (gameMode)
		{
		case GameMode::MoveLight: UpdateMoveLightScene(); break;
		case GameMode::Transform: UpdateTransformScene(); break;
		case GameMode::WaterScene: UpdateWaterScene(); break;
		case GameMode::SpaceScene: UpdateSpaceScene(); break;
		}

		postProcessor->End();

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		std::vector<std::string> debugLines = {
			"Final Project",
			"FPS: " + std::to_string(GameTime::GetInstance().GetFramesPerSecond()),
			"Mouse Pos: " + std::to_string(mouseX) + " " + std::to_string(mouseY),
			"Left Button: " + (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) ? "Down" : "Up",
			"Right Button: " + (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) ? "Down" : "Up",
			"Fighter Position: " + Vec3ToString(GetFighterMesh()->position),
			"Fighter Rotation: " + Vec3ToString(glm::degrees(GetFighterMesh()->eulerAngles)),
			"Fighter Scale: " + Vec3ToString(GetFighterMesh()->scale)
		};
		RenderLines(debugLines, glm::vec3(1.f, 1.f, 0.f));

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
	postProcessor->Cleanup();
	delete postProcessor;
}

void GameController::UpdateMoveLightScene()
{
	GetFighterMesh()->eulerAngles.x += (float)GameTime::GetInstance().GetDeltaTime();

	lightMeshes[0]->position += 2.f * (float)GameTime::GetInstance().GetDeltaTime() * GetMouseDelta();

	glm::mat4 vp = camera.GetProjection() * camera.GetView();
	lightMeshes[0]->Render(vp, camera.GetPosition(), lightMeshes, specularStrength, specularColor);
	GetFighterMesh()->Render(vp, camera.GetPosition(), lightMeshes, specularStrength, specularColor);
}

void GameController::UpdateTransformScene()
{
	glm::vec3 timeScaledMouseDelta = (float)GameTime::GetInstance().GetDeltaTime() * GetMouseDelta();
	if (transformPosition) GetFighterMesh()->position += 2.f * timeScaledMouseDelta;
	if (transformRotation) GetFighterMesh()->eulerAngles += 3.f * timeScaledMouseDelta;
	if (transformScale) GetFighterMesh()->scale += 1e-2f * timeScaledMouseDelta;

	glm::mat4 vp = camera.GetProjection() * camera.GetView();
	GetFighterMesh()->Render(vp, camera.GetPosition(), lightMeshes, specularStrength, specularColor);
}

void GameController::UpdateWaterScene()
{
	glm::mat4 vp = camera.GetProjection() * camera.GetView();
	GetFishMesh()->Render(vp, camera.GetPosition(), lightMeshes, specularStrength, specularColor);
}

void GameController::UpdateSpaceScene()
{
	camera.RotateAround(glm::vec3(0.f, 0.f, 19.99f), glm::vec3(0.f, 1.f, 0.f), -10.f * (float)GameTime::GetInstance().GetDeltaTime());

	GetFighterMesh()->eulerAngles.x += (float)GameTime::GetInstance().GetDeltaTime();

	glm::mat4 vp = camera.GetProjection() * camera.GetView();
	skybox->Render(vp * glm::translate(glm::mat4(1.f), camera.GetPosition()));
	GetFighterMesh()->Render(vp, camera.GetPosition(), lightMeshes, specularStrength, specularColor);
	GetMultiFishMesh()->Render(vp, camera.GetPosition(), lightMeshes, specularStrength, specularColor);
}