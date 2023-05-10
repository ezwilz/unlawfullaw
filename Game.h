#pragma once

// ======================================================= 
// Libraries / Headers to include 
#include <SDL.h>
#include<SDL_Image.h>
#include<SDL_ttf.h>
#include<iostream>
#include<string>


// Constants
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define ROWS = 18
#define COLUMNS = 25
#define SPRITE_SIZE 32
#define FPS 50


// ======================================================= 
class Game
{
public:	

	Game(); // constructor
	bool isRunning() { return gameRunning; }
	void startSDL(const char* title);
	void welcomeScreen();
	void createGameObjects();
	void loadMap(int levelNumber);
	void handleEvents();
	void checkCollision();
	void checkAttacks();
	void checkGameStates();
	
	//timer functions
	void updateGUI();
	int getLevel() { return currentLevel; }

	void update(float frametime);
	void render();
	void closeSDL();
	void exitScreen();
	bool isReplaying() { return replay; }
	void levelCompleteScreen();
	void resetAllObjects();

	static SDL_Renderer* renderer;
	static SDL_Event playerInputEvent;

private:
	bool gameRunning = false;
	SDL_Window* gameWindow = nullptr;	
	bool replay = true;

	int activeNPCs = 0;
	int activeFNPCs = 0;
	int currentLevel = 0;
};

// =======================================================
struct GameInput
{
	int mouseX = 0, mouseY = 0;
	bool mouseL = false, mouseR = false;
	bool mouseUpL = false;
	bool mouseWheelUp = false, mouseWheelDown = false;
	int keyPressed = NULL;
};