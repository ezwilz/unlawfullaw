// -----------------------------------------------------
//	CI411 Esme Wilson 2023 - Semester 2 Coursework
// -----------------------------------------------------

// Libraries and headers included
#include "Game.h" 
#include "GameObjects.h"
#include "Level.h"

// Variables
Game* game = nullptr;

// ----------------------------------------------------- 
int main(int argc, char* argv[])
{

	
	// Frame Limit Variables
	const int frameDelay = 1000 / FPS;
	Uint64 frameStart = 0, frameTime = 0;

	// Create the Game Object
	game = new Game;

	// Start SDL & Create the Game Window
	game->startSDL("Unlawful Law");	
	game->welcomeScreen();
	game->createGameObjects();

	// Main Game Loop
	while (game->isReplaying())
	{ 
		while (game->isRunning())
		{
			frameStart = SDL_GetTicks64();

			game->handleEvents();
			game->update(frameTime);
			game->render();

			// Limit Frame Rate
			frameTime = SDL_GetTicks64() - frameStart;
			if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
		}
		if (game->isReplaying()) game->levelCompleteScreen();
	}
	game->exitScreen();
	game->closeSDL();
	return 0;
}//---







