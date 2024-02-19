#pragma once
#include <SDL.h>

class Game {
public:
	// class constructor
	Game();

	// Game Initializer
	bool Initialize();

	//this runs the game loop until the game is over
	void RunLoop();

	//this shuts down the game
	void ShutDown();

private:
	//these are helper functions for the game loop
	void ProcessInput();

	void UpdateGame();

	void GenerateOutput();

	//WINDOW CREATED BY SDL
	SDL_Window* mWindow;

	//game should continue to run
	bool mIsRunning;

	//THIS IS FOR THE GRAPHICS RENDERER
	SDL_Renderer* mRenderer;

	// this part if for game objects variables
	struct Vector2 {
		float x;
		float y;
	};
	struct Vector2 mPaddlePos;

	struct Vector2 mBallPos;

	struct Vector2 mBallVel;

	// this part is for the delta time
	Uint32 mTicksCount;

	//this is for the paddle movement
	int mPaddleDir = 0;

	const int thickness = 15;

	const float paddleH = 100.0f;
	const float windowH = 480.0f;
	const float windowW = 854.0f;
};