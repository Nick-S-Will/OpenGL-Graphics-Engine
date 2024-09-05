#pragma once
#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "StandardIncludes.h"
#include "Mesh.h"

class GameController : public Singleton<GameController>
{
public:
	GameController();
	virtual ~GameController() {}

	void Initialize();
	void RunGame();

private:
	Mesh mesh;
};

#endif // !GAME_CONTROLLER_H