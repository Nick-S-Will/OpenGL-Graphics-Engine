#pragma once
#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H
#define LIGHT_COUNT 1

#include "StandardIncludes.h"
#include "Camera.h"
#include "Shader.h"
#include "Skybox.h"
#include "Mesh.h"
#include "Font.h"
#include "PostProcessor.h"
#include "WindowController.h"

enum class GameMode : int {
	MoveLight,
	Transform,
	WaterScene,
	SpaceScene
};

class GameController : public Singleton<GameController>
{
public:
	GameMode gameMode = GameMode::MoveLight;

	float specularStrength = 1.f;
	glm::vec3 specularColor = { 1.f, 1.f, 1.f };

	bool transformPosition, transformRotation, transformScale;

	GameController() = default;
	virtual ~GameController() {}

	PostProcessor* GetPostProcessor() { return postProcessor; }
	Mesh* GetFighterMesh() { return meshes[0]; }
	Mesh* GetFishMesh() { return meshes[1]; }

	void Initialize();
	void RunGame();
	void ResetLightPosition() { lightMeshes[0]->position = lightPosition; }
	void ResetTransform()
	{
		GetFighterMesh()->position = glm::vec3(0.f);
		GetFighterMesh()->eulerAngles = glm::vec3(0.f);
		GetFighterMesh()->scale = glm::vec3(1e-2f);
	}

private:
	Camera camera;
	Shader skyboxShader = {};
	Skybox* skybox = nullptr;
	Shader diffuseShader = {};
	std::vector<Mesh*> meshes;
	Shader colorShader = {};
	std::vector<Mesh*> lightMeshes;
	glm::vec3 lightPosition = glm::vec3(0.f, 0.f, 15.f);
	Shader fontShader = {};
	Font* arialFont = nullptr;
	int fontSize = 20;
	Shader postProcessorShader = {};
	PostProcessor* postProcessor = {};
	GLuint vao;

	void LoadAssets();
	void CleanupAssets();
	void UpdateMoveLightScene();
	void UpdateTransformScene();
	void UpdateWaterScene();
	void UpdateSpaceScene();

	void RenderLines(const std::vector<std::string>& lines, const glm::vec3& color)
	{
		for (int i = 0; i < (int)lines.size(); i++)
		{
			arialFont->RenderText(lines[i], fontSize / 4.f, (i + 1.f) * fontSize, color);
		}
	}

	static glm::vec3 GetMouseDelta()
	{
		GLFWwindow* window = WindowController::GetInstance().GetWindow();
		bool leftPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
		bool middlePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;
		auto mouseDelta = glm::vec3(0);
		if (!(leftPressed || middlePressed)) return mouseDelta;

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		int windowWidth, windowHeight;
		glfwGetWindowSize(window, &windowWidth, &windowHeight);

		if (leftPressed)
		{
			mouseDelta.x = 2.f * (float)mouseX / windowWidth - 1.f;
			mouseDelta.y = 1.f - (2.f * (float)mouseY) / windowHeight;
		}

		if (middlePressed)
		{
			mouseDelta.z = 2.f * (float)mouseY / windowHeight - 1.f;
		}

		return mouseDelta;
	}
};

#endif // !GAME_CONTROLLER_H