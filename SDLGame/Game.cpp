#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game() :
	mWindow(nullptr),
	mIsRunning(true),
	mRenderer(nullptr),
	mPaddleDir(0), mTicksCount(0)
{
	mBallPos.x = 0;
	mBallPos.y = 0;
	mBallVel.x = 0;
	mBallVel.y = 0;
	mPaddlePos.x = 0;
	mPaddlePos.y = 0;
}

bool Game::Initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)",
		100,
		100,
		1024,
		768,
		0
	);


	if (!mWindow) {
		SDL_Log("Failed to create window : %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer) {
		SDL_Log("Failed to create renderer : %s", SDL_GetError());
		return false;
	}

	//패들 , 공 초기 위치
	mPaddlePos.x = 10.0f;
	mPaddlePos.y = 768.0f / 2.0f;
	mBallPos.x = 1024.0f / 2.0f;
	mBallPos.y = 768.0f / 2.0f;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning) {
		ProcessInput();

		UpdateGame();

		GeneratOutput();
	}
}


void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W]) 
	{
		mPaddleDir = -1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir = 1;
	}
	

}

void Game::UpdateGame()
{
	//프레임고정
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
	{

	}

	float deltaTime = ((SDL_GetTicks() - mTicksCount) / 1500.f);

	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	mTicksCount = SDL_GetTicks();

	//패들 움직임
	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 200.f * deltaTime;
		if (mPaddlePos.y < paddleH / 2.0f + thickness)
		{
			mPaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos.y > 768.0f - paddleH / 2.0f - thickness)
		{
			mPaddlePos.y = 768.0f -paddleH / 2.0f - thickness;
		}
	}

	//볼처리 계속움직임

	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	float diff = mPaddlePos.y - mBallPos.y;
	diff = (diff > 0.0f) ? diff : -diff;
	if (
		diff <= paddleH / 2.f && mBallPos.x <= 25.f && mBallPos.x >= 20.f &&
		mBallVel.x < 0.f)
	{
		mBallVel.x *= -1.f;
	}
	// Did the ball go off the screen? (if so, end game)
	else if (mBallPos.x <= 0.0f)
	{
		mIsRunning = false;
	}
	// Did the ball collide with the right wall?
	else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1.0f;
	}

	// Did the ball collide with the top wall?
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	}
	// Did the ball collide with the bottom wall?
	else if (mBallPos.y >= (768 - thickness) &&
		mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1;
	}
	

}

void Game::GeneratOutput()
{
	//배경색 그리기
	SDL_SetRenderDrawColor(
		mRenderer,
		0,
		0,
		255,
		255
	);
	//후면버퍼 클리어
	SDL_RenderClear(mRenderer);

	//배경색 그리기 투명
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);


	//벽 위아래 그리기
	SDL_Rect wall{
		0,0,1024,thickness
	};
	SDL_RenderFillRect(mRenderer, &wall);
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	//오른쪽벽
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);


	//패들그리기
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);


	//공그리기
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	//스왑 전면 후면 버퍼
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
