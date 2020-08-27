#pragma once

#include "SDL.h"

struct Vector2
{
	float x;
	float y;
};

class Game
{
public:
	Game();

	bool Initialize();

	void RunLoop();

	void Shutdown();

private:
	void ProcessInput();
	void UpdateGame();
	void GeneratOutput();

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;

	bool mIsRunning;

	int mPaddleDir;
	// Position of paddle
	Vector2 mPaddlePos;
	// Position of ball
	Vector2 mBallPos;
	// Velocity of ball
	Vector2 mBallVel;
	Uint32 mTicksCount;
};

