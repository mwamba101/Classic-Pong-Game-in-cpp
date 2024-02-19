#include "Game.h"

Game::Game()
{
	mWindow = nullptr;
	mIsRunning = true;
	mTicksCount = 0;

	mBallPos.x = windowW / 2.0f;
	mBallPos.y = windowW / 2.0f;

	mPaddlePos.x = 5;
	mPaddlePos.y = (static_cast<int>(windowH)-(thickness * 2)) / 2;

	mBallVel.x = 200.0f;
	mBallVel.y = 235.0f;
};

bool Game::Initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0)
	{
		SDL_Log("Unable To Initialize SDL: %s", SDL_GetError);

		return false;
	}
	else
	{


		mWindow = SDL_CreateWindow(
			"Classic Pong Game",
			100,
			100,
			static_cast<int>(windowW),
			static_cast<int>(windowH),
			0
		);

		if (!mWindow)
		{
			SDL_Log("Failed To Create Window: %s", SDL_GetError);

			return false;
		}

		mRenderer = SDL_CreateRenderer(
			mWindow,// POINTER TO THE WINDOW WE CREATED
			-1, // THIS IS FOR THE GRAPHICS DRIVER, WILL USE THE DEFAULT -1 SINCE WE ONLY HAVE ONE WINDOW, THIS MEANS WILL LET SDL DECIDE FOR US
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
		);

		if (!mRenderer)
		{
			SDL_Log("Failed To Create Renderer: %s", SDL_GetError);

			return false;
		}

		return true;
	}

}

void Game::ShutDown()
{
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

void Game::ProcessInput()
{
	SDL_Event event;

	//WHILE THERE IS STILL EVENTS COMING IN, KEEP CALLING THE SDL_PollEvent
	while (SDL_PollEvent(&event))
	{
		//CREATING A SWITCH TO PERFORM ACTIONS BASED ON EVENT TYPE
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;

		default:
			break;
		}
	};

	// THIS SECTION GETS THE STATE OF THE KEYBOARD AND PERFORMS ACTIONS BASE ON THEM
	const Uint8* state = SDL_GetKeyboardState(NULL);

	//CHECK IF ESCAPE IS PRESSED AND END THE LOOP IF IT WAS
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	//PADDLE CONTROLL
	if (state[SDL_SCANCODE_UP])
	{
		mPaddleDir -= 1;
	};

	if (state[SDL_SCANCODE_DOWN])
	{
		mPaddleDir += 1;
	};
};

void Game::UpdateGame()
{
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount * 16));

	//Delta time is the difference in ticks from last frame
	//(converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	//clamps maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	};

	//Update the tick count for the next frame
	mTicksCount = SDL_GetTicks();

	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 150.0f * deltaTime;

		if (mPaddlePos.y < thickness)
		{
			mPaddlePos.y = thickness;
		}

		if (mPaddlePos.y >= (windowH - (paddleH + thickness)))
		{
			mPaddlePos.y = windowH - (paddleH + thickness);
		}
	}

	// THIS PART DEALS WITH BALL MOVEMENT
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	};

	if (mBallPos.y >= (static_cast<int>(windowH) - thickness) && mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1;
	};

	if (mBallPos.x >= (static_cast<int>(windowW) - thickness) && mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1;
	}

	if (mBallPos.x <= 0 && mBallVel.x < 0.0f)
	{
		mBallVel.x *= -1;
	}

	if (
		mBallPos.y >= mPaddlePos.y &&
		mBallPos.y <= (mPaddlePos.y + paddleH) &&
		mBallPos.x <= (thickness * 2) && 
		mBallVel.x < 0.0f
		)
	{
		mBallVel.x *= -1;
	};
}

void Game::GenerateOutput()
{
	SDL_Rect topWall{
		0, // top left x co-ordinate
		0,// top left y co-ordinate
		static_cast<int>(windowW), //the width of the rectangle
		thickness // the height of the wall
	};

	SDL_Rect rightWall{
		static_cast<int>(windowW) - thickness,
		0,
		thickness,
		static_cast<int>(windowH)
	};

	SDL_Rect bottomWall{
		0, // top left x co-ordinate
		static_cast<int>(windowH) - thickness,// top left y co-ordinate
		static_cast<int>(windowW), //the width of the rectangle
		thickness // the height of the wall
	};

	SDL_Rect paddle{
		thickness,
		static_cast<int>(mPaddlePos.y),
		thickness,
		static_cast<int>(paddleH)
	};

	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};

	SDL_SetRenderDrawColor(
		mRenderer,
		0, //R THIS IS FOR THE RGBA COLOR 
		0,//G
		255,//B
		255//(A) OPACITY
	);

	SDL_RenderClear(mRenderer); // THIS CLEARS THE BACK BUFFER TO THE CURRENT  DRAW COLOR

	SDL_SetRenderDrawColor(
		mRenderer,
		0, //R THIS IS FOR THE RGBA COLOR 
		255,//G
		0,//B
		255//(A) OPACITY
	);

	SDL_RenderFillRect(mRenderer, &topWall);
	SDL_RenderFillRect(mRenderer, &rightWall);

	SDL_RenderFillRect(mRenderer, &paddle);

	SDL_RenderFillRect(mRenderer, &ball);

	SDL_RenderFillRect(mRenderer, &bottomWall);

	SDL_RenderPresent(mRenderer); // THIS SWAPS THE FRONT AND BACK BUFFER
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		
		ProcessInput();
		
		UpdateGame();
		
		GenerateOutput();
		
	}
}