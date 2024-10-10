#pragma once
#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "StandardIncludes.h"
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"

class GameController : public Singleton<GameController>
{
public:
	GameController() = default;
	virtual ~GameController() {}

	void Initialize();
	void RunGame();

private:
	Resolution resolution;
	Camera camera;
	Shader diffuseShader = {};
	Shader colorShader = {};
	Mesh boxMesh = {};
	Mesh lightMesh = {};
};

#endif // !GAME_CONTROLLER_H