// GameApp.h - GameApp class declaration.

#pragma once			// Replace by macro for portability between OS.
#include <string>

class GameApp
{
public:
	GameApp(void);
	~GameApp(void);
	int play();

	// Private methods.
private:
	void initLibraries();	// Initialize: SDL, OGL, sound, timers, game params, rand number seed.
	void loadData();		// Load sounds, textures.
	void shutDown();		// Clean up resources.
	void setupSound();		// Init fmod library.
	void setupTimers();
	void updateTimers();
	void setupNewGame();	// Init game-specific variables/objects.
	void doLogic();			// Manage the game depending on the inner state of the system.
	void swapBuffers();		// Swap double buffers.
	void doInput();			// Keyboard/mouse.
	
	// OpenGl-specific.
	void setupRenderingContext();	// OGL initial state: depth, lights, materials, etc.
	void setupMatrices();	// Transforms: modelview, projection matrices.
	void doDrawing();

	// Private members.
private:
	float flWidth;			// Screen width.
	float flHeight;			// Screen height.
	std::string strGameName;
	bool bRunning;			// True if the game is going on.
	bool bGameOver;

	float deltaTime;
	Uint32 lastMillisec;
	SDL_Event sdlEvent;
};