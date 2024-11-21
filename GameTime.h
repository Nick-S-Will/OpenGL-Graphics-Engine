#pragma once
#ifndef GAMETIME_H
#define GAMETIME_H

#include "StandardIncludes.h"

class GameTime :public Singleton<GameTime>
{
public:
	void Initialize();
	void Update();

	double GetStartTime() const { return lastUpdateTime; }
	double GetDeltaTime() const { return deltaTime; }
	double GetCurrentTime() const { return currentTime; }
	int GetFrameCount() const { return frameCount; }
	int GetFramesPerSecond() const { return framesPerSecond; }

private:
	double lastUpdateTime;
	double lastSampleTime;
	double deltaTime;
	double currentTime;
	int frameCount;
	int framesPerSecond;
	int framesPerSecondCounter;
};

#endif // !GAMETIME_H