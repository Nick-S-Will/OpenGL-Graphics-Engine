#include "GameTime.h"

void GameTime::Initialize()
{
	lastUpdateTime = glfwGetTime();
}

void GameTime::Update()
{
	currentTime = glfwGetTime();
	framesPerSecondCounter++;

	if (currentTime - lastSampleTime >= 1.)
	{
		framesPerSecond = framesPerSecondCounter;
		framesPerSecondCounter = 0;
		lastSampleTime = currentTime;
	}

	deltaTime = currentTime - lastUpdateTime;
	lastUpdateTime = currentTime;
}