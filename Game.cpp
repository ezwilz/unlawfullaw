// Libraries and headers included
#include "Game.h"
#include "GameObjects.h"
#include"Level.h"


// ======================================================= 
// SDL Variables
SDL_Renderer* Game::renderer = nullptr;;
SDL_Event Game::playerInputEvent;
GameInput playerInput;

// Game Objects
GameObject* backGround = nullptr;
PlayerCharacter* pc = nullptr;
NPC* npcs[15] = {};
Friendly* fnpcs[15] = {};
GameObject* terrainBlocks[250];
Levels* levelMaps = nullptr;


// Text
 
// Gobal Text Variables
TTF_Font* font = nullptr;
TTF_Font* font2 = nullptr;
SDL_Color textColour = { 255, 255, 200 };
SDL_Surface* textSurface = nullptr;
SDL_Texture* textTexture = nullptr;

// ======================================================= 
void Game::createGameObjects()
{
	// Create Level Object
	levelMaps = new Levels;

	printf("\nCreating Game Objects");
	// Create Background
	backGround = new GameObject("assets/images/Level_1_Bg.png", 0, 0);
	backGround->setSize(800, 600); // as not a standard sprite size
	
	
	

	// Create Game Objects - filename , x and y pos, initial angle
	pc = new PlayerCharacter("assets/images/Pc_Car.png", 0, 0, 0);

	// Create an Array of NPCs
	for (int i = 0; i < sizeof(npcs) / sizeof(npcs[0]); i++)
	{
		npcs[i] = new NPC("assets/images/Npc_Enemy_Car.png", 0, 0, 0);
		npcs[i]->setSpeed(80);
		if (currentLevel == 3)
		{
			npcs[i]->setSpeed(100);
		}
	}

	//Create an array of friendlies
	for (int i = 0; i < sizeof(fnpcs) / sizeof(fnpcs[0]); i++)
	{
		fnpcs[i] = new Friendly("assets/images/Npc_Friendly_Car.png", 0, 0, 0);
		fnpcs[i]->setSpeed(80);
	
	}



	// create the invisible terrain tiles
	srand(time(NULL)); // seed the random number for variation
	for (int i = 0; i < sizeof(terrainBlocks) / sizeof(terrainBlocks[0]); i++)
	{
		terrainBlocks[i] = new GameObject("assets/images/Square_Cross_Blue.png", 0, 0);
	}
	// Load Map  
	loadMap(1);
	currentLevel = 1;
	
}//----


void Game::loadMap(int levelNumber)
{
	std::cout << "\nLoading Level " << levelNumber;

	for (int row = 0; row < 18; row++)
	{
		for (int col = 0; col < 25; col++)
		{
			if (levelMaps->getTileContent(levelNumber, col, row) == 1) //  Terrain 
			{
				for (GameObject* block : terrainBlocks)
				{
					if (block->getAliveState() == false)
					{
						block->setAlive(true);
						block->setX(col * SPRITE_SIZE);
						block->setY(row * SPRITE_SIZE);				
						break;
					}
				}
			}

			if (levelMaps->getTileContent(levelNumber, col, row) == 2) // PC
			{
				pc->setX(col * SPRITE_SIZE);
				pc->setY(row * SPRITE_SIZE);
			}

			if (levelMaps->getTileContent(levelNumber, col, row) == 3) //  NPC
			{
				for (NPC* npc : npcs)
				{
					if (npc->getAliveState() == false)
					{
						npc->setAlive(true);
						npc->setX(col * SPRITE_SIZE);
						npc->setY(row * SPRITE_SIZE);
						break;
					}
				}
			}



			if (levelMaps->getTileContent(levelNumber, col, row) == 4) //  Items
			{
				for (Friendly* fnpc : fnpcs)
				{
					if (fnpc->getAliveState() == false)
					{
						fnpc->setAlive(true);
						fnpc->setX(col * SPRITE_SIZE);
						fnpc->setY(row * SPRITE_SIZE);
						break;
					}
				}
			}

			
		}
	}
}//---



// ======================================================= 

void Game::checkCollision()
{
	// Create the Rects for checking what collides
	SDL_Rect pcRect = { pc->getX(), pc->getY(), SPRITE_SIZE, SPRITE_SIZE };
	SDL_Rect objectRect = { -100,-100, SPRITE_SIZE, SPRITE_SIZE };
	SDL_Rect npcRect = { -100,-100, SPRITE_SIZE, SPRITE_SIZE };
	SDL_Rect fnpcRect = { -100,-100, SPRITE_SIZE, SPRITE_SIZE };
	SDL_Rect bulletRect = { 0,0,0,0 };

	// Set the PC's next Position by adding velocity
	SDL_Rect pcNextRect = { pc->getX() + pc->getVelX(), pc->getY() + pc->getVelY(), SPRITE_SIZE, SPRITE_SIZE };

	for (GameObject* block : terrainBlocks)  //  Terrain -----------------
	{
		if (block->getAliveState())
		{
			objectRect.x = block->getX();
			objectRect.y = block->getY();

			if (SDL_HasIntersection(&pcNextRect, &objectRect))  // PC -----
			{
				pc->stop(); // Stop the PC moving
			}

			for (NPC* npc : npcs) // NPCs ---------
			{
				if (npc->getAliveState() == true)
				{
					npcRect.x = npc->getX(); // Update the rect
					npcRect.y = npc->getY();

					if (SDL_HasIntersection(&npcRect, &objectRect))
					{
						npc->changeDirection();
					}
				}
			}

			for (Friendly* fnpc : fnpcs) // FNPCs ---------
			{
				if (fnpc->getAliveState() == true)
				{
					fnpcRect.x = fnpc->getX(); // Update the rect
					fnpcRect.y = fnpc->getY();

					if (SDL_HasIntersection(&fnpcRect, &objectRect))
					{
						fnpc->changeDirection();
					}
				}
			}

		
		}
	}


	



}//---

// ======================================================= 

void Game::checkAttacks()
{
	
}//---

// ======================================================= 

void Game::checkGameStates()
{
	

	activeNPCs = 0;
	for (NPC* npc : npcs)
	{
		if (npc->getAliveState()) activeNPCs++;
	}

	activeFNPCs = 0;
	for (Friendly* fnpc : fnpcs)
	{
		if (fnpc->getAliveState()) activeFNPCs++;
	}

	//check NPCs are cleared
	if (activeNPCs == 0) gameRunning = false;


	// Check if PC is alive
	if (pc->getHP() <= 0) gameRunning = false;

}//---



// ======================================================= 


void Game::update(float frameTime)
{
	// frametime no longer dynamic, fixed at 1fps rate
	frameTime = static_cast<float>(1) / FPS;

	pc->updatePC(playerInput.keyPressed, frameTime);

	// Set NPC Behaviours
	for (NPC* npc : npcs)
	{
		if (npc->getAliveState())
		{
			
			npc->roam(frameTime);
			
			npc->updateNPC();
			//npc->screenCrawl(frameTime);		
		}
	}
	for (Friendly* fnpc : fnpcs)
	{
		if (fnpc->getAliveState())
		{
			//fnpc->roam(frameTime);
			
			fnpc->patrolFriendly(frameTime);
			
			fnpc->updateFriendly();
			//npc->screenCrawl(frameTime);		
		}
	}

	for (GameObject* block : terrainBlocks)
	{
		if (block->getAliveState()) block->update();
	}



	//check if PC hit NPCs
	SDL_Rect objectRect = { -100, -100, SPRITE_SIZE, SPRITE_SIZE };
	SDL_Rect pcRect = { pc->getX(), pc->getY(), SPRITE_SIZE, SPRITE_SIZE };
	
	for (int i = 0; i < sizeof(npcs) / sizeof(npcs[0]); i++)
	{
		//only check alive NPCs
		if (npcs[i]->getAliveState() == true)
		{
			objectRect.x = npcs[i]->getX();
			objectRect.y = npcs[i]->getY();
			
			if (SDL_HasIntersection(&pcRect, &objectRect))
			{
				npcs[i]->setAlive(false);//disable the NPCs hit
			}
		}
	}

	for (int i = 0; i < sizeof(fnpcs) / sizeof(fnpcs[0]); i++)
	{
		//only check alive FNPCs
		if (fnpcs[i]->getAliveState() == true)
		{
			objectRect.x = fnpcs[i]->getX();
			objectRect.y = fnpcs[i]->getY();

			if (SDL_HasIntersection(&pcRect, &objectRect))
			{
				fnpcs[i]->setAlive(false);//disable the NPCs hit
				pc->changeHP(-21);
			}
		}
	}

	// damge the pc when terrain blocks are hit
	for (int i = 0; i < sizeof(terrainBlocks) / sizeof(terrainBlocks[0]); i++)
	{
		//check the terrain blocks
		if (terrainBlocks[i]->getAliveState() == true)
		{
			objectRect.x = terrainBlocks[i]->getX();
			objectRect.y = terrainBlocks[i]->getY();

			if (SDL_HasIntersection(&pcRect, &objectRect))
			{
				pc->changeHP(-10);//damage the pc for hitting terrain blocks
			}
		}
	}
	pc->getTimer();
	checkAttacks();
	checkCollision();
	checkGameStates();
	

}//---


// =======================================================
void Game::updateGUI()
{
	std::string  screenText;
	SDL_Rect textRect = { 8,8,0,0 }; // start position of the text

	// text to be on screen Left Side	
	screenText += "Criminals: " + std::to_string(activeNPCs);
	textColour = { 0, 0, 0 };

	// render the text to screen
	textSurface = TTF_RenderText_Blended_Wrapped(font, screenText.c_str(), textColour, 0);
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);


	// text to be on screen Right Side

	std::string level = std::to_string(currentLevel++);

	textRect = { 300,580,0,0 }; // start position of the text
	screenText = "HP: " + std::to_string(int(pc->getHP()));
	screenText += "      Time: " + std::to_string(int(pc->getTimer()));
	textColour = { 0, 255, 0 };

	// render the text to screen
	textSurface = TTF_RenderText_Blended_Wrapped(font2, screenText.c_str(), textColour, 0);
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);


	// Clear the memory
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

}//--



// ======================================================= 
void Game::render()
{
	SDL_RenderClear(renderer);
	backGround->render();
	for (GameObject* block : terrainBlocks)
	{
		if (block->getAliveState() == true)  block->render();
	}
	for (NPC* npc : npcs)
	{
		if (npc->getAliveState()) npc->renderNPC();
	}
	for (Friendly* fnpc : fnpcs)
	{
		if (fnpc->getAliveState() == true)  fnpc->renderFriendly();
	}
	pc->renderPC();
	/*drawHPBars();*/
	updateGUI();
	SDL_RenderPresent(renderer); 	// Update the screen
}//---

// ======================================================= 
void Game::handleEvents()
{
	// Reset Inputs
	playerInput.keyPressed = NULL;
	playerInput.mouseL = false;
	playerInput.mouseR = false;

	//Check for Events
	SDL_PollEvent(&playerInputEvent);

	switch (playerInputEvent.type)
	{
	case SDL_QUIT:
		gameRunning = false;
		break;

	case SDL_MOUSEMOTION: //--------- New Input
		int mouseXpos, mouseYpos;
		SDL_GetMouseState(&mouseXpos, &mouseYpos);
		playerInput.mouseX = mouseXpos;
		playerInput.mouseY = mouseYpos;

	case SDL_MOUSEBUTTONDOWN: //--------- New Input
		if (playerInputEvent.button.button == SDL_BUTTON_LEFT) playerInput.mouseL = true;
		if (playerInputEvent.button.button == SDL_BUTTON_RIGHT) playerInput.mouseR = true;

	case SDL_KEYDOWN:
		//std::cout << "\n" << playerInputEvent.key.keysym.sym;
		playerInput.keyPressed = playerInputEvent.key.keysym.sym;
		break;

	default:
		break;
	}
}//---

// ======================================================= 
void Game::startSDL(const char* title)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "\nSDL Initialised  \n";
		gameWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
		if (gameWindow) printf("\nGame Window Created ");
		renderer = SDL_CreateRenderer(gameWindow, -1, 0);
		if (renderer) printf("\nRenderer Created \n");
		gameRunning = true;

		// Initialise Fonts
		TTF_Init();
		font = TTF_OpenFont("assets/fonts/upheavtt.ttf", 22); // size is the last value
		font2 = TTF_OpenFont("assets/fonts/upheavtt.ttf", 18); // size is the last value
	}
	else
	{
		gameRunning = false;
	}
}//---

// ======================================================= 

void Game::welcomeScreen()
{
	GameObject* splashScreen;
	splashScreen = new GameObject("assets/images/unlawful_title.png", 0, 0);
	splashScreen->setSize(800, 600);
	splashScreen->render();
	SDL_RenderPresent(renderer);
	SDL_Delay(500);

	// Load a Background to cover the sprites
	GameObject* background;
	background = new GameObject("assets/images/Title_Screen_Rules.png", 0, 0);
	background->setSize(800, 600);
	background->render();
	SDL_RenderPresent(renderer);

	// text Variables
	std::string screenText;
	textColour = { 0, 0, 0 };
	SDL_Rect textRect = { 100, 150,0,0 }; // start position of the text	

	screenText += "\n \n \n\n Detain [Crash into] the roaming criminals";
	screenText += "\n \n Don't hit civillians";
	screenText += "\n \n WASD to Drive";
	screenText += "\n \n \n\n\n Press any key to start";

	// render the text to screen
	textSurface = TTF_RenderText_Blended_Wrapped(font2, screenText.c_str(), textColour, 0);
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
	SDL_RenderPresent(renderer);
	// Clear the memory
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

	// Wait for key press or mouse click
	bool exitLoop = false;
	while (!exitLoop)
	{
		SDL_PollEvent(&playerInputEvent);
		if (playerInputEvent.type == SDL_MOUSEBUTTONDOWN || playerInputEvent.type == SDL_KEYDOWN)
		{
			exitLoop = true;
		}
	}

	
}

void Game::resetAllObjects()
{
	// Turn off all objects

	for (GameObject* block : terrainBlocks)
		block->setAlive(false);

	for (NPC* npc : npcs)
	{
		npc->setAlive(false);
		npc->setHP(100); // reset NPC health

	}

	for (Friendly* fnpc : fnpcs)
	{
		fnpc->setAlive(false);
		fnpc->setHP(100); // reset NPC health

	}

	// Reset PC stats
	pc->setHP(100);
	pc->setTimer((120 - SDL_GetTicks64() / 1000));
	// anything else that need resetting 
}//---


void Game::levelCompleteScreen()
{
	// Load a Background to cover the sprites
	GameObject* background;
	background = new GameObject("assets/images/Title_Screen_Rules.png", 0, 0);
	background->setSize(800, 600);
	background->render();
	SDL_RenderPresent(renderer);
	// text Variables
	std::string screenText;
	textColour = { 0, 0, 0 };
	SDL_Rect textRect = { 200, 250,0,0 }; // start position of the text		

	if (pc->getHP() <= 0) // PC Died - replay current level
	{
		// Display Retry Message
		screenText = "Your Character Died \n\nTry not to hit the walls or civillians\n\n \n\nPress any key to try again \n\nOr click to exit the game";
		resetAllObjects();
		// reload the same map
		if (currentLevel == 1) loadMap(1);
		if (currentLevel == 2) loadMap(2);
		if (currentLevel == 3) loadMap(3);
	}
	else // level complete move on
	{
		// Display Continue Message
		switch (currentLevel)
		{
			case 1: 
				screenText = "Well Done, \n\nPress any key to continue to the next level \n\nOr click to exit the game";
			break;

			case 2:
				screenText = "Well Done, \n\nPress any key to continue to the next level \n\nOr click to exit the game";
				break;
			case 3:
				screenText = "Well Done, You have detained [killed] all the criminals \n\nPress any key to play again \n\nOr click to exit the game";
				break;
			default:
				screenText = "Well Done, \n\nPress any key to continue to the next level \n\nOr click to exit the game";
		}
			
		resetAllObjects();
		// load the next map
		if (currentLevel == 1)
		{
			loadMap(2);
			currentLevel = 2;
			backGround = new GameObject("assets/images/Level_2_Bg.png", 0, 0);
			backGround->setSize(800, 600); // as not a standard sprite size

		}
		else if (currentLevel == 2)
		{
			loadMap(3);
			currentLevel = 3;
			backGround = new GameObject("assets/images/Level_3_Bg.png", 0, 0);
			backGround->setSize(800, 600); // as not a standard sprite size
		}
		else if (currentLevel == 3)
		{
			loadMap(1);
			currentLevel = 1;
		}
	}
	// render the text to screen
	textSurface = TTF_RenderText_Blended_Wrapped(font2, screenText.c_str(), textColour, 0);
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
	SDL_RenderPresent(renderer);
	// Clear the memory
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

	// Pause to ensure game actions have stopped
	SDL_Delay(2000);
	// Wait for key press or mouse click
	bool exitLoop = false;
	while (!exitLoop)
	{
		SDL_PollEvent(&playerInputEvent);
		if (playerInputEvent.type == SDL_KEYDOWN)
		{
			exitLoop = true;
		}
		else if (playerInputEvent.type == SDL_MOUSEBUTTONDOWN)
		{
			exitLoop = true;
			replay = false;
		}
		gameRunning = true;
	}
	
}//---


// ======================================================= 
void Game::exitScreen()
{
	// Load a Background to cover the sprites
	GameObject* background;
	background = new GameObject("assets/images/Title_Screen_Rules.png", 0, 0);
	background->setSize(800, 600);
	background->render();
	SDL_RenderPresent(renderer);

	// text Variables
	std::string screenText;
	textColour = { 0, 0, 0 };
	SDL_Rect textRect = { 250, 250,0,0 }; // start position of the text	

	screenText = "Thanks for Playing Unlawful Law\n \n     See ya!";

	// render the text to screen
	textSurface = TTF_RenderText_Blended_Wrapped(font2, screenText.c_str(), textColour, 0);
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
	SDL_RenderPresent(renderer);
	// Clear the memory
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

	SDL_Delay(3000);


}//---
Game::Game() // Constructor
{
	printf(" \n\n ----- Game Started ------ \n");
}



//// ======================================================= 
void Game::closeSDL() // Clear Memory and exit SDL
{
	SDL_DestroyWindow(gameWindow);
	SDL_DestroyRenderer(renderer);
	TTF_CloseFont(font);
	TTF_CloseFont(font2);
	TTF_Quit();
	SDL_Quit();
	std::cout << "\nSDL Closed \n";
}//---

// ======================================================= 


// ======================================================= 