// GameApp.h - GameApp class declaration.
// (c) Nikolay Prodanov, Juelich, summer 2013.

#pragma once			// !Later replace by macro for portability between OS.
#include <string>
#include "Ball.h"

class GameApp
{
public:
	GameApp(void);
	~GameApp(void);
	int play();

	// Private methods.
private:
	void initLibraries();	// Initialize: SDL, OGL, sound, timers, game params, rand numbers.
	void loadData();		// Load sounds, textures.
	void shutDown();		// Clean up resources.
	void setupSound();		// Init fmod library.
	void setupTimers();
	void updateTimers();
	void setupNewGame();	// Init game-specific variables/objects.
	void doLogic();			// Manage the game depending on the inner state of the system.
	void swapBuffers();		// Swap double buffers.
	void doInput();			// Keyboard/mouse.

	void addShapes();
	
	// OpenGl-specific.
	void setupRenderingContext();	// OGL initial state: depth, lights, materials, etc.
	void setupMatrices();	// Init transforms: modelview, projection and other matrices.
	void doDrawing();
	void drawAxes();

	// Private members.
private:
	enum {BALL};
	// Game drawing related.
	float flWidth;			// Screen width.
	float flHeight;			// Screen height.
	std::string strGameName;

	// A vecor of pointers to Shapes. Ball is stored in 1st item.
	// Use smart pointers for memory management.
	std::vector<std::tr1::shared_ptr<Shape> > shapes;
	//std::tr1::shared_ptr<Ball> ball;

	// Game logic related.
	bool bRunning;			// True if the game is going on.
	bool bGameOver;			// True if the player has lost.

	float deltaTime;
	Uint32 lastMillisec;
	SDL_Event sdlEvent;
};