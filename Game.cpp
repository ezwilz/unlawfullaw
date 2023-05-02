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
GameObject* items[3] = {};
NPC* npcs[15] = {};
Friendly* fnpcs[15] = {};
Projectile* bulletsPC[10] = {};
Projectile* bulletsNPC[20] = {};
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
		npcs[i]->setNextShotTime(rand() % 10000); // Set Random shot time upto 10 Secs
	}

	//Create an array of friendlies
	for (int i = 0; i < sizeof(fnpcs) / sizeof(fnpcs[0]); i++)
	{
		fnpcs[i] = new Friendly("assets/images/Npc_Friendly_Car.png", 0, 0, 0);
		fnpcs[i]->setSpeed(80);
		fnpcs[i]->setNextShotTime(rand() % 10000);

	}


	//// Create the array of items
	//for (int i = 0; i < sizeof(items) / sizeof(items[0]); i++)
	//{
	//	items[i] = new GameObject("assets/images/Npc_Friendly_Car.png", 0, 0);
	//}

	//create PC bullets but do not enable
	for (int i = 0; i < sizeof(bulletsPC) / sizeof(bulletsPC[0]); i++)
	{
		bulletsPC[i] = new Projectile("assets/images/Circle_8.png", -100, -100, 0, 8);
		bulletsPC[i]->setBulletSpeed(300);
		bulletsPC[i]->setDamage(35);
	}

	//create NPC bullets but do not enable
	for (int i = 0; i < sizeof(bulletsNPC) / sizeof(bulletsNPC[0]); i++)
	{
		bulletsNPC[i] = new Projectile("assets/images/Circle_Red_8.png", -100, -100, 90, 8);
		bulletsNPC[i]->setBulletSpeed(250);
		bulletsNPC[i]->setDamage(20);
	}

	// Create Terrain Tiles
	srand(time(NULL)); // seed the random number for variation
	for (int i = 0; i < sizeof(terrainBlocks) / sizeof(terrainBlocks[0]); i++)
	{
		terrainBlocks[i] = new GameObject("assets/images/Transparent_Terrain.png", 0, 0);
	}
	// Load Map  
	loadMap(1);
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
					npcRect.x = fnpc->getX(); // Update the rect
					npcRect.y = fnpc->getY();

					if (SDL_HasIntersection(&npcRect, &objectRect))
					{
						fnpc->changeDirection();
					}
				}
			}

			for (Projectile* bullet : bulletsPC)  // PC  Bullets -----------------
			{
				if (bullet->getAliveState() == true)
				{
					bulletRect.x = bullet->getX(); // Update the rect
					bulletRect.y = bullet->getY();
					bulletRect.w = bulletRect.h = bullet->getSize();

					if (SDL_HasIntersection(&objectRect, &bulletRect))
					{
						bullet->setAlive(false); // disable bullet
					}
				}
			}
		}
	}


	for (Projectile* bullet : bulletsNPC)  //  NPC Bullets -----------------
	{
		if (bullet->getAliveState() == true)
		{
			bulletRect.x = bullet->getX(); // Update the rect
			bulletRect.y = bullet->getY();
			bulletRect.w = bulletRect.h = bullet->getSize();

			if (SDL_HasIntersection(&pcRect, &bulletRect))  //  PC ------
			{
				pc->changeHP(-bullet->getDamage()); // Apply damage
				bullet->setAlive(false); // disable bullet
			}

			for (GameObject* block : terrainBlocks)  //  Terrain ------
			{
				if (block->getAliveState())
				{
					objectRect.x = block->getX();
					objectRect.y = block->getY();

					if (SDL_HasIntersection(&objectRect, &bulletRect))
					{
						bullet->setAlive(false); // disable bullet
					}
				}
			}
		}
	}

	for (GameObject* item : items) // Pickups items
	{
		// Only check Alive Items
		if (item->getAliveState() == true)
		{
			objectRect.x = item->getX();
			objectRect.y = item->getY();

			if (SDL_HasIntersection(&pcRect, &objectRect))
			{
				item->setAlive(false); // Disable the item hit
				pc->changeHP(-10);
			}
		}
	}

	// check what alive NPCs hit -
	for (NPC* npc : npcs)
	{
		if (npc->getAliveState() == true)
		{
			npcRect.x = npc->getX(); // Update the rect
			npcRect.y = npc->getY();

			for (Projectile* bullet : bulletsPC)  // PC  Bullets -----------------
			{
				if (bullet->getAliveState() == true)
				{
					bulletRect.x = bullet->getX(); // Update the rect
					bulletRect.y = bullet->getY();
					bulletRect.w = bulletRect.h = bullet->getSize();

					if (SDL_HasIntersection(&npcRect, &bulletRect)) // NPC
					{
						//npc->setAlive(false); // Disable the NPC 						
						npc->changeHP(-bullet->getDamage()); // Apply damage

						bullet->setAlive(false); // disable bullet
					}
				}
			}
		}
	}

	for (Friendly* fnpc : fnpcs)
	{
		if (fnpc->getAliveState() == true)
		{
			fnpcRect.x = fnpc->getX(); // Update the rect
			fnpcRect.y = fnpc->getY();

			for (Projectile* bullet : bulletsPC)  // PC  Bullets -----------------
			{
				if (bullet->getAliveState() == true)
				{
					bulletRect.x = bullet->getX(); // Update the rect
					bulletRect.y = bullet->getY();
					bulletRect.w = bulletRect.h = bullet->getSize();

					if (SDL_HasIntersection(&fnpcRect, &bulletRect)) // NPC
					{
						//npc->setAlive(false); // Disable the NPC 						
						fnpc->changeHP(-bullet->getDamage()); // Apply damage

						bullet->setAlive(false); // disable bullet
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
	activeItems = 0;
	for (GameObject* item : items)
	{
		if (item->getAliveState()) activeItems++;
	}

	activeNPCs = 0;
	for (NPC* npc : npcs)
	{
		if (npc->getAliveState()) activeNPCs++;
	}

	activeFriendly = 0;
	for (Friendly* fnpc : fnpcs)
	{
		if (fnpc->getAliveState()) activeFriendly++;
	}

	// Check if PC is alive
	if (pc->getHP() < 0) gameRunning = false;

}//---



// ======================================================= 


void Game::update(float frameTime)
{
	// Ensure Frame rate is at the delay speed and convert to deltaTime
	if (frameTime < 1000 * (float)1 / FPS) frameTime = (float)1 / FPS;

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
			fnpc->roam(frameTime);
			fnpc->updateFriendly();
			//npc->screenCrawl(frameTime);		
		}
	}

	for (GameObject* block : terrainBlocks)
	{
		if (block->getAliveState()) block->update();
	}


	for (GameObject* item : items) // Update Items
	{
		if (item->getAliveState() == true) item->update();
	}

	for (Projectile* bullet : bulletsPC) //--------- New Bullet
	{
		if (bullet->getAliveState()) bullet->update(frameTime);
	}

	for (Projectile* bullet : bulletsNPC) //--------- New Bullet
	{
		if (bullet->getAliveState()) bullet->update(frameTime);
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
				npcs[i]->setAlive(false);//disable the NPCs hit
				pc->changeHP(-10);
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
	screenText = "Items: " + std::to_string(activeItems);
	screenText += "     NPCs: " + std::to_string(activeNPCs);
	textColour = { 255, 255, 0 };

	// render the text to screen
	textSurface = TTF_RenderText_Blended_Wrapped(font, screenText.c_str(), textColour, 0);
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);


	// text to be on screen Right Side
	textRect = { 400,580,0,0 }; // start position of the text
	screenText = "HP: " + std::to_string(int(pc->getHP()));
	screenText += "      Time: " + std::to_string(SDL_GetTicks64() / 1000);
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


void Game::drawHPBars()
{
	SDL_Rect hpBar{ 0,0,0,0 };

	for (NPC* npc : npcs) //Draw NPC Bars
	{
		if (npc->getAliveState())
		{
			hpBar.h = 2;
			hpBar.w = SPRITE_SIZE * npc->getHP() / 100; ;
			hpBar.x = npc->getX();
			hpBar.y = npc->getY() - 10;

			// Colour of HP Bar
			if (npc->getHP() >= 50) // Green
				SDL_SetRenderDrawColor(Game::renderer, 0, 200, 00, 255);

			else if (npc->getHP() >= 25 && npc->getHP() < 50) // Yellow
				SDL_SetRenderDrawColor(Game::renderer, 250, 250, 0, 255);

			else // Red
				SDL_SetRenderDrawColor(Game::renderer, 250, 0, 0, 255);

			SDL_RenderDrawRect(Game::renderer, &hpBar);
		}
	}

	for (Friendly* fnpc : fnpcs) //Draw NPC Bars
	{
		if (fnpc->getAliveState())
		{
			hpBar.h = 2;
			hpBar.w = SPRITE_SIZE * fnpc->getHP() / 100; ;
			hpBar.x = fnpc->getX();
			hpBar.y = fnpc->getY() - 10;

			// Colour of HP Bar
			if (fnpc->getHP() >= 50) // Green
				SDL_SetRenderDrawColor(Game::renderer, 0, 200, 00, 255);

			else if (fnpc->getHP() >= 25 && fnpc->getHP() < 50) // Yellow
				SDL_SetRenderDrawColor(Game::renderer, 250, 250, 0, 255);

			else // Red
				SDL_SetRenderDrawColor(Game::renderer, 250, 0, 0, 255);

			SDL_RenderDrawRect(Game::renderer, &hpBar);
		}
	}

	// Draw PC Bar
	hpBar.w = SPRITE_SIZE * pc->getHP() / 100;
	hpBar.x = pc->getX();
	hpBar.y = pc->getY() - 10;

	// Colour of HP Bar
	if (pc->getHP() >= 50) // Green
		SDL_SetRenderDrawColor(Game::renderer, 0, 200, 00, 255);

	else if (pc->getHP() >= 25 && pc->getHP() < 50) // Yellow
		SDL_SetRenderDrawColor(Game::renderer, 250, 250, 0, 255);

	else // Red
		SDL_SetRenderDrawColor(Game::renderer, 250, 0, 0, 255);

	SDL_RenderDrawRect(Game::renderer, &hpBar);
}//----



// ======================================================= 
void Game::render()
{
	SDL_RenderClear(renderer);
	backGround->render();
	for (GameObject* block : terrainBlocks)
	{
		if (block->getAliveState() == true)  block->render();
	}
	for (GameObject* item : items)
	{
		if (item->getAliveState() == true)  item->render();
	}
	for (NPC* npc : npcs)
	{
		if (npc->getAliveState()) npc->renderNPC();
	}
	for (Projectile* bullet : bulletsPC)
	{
		if (bullet->getAliveState()) bullet->renderProjectile();
	}
	for (Projectile* bullet : bulletsNPC)
	{
		if (bullet->getAliveState()) bullet->renderProjectile();
	}
	for (Friendly* fnpc : fnpcs)
	{
		if (fnpc->getAliveState() == true)  fnpc->renderFriendly();
	}
	pc->renderPC();
	drawHPBars();
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
	SDL_Delay(2500);
}//---

// ======================================================= 
void Game::exitScreen()
{
	printf("\n\n ----- Thank you for playing -----");

	if (pc->getHP() < 0)
	{
		GameObject* splashScreen;
		splashScreen = new GameObject("assets/images/GameOverScreen.png", 0, 0);
		splashScreen->setSize(800, 600);
		splashScreen->render();
		SDL_RenderPresent(renderer);
		SDL_Delay(2000);
	}
	SDL_Delay(500);

}//---

// ======================================================= 
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
Game::Game() // Constructor
{
	printf(" \n\n ----- Game Started ------ \n");
}

// ======================================================= 