#pragma once
#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H
#define CAMERA_COUNT 3

#include "StandardIncludes.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"

class GameController : public Singleton<GameController>
{
public:
	GameController() = default;
	virtual ~GameController() {}

	void Initialize();
	void RunGame();
	void ChangeCamera();
	void ChangeResolution();

private:
	Shader shader = {};
	Mesh mesh = {};
	Resolution resolutions[CAMERA_COUNT];
	Camera cameras[CAMERA_COUNT];
	int resolutionIndex = 0, cameraIndex = 0;

	static float PingPong(const float& t, const float& length)
	{
		float mod = fmod(t, 2 * length);
		return length - fabs(mod - length);
	}
};

#endif // !GAME_CONTROLLER_H