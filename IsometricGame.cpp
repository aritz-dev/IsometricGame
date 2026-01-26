#include <SDL3/SDL.h>

using namespace std;

int main()
{
	//====================================
	// INIT
	//====================================
	//1. Set variables (Game State, Player Position, etc.)
	bool isRunning = true;
	SDL_FRect rect = { 0,0,100,100 };
	float speed = 0.1f;
	int directionX = 0;
	int directionY = 0;

	int windowWidth = 1000;
	int windowHeight = 1000;

	//2. Initialize SDL
	SDL_Init(SDL_INIT_VIDEO);

	//3. Create Window and Renderer
	SDL_Window* window = SDL_CreateWindow("My game", windowWidth, windowHeight,  SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer= SDL_CreateRenderer(window, NULL);
	//4. Load Assets (Textures, Fonts, etc.)
	
	

	//====================================
	// LOOP
	//====================================
	while (isRunning) {
		//1. Handle Events (Input)
		SDL_Event event;
		while (SDL_PollEvent(&event)) {  
			switch (event.type) {
				case SDL_EVENT_QUIT:
					isRunning = false;
					break;
			}
		}
		//2. handle state (Input)
		const bool* key_states = SDL_GetKeyboardState(NULL);
		directionX = 0;
		directionY = 0;
		if (key_states[SDL_SCANCODE_D]) directionX += 1; 
		if (key_states[SDL_SCANCODE_A]) directionX -= 1;
		if (key_states[SDL_SCANCODE_W]) directionY -= 1;
		if (key_states[SDL_SCANCODE_S]) directionY += 1;
		
		if (rect.x < 0) rect.x = 0;
		if (rect.x + rect.w > windowWidth) rect.x = windowWidth - rect.w;

		if (rect.y < 0) rect.y = 0;
		if (rect.y + rect.h > windowHeight) rect.y = windowHeight - rect.h;


		//3. Update Game State (Logic)
		rect.x += speed * directionX;
		rect.y += speed * directionY;

		
		//4. Render (Draw)
		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		SDL_RenderFillRect(renderer, &rect);

		SDL_RenderPresent(renderer);
	}

	//====================================
	// CLEANUP
	//====================================
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
