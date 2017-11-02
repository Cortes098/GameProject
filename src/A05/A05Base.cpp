#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include <SDL_mixer.h>
#include <time.h>
#include <iostream>


//Game general information

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 700


int main(int, char*[]) {

	bool StartPointed, StopPointed;
	bool GameOverPointed, RestartPointed;
	bool UpKey1 = false, 
		 DownKey1 = false, 
		 RightKey1 = false, 
		 LeftKey1 = false;
	bool UpKey2 = false,
		 DownKey2 = false,
		 RightKey2 = false,
		 LeftKey2 = false;
	bool RightClick = false,
		 LeftClick = false;
	bool isGold1 = true,
		 isGold2 = true,
	 	 isGold3 = true,
	 	 isGold4 = true,
	 	 isGold5 = true;
	int x,
		y;
	int puntuacio1 = 0,
		puntuacio2 = 0;
	int MovSpeed = 15,
		GoldSize = 50;
	int Map{ 0 };

	// --- INIT ---
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) throw "No es pot inicialitzar SDL subsystems";

	const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };
	if (!(IMG_Init(imgFlags) & imgFlags)) throw "Error: SDL_image init";

	if (TTF_Init() != 0) throw "No es pot inicialitzar SDL_ttf";

	const Uint8 mixFlags{ MIX_INIT_MP3 | MIX_INIT_OGG };
	if (!(Mix_Init(mixFlags) & mixFlags)) throw "Error: SDL_mixer init";

	// --- TIME ---
	clock_t lastTime = clock();
	float timeDown = 60;
	float deltaTime = 0;


	// --- WINDOW ---
	SDL_Window *window{ SDL_CreateWindow("SDL...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN) };
	if (window == nullptr) throw "No es pot inicialitzar SDL_Window";

	// --- RENDERER ---
	SDL_Renderer *renderer{ SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };
	if (renderer == nullptr) throw "No es pot inicialitzar SDL_Renderer";

	// --- SPRITES ---
		//BG WORLD 0
	SDL_Texture *bgTexture0{ IMG_LoadTexture(renderer, "../../res/img/bg.jpg") };
	if (bgTexture0 == nullptr) throw "No s'han pogut crear les textures World0";

		//BG WORLD 1
	SDL_Texture *bgTexture1{ IMG_LoadTexture(renderer, "../../res/img/bgCastle.jpg") };
	if (bgTexture1 == nullptr) throw "No s'han pogut crear les textures World1";

	SDL_Rect bgRect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };

	SDL_Rect playerTarget { 0,0,350,200 };




#define FPS 60
		// --- Animated Sprite ---
	int frameTime = 0;
	//Sprite1
	SDL_Texture *playerTexture1{ IMG_LoadTexture(renderer, "../../res/img/spCastle.png") };
	SDL_Rect playerRect1, playerPosition1;
	int textWidth, textHeight, frameWidth, frameHeight;
	SDL_QueryTexture(playerTexture1, NULL, NULL, &textWidth, &textHeight);
	frameWidth = textWidth / 12;
	frameHeight = textHeight / 8;
	playerPosition1.x = playerPosition1.y = 500;
	playerRect1.x = playerRect1.y = 0;
	playerPosition1.h = 75;
	playerPosition1.w = 75;
	playerRect1.h = frameHeight;
	playerRect1.w = frameWidth;

	//Sprite2
	SDL_Rect playerRect2, playerPosition2;
	SDL_QueryTexture(playerTexture1, NULL, NULL, &textWidth, &textHeight);
	frameWidth = textWidth / 12;
	frameHeight = textHeight / 8;
	playerRect2.x = frameWidth*3;
	playerRect2.y = 0;
	playerRect2.h = frameHeight;
	playerRect2.w = frameWidth;

	playerPosition2.x = 900;
	playerPosition2.y = 300;
	playerPosition2.h = 75;
	playerPosition2.w = 75;

/*	
	//PUNTUACIÓ
	SDL_Texture *puntTexture{ IMG_LoadTexture(renderer, "../../res/img/num.png") };
	SDL_Rect puntRect1, puntRect1Position, puntRect2;
	int numWidth, numHeight, numFrameWidth, numFrameHeight;
	SDL_QueryTexture(puntTexture, NULL, NULL, &numWidth, &numHeight);
	numFrameWidth = numWidth / 10;
	numFrameHeight = numHeight;
	puntRect1Position.x = puntRect1Position = 50;
*/

	//SpriteGold
	SDL_Texture *GoldTexture{ IMG_LoadTexture(renderer, "../../res/img/gold.png") };
	int GoldWidth, GoldHeight;
	SDL_Rect Gold1, Gold2, Gold3, Gold4, Gold5;
	SDL_QueryTexture(GoldTexture, NULL, NULL, &GoldWidth, &GoldHeight);
	Gold1.x = 100, Gold1.y = 500;
	Gold2.x = 900, Gold2.y = 600;
	Gold3.x = 450, Gold3.y = 450;
	Gold4.x = 800, Gold4.y = 300;
	Gold5.x = SCREEN_WIDTH / 2, Gold5.y = SCREEN_HEIGHT / 2;
	
	Gold1.w = Gold1.h = Gold2.w = Gold2.h = Gold3.w = Gold3.h = Gold4.w = Gold4.h = Gold5.w = Gold5.h = GoldSize;
	// --- TEXT ---
	
	//Start
	TTF_Font *font{ TTF_OpenFont("../../res/ttf/saiyan.ttf", 80) };
	if (font == nullptr) throw "No es pot inicialitzar the TTF_Font";
	SDL_Surface *tmpSurfStart{ TTF_RenderText_Blended(font, "START", SDL_Color{100, 255, 100, 255}) };
	if (tmpSurfStart == nullptr) TTF_CloseFont(font), throw "Unable to create the SDL text START";
	SDL_Texture *textTextureStart{ SDL_CreateTextureFromSurface(renderer, tmpSurfStart) };
	SDL_Rect textRect{ 500, 200, tmpSurfStart->w, tmpSurfStart->h };
	SDL_FreeSurface(tmpSurfStart);

	//Start2
	SDL_Surface *tmpSurfStart2{ TTF_RenderText_Blended(font, "START", SDL_Color{0, 255, 0, 255}) };
	if (tmpSurfStart == nullptr) TTF_CloseFont(font), throw "Unable to create the SDL text START2";
	SDL_Texture *textTextureStart2{ SDL_CreateTextureFromSurface(renderer, tmpSurfStart2) };

	//Stop
	if (font == nullptr) throw "No es pot inicialitzar the TTF_Font";
	SDL_Surface *tmpSurfStop{ TTF_RenderText_Blended(font, "STOP", SDL_Color{ 255, 100, 100, 255 }) };
	if (tmpSurfStop == nullptr) TTF_CloseFont(font), throw "Unable to create the SDL text STOP";
	SDL_Texture *textTextureStop{ SDL_CreateTextureFromSurface(renderer, tmpSurfStop) };
	SDL_Rect textRect2{ 510, 300, tmpSurfStop->w, tmpSurfStop->h };
	SDL_FreeSurface(tmpSurfStop);
	
	//Stop2
	SDL_Surface *tmpSurfStop2{ TTF_RenderText_Blended(font, "STOP", SDL_Color{ 255, 0, 0, 255 }) };
	if (tmpSurfStop2 == nullptr) TTF_CloseFont(font), throw "Unable to create the SDL text STOP2";
	SDL_Texture *textTextureStop2{ SDL_CreateTextureFromSurface(renderer, tmpSurfStop2) };
	

	//GAME OVER
	SDL_Surface *tmpEndGame{ TTF_RenderText_Blended(font, "GAME OVER", SDL_Color{ 0, 0, 255, 255}) };
	if (tmpEndGame == nullptr) TTF_CloseFont(font), throw "Unable to create the SDL text ENDGAME";
	SDL_Texture *textTextureEnd{ SDL_CreateTextureFromSurface(renderer, tmpEndGame) };
	SDL_Rect EndRect{ SCREEN_WIDTH/2 - (tmpEndGame->w/2), SCREEN_HEIGHT/2 - (tmpEndGame->h / 2), tmpEndGame->w, tmpEndGame->h };

	//RESTART 1
	SDL_Surface *tmpRestartGame1{ TTF_RenderText_Blended(font, "GAME OVER", SDL_Color{ 0, 255, 0, 255 }) };
	if (tmpRestartGame1 == nullptr) TTF_CloseFont(font), throw "Unable to create the SDL text RESTART";
	SDL_Texture *textTextureRestart1{ SDL_CreateTextureFromSurface(renderer, tmpRestartGame1) };
	SDL_Rect RestartRect{ SCREEN_WIDTH / 2 - (tmpRestartGame1->w / 2), SCREEN_HEIGHT / 2 - (tmpRestartGame1->h / 2), tmpRestartGame1->w, tmpRestartGame1->h };

	//RESTART 2
	SDL_Surface *tmpRestartGame2{ TTF_RenderText_Blended(font, "GAME OVER", SDL_Color{ 0, 255, 0, 255 }) };
	if (tmpRestartGame2 == nullptr) TTF_CloseFont(font), throw "Unable to create the SDL text RESTART";
	SDL_Texture *textTextureRestart2{ SDL_CreateTextureFromSurface(renderer, tmpRestartGame2) };
	


	
	TTF_CloseFont(font);
	
	// --- AUDIO ---

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		throw "Unable  to initializate SDL mixer audio systems";
	}
	Mix_Music * soundtrack{ Mix_LoadMUS("../../res/au/mainTheme.mp3") };
	if (!soundtrack) throw "Unable to load the Mix_Music soundtrack";
	Mix_VolumeMusic(MIX_MAX_VOLUME/2);
	//Mix_PlayMusic(soundtrack, -1);

	// --- GAME LOOP ---
	SDL_Event event;
	bool isRunning = true;
	while (isRunning){
		SDL_GetMouseState(&x, &y);
		StartPointed = false;
		StopPointed = false;
		// HANDLE EVENTS
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:		isRunning = false; break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false; 
				//Player 1
				else if (event.key.keysym.sym == SDLK_DOWN) {
					DownKey1 = true;
					UpKey1 = false;
					RightKey1 = false;
					LeftKey1 = false;
				}	
				else if (event.key.keysym.sym == SDLK_UP) {
					DownKey1 = false;
					UpKey1 = true;
					RightKey1 = false;
					LeftKey1 = false;
				}
				else if (event.key.keysym.sym == SDLK_RIGHT) {
					DownKey1 = false;
					UpKey1 = false;
					RightKey1 = true;
					LeftKey1 = false;
				}
				else if (event.key.keysym.sym == SDLK_LEFT) {
					DownKey1 = false;
					UpKey1 = false;
					RightKey1 = false;
					LeftKey1 = true;
				}
				//Player 2
				else if (event.key.keysym.sym == SDLK_s) {
					DownKey2 = true;
					UpKey2 = false;
					RightKey2 = false;
					LeftKey2 = false;
				}
				else if (event.key.keysym.sym == SDLK_w) {
					DownKey2 = false;
					UpKey2 = true;
					RightKey2 = false;
					LeftKey2 = false;
				}
				else if (event.key.keysym.sym == SDLK_d) {
					DownKey2 = false;
					UpKey2 = false;
					RightKey2 = true;
					LeftKey2 = false;
				}
				else if (event.key.keysym.sym == SDLK_a) {
					DownKey2 = false;
					UpKey2 = false;
					RightKey2 = false;
					LeftKey2 = true;
				}
				
				break;		
			case SDL_KEYUP:
				DownKey1 = false;
				UpKey1 = false;
				RightKey1 = false;
				LeftKey1 = false;

				DownKey2 = false;
				UpKey2 = false;
				RightKey2 = false;
				LeftKey2 = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					LeftClick = true;
				}
				if (event.button.button == SDL_BUTTON_RIGHT) {
					RightClick = true;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) {
					LeftClick = false;
				}
				if (event.button.button == SDL_BUTTON_RIGHT){
					RightClick = false;
				}
			default:;
			}
		}

		// UPDATE

		deltaTime = (clock() - lastTime);
		lastTime = clock();
		deltaTime /= CLOCKS_PER_SEC;
		timeDown -= deltaTime;
		//std::cout << timeDown<< std::endl;
		
			//PintarSprite
		frameTime++;
		if (FPS / frameTime <= 9) {
			frameTime = 0;
			//MOVEMENT
			//Player 1
			if (RightKey1 == true) {
				playerPosition1.x += MovSpeed;
				playerRect1.y = frameHeight*2;
				playerRect1.x += frameWidth;
					if (playerRect1.x >= frameHeight*3) {
						playerRect1.x = 0;
					}
			}
			else if (UpKey1 == true)
				{
				playerPosition1.y -= MovSpeed;
				playerRect1.y = frameHeight *3;
				playerRect1.x += frameWidth;
					if (playerRect1.x >= frameHeight * 3) {
						playerRect1.x = 0;
					}
				}
			else if (LeftKey1 == true)
			{
				playerPosition1.x -= MovSpeed;
				playerRect1.y = frameHeight;
				playerRect1.x += frameWidth;
					if (playerRect1.x >= frameHeight * 3) {
						playerRect1.x = 0;
					}
			}
			else if (DownKey1 == true)
			{
				playerPosition1.y += MovSpeed;
				playerRect1.y = 0;
				playerRect1.x += frameWidth;
					if (playerRect1.x >= frameHeight * 3) {
						playerRect1.x = 0;
					}
			}
			//Player 2
			if (RightKey2 == true) {
					playerPosition2.x += MovSpeed;
					playerRect2.y = frameHeight * 2;
					playerRect2.x += frameWidth;
					if (playerRect2.x >= frameHeight * 6) {
						playerRect2.x = frameWidth*4;
					}
				}
			else if (UpKey2 == true)
				{
					playerPosition2.y -= MovSpeed;
					playerRect2.y = frameHeight * 3;
					playerRect2.x += frameWidth;
					if (playerRect2.x >= frameHeight * 6) {
						playerRect2.x = frameWidth*4;
					}
				}
			else if (LeftKey2 == true)
				{
					playerPosition2.x -= MovSpeed;
					playerRect2.y = frameHeight;
					playerRect2.x += frameWidth;
					if (playerRect2.x >= frameHeight * 6) {
						playerRect2.x = frameWidth * 4;
					}
				}
			else if (DownKey2 == true)
				{
					playerPosition2.y += MovSpeed;
					playerRect2.y = 0;
					playerRect2.x += frameWidth;
					if (playerRect2.x >= frameHeight * 6) {
						playerRect2.x = frameWidth * 4;
					}
				}
		}

		// DRAW
		SDL_RenderClear(renderer);
		switch (Map)
		{
		case 0:
			//BG
			SDL_RenderCopy(renderer, bgTexture0, nullptr, &bgRect);

			//START BUTTON
			if (((x > textRect.x) && (x < textRect.x + textRect.w)) && ((y > textRect.y) && (y < textRect.y + textRect.h - 15))) {
				StartPointed = true;
				if (LeftClick == true) {
					Map = 1;
				}
			}
			else {
				StartPointed = false;
			}
			//STOP BUTTON
			if (((x > textRect2.x) && (x < textRect2.x + textRect2.w)) && ((y > textRect2.y) && (y < textRect2.y + textRect2.h - 15))) {
				StopPointed = true;
				if (LeftClick == true) {
					isRunning = false;
				}
			}
			else {
				StopPointed = false;
			}
			//HOVER
				//START
			if (StartPointed == true) {
				SDL_RenderCopy(renderer, textTextureStart, nullptr, &textRect);
			}
			else {
				SDL_RenderCopy(renderer, textTextureStart2, nullptr, &textRect);
			}

				//STOP
			if (StopPointed == true) {
				SDL_RenderCopy(renderer, textTextureStop, nullptr, &textRect2);
			}
			else {
				SDL_RenderCopy(renderer, textTextureStop2, nullptr, &textRect2);
			}

			break;

		case 1:
			//BG
			SDL_RenderCopy(renderer, bgTexture1, nullptr, &bgRect);

			//PLAYERS
			SDL_RenderCopy(renderer, playerTexture1, &playerRect1, &playerPosition1);
			SDL_RenderCopy(renderer, playerTexture1, &playerRect2, &playerPosition2);

			//GOLD
			if (isGold1)	SDL_RenderCopy(renderer, GoldTexture, nullptr, &Gold1);
			if (isGold2)	SDL_RenderCopy(renderer, GoldTexture, nullptr, &Gold2);
			if (isGold3)	SDL_RenderCopy(renderer, GoldTexture, nullptr, &Gold3);
			if (isGold4)	SDL_RenderCopy(renderer, GoldTexture, nullptr, &Gold4);
			if (isGold5)	SDL_RenderCopy(renderer, GoldTexture, nullptr, &Gold5);

			//HORIZON
				//PLAYER1
			if		(playerPosition1.y <= (SCREEN_HEIGHT/3)-30) {
				playerPosition1.y = (SCREEN_HEIGHT / 3) - 30;
			}
			else if (playerPosition1.x > SCREEN_WIDTH){
				playerPosition1.x = 0;
			}
			else if (playerPosition1.x < 0) {
				playerPosition1.x = SCREEN_WIDTH;
			}
			else if (playerPosition1.y > SCREEN_HEIGHT - playerPosition1.h) {
				playerPosition1.y = SCREEN_HEIGHT - playerPosition1.h;
			}

			//GOLD COLLIDER
			if ((Gold1.x < playerPosition1.x + playerPosition1.w) && (Gold1.x > playerPosition1.x) &&
				(Gold1.y > playerPosition1.y) && (Gold1.y <= playerPosition1.y + playerPosition1.h)){
				isGold1 = false;
			}
			else if ((Gold2.x < playerPosition1.x + playerPosition1.w) && (Gold2.x > playerPosition1.x) &&
				(Gold2.y > playerPosition1.y) && (Gold2.y <= playerPosition1.y + playerPosition1.h)) {
				isGold2 = false;
			}
			else if ((Gold3.x < playerPosition1.x + playerPosition1.w) && (Gold3.x > playerPosition1.x) &&
				(Gold3.y > playerPosition1.y) && (Gold3.y <= playerPosition1.y + playerPosition1.h)) {
				isGold3 = false;
			}
			else if ((Gold4.x < playerPosition1.x + playerPosition1.w) && (Gold4.x > playerPosition1.x) &&
				(Gold4.y > playerPosition1.y) && (Gold4.y <= playerPosition1.y + playerPosition1.h)) {
				isGold4 = false;
			}
			else if ((Gold5.x < playerPosition1.x + playerPosition1.w) && (Gold5.x > playerPosition1.x) &&
				(Gold5.y > playerPosition1.y) && (Gold5.y <= playerPosition1.y + playerPosition1.h)) {
				isGold5 = false;
			}

				//PLAYER2
			if		(playerPosition2.y <= (SCREEN_HEIGHT / 3) - 30) {
				playerPosition2.y = (SCREEN_HEIGHT / 3) - 30;
			}
			else if (playerPosition2.x > SCREEN_WIDTH) {
				playerPosition2.x = 0;
			}
			else if (playerPosition2.x < 0) {
				playerPosition2.x = SCREEN_WIDTH;
			}
			else if (playerPosition2.y > SCREEN_HEIGHT - playerPosition2.h) {
				playerPosition2.y = SCREEN_HEIGHT - playerPosition2.h;
			}

			//GOLD COLLIDER
			if ((Gold1.x < playerPosition2.x + playerPosition2.w) && (Gold1.x > playerPosition2.x) &&
				(Gold1.y > playerPosition2.y) && (Gold1.y <= playerPosition2.y + playerPosition2.h)) {
				isGold1 = false;
			}
			else if ((Gold2.x < playerPosition2.x + playerPosition2.w) && (Gold2.x > playerPosition2.x) &&
				(Gold2.y > playerPosition2.y) && (Gold2.y <= playerPosition2.y + playerPosition2.h)) {
				isGold2 = false;
			}
			else if ((Gold3.x < playerPosition2.x + playerPosition2.w) && (Gold3.x > playerPosition2.x) &&
				(Gold3.y > playerPosition2.y) && (Gold3.y <= playerPosition2.y + playerPosition2.h)) {
				isGold3 = false;
			}
			else if ((Gold4.x < playerPosition2.x + playerPosition2.w) && (Gold4.x > playerPosition2.x) &&
				(Gold4.y > playerPosition2.y) && (Gold4.y <= playerPosition2.y + playerPosition2.h)) {
				isGold4 = false;
			}
			else if ((Gold5.x < playerPosition2.x + playerPosition2.w) && (Gold5.x > playerPosition2.x) &&
				(Gold5.y > playerPosition2.y) && (Gold5.y <= playerPosition2.y + playerPosition2.h)) {
				isGold5 = false;
			}

		if ((isGold1 == false) && (isGold2 == false) && (isGold3 == false) && (isGold4 == false) && (isGold5 == false)) Map = 2;
		
		break;
		case 2:	
			//BG
			SDL_RenderCopy(renderer, bgTexture0, nullptr, &bgRect);
			
			if (RestartPointed == true) {
				SDL_RenderCopy(renderer, textTextureRestart2, nullptr, &RestartRect);
			}
			else {
				SDL_RenderCopy(renderer, textTextureRestart1, nullptr, &RestartRect);
			}

			if (GameOverPointed == true){
				
			}
			else {
				SDL_RenderCopy(renderer, textTextureEnd, nullptr, &EndRect);
			}
			
			

		//HOVER
			//RESTART BUTTON
			if (((x > RestartRect.x) && (x < RestartRect.x + RestartRect.w)) && ((y > RestartRect.y) && (y < RestartRect.y + RestartRect.h - 15))) {
				RestartPointed = true;
				if (LeftClick == true) {
					Map = 1;
				}
			}
			else {
				RestartPointed = false;
			}
			//GAME OVER BUTTON
			if (((x > EndRect.x) && (x < EndRect.x + EndRect.w)) && ((y > EndRect.y) && (y < EndRect.y + EndRect.h - 15))) {
				GameOverPointed = true;
				if (LeftClick == true) {
					isRunning = false;
				}
			}
			else {
				GameOverPointed = false;
			}
			


			break;
		default:
			break;
		}


		
		//FINAL RENDER
		SDL_RenderPresent(renderer);
		

	}

	// --- DESTROY ---
	SDL_DestroyTexture(bgTexture0);
	SDL_DestroyTexture(bgTexture1);
	SDL_DestroyTexture(textTextureStart);
	SDL_DestroyTexture(textTextureStart2);
	SDL_DestroyTexture(textTextureEnd);
	SDL_DestroyTexture(playerTexture1);
	SDL_DestroyRenderer(renderer);
	Mix_CloseAudio();
	SDL_DestroyWindow(window);

	// --- QUIT ---
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	return 0;
}