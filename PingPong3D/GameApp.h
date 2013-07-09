// GameApp.h - GameApp class declaration.
// (c) Nikolay Prodanov, Juelich, summer 2013.

#pragma once			// !Later replace by macro for portability between OS.
#include <string>
#include "Ball.h"
#include "Wall.h"
#include "Paddle.h"

class GameApp
{
public:
	GameApp(void);
	~GameApp(void);
	int play();

	// Private methods.
private:
	int initLibraries();	// Initialize: SDL, OGL, sound, timers, game params, rand numbers.
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

	// SDL-specific.
	int setupSDL();
	void handleKeyDown(const SDL_Event& sdle );
	void handleKeyUp(const SDL_Event& sdle );
	void handleResize(const SDL_Event& sdle);
	void handleMouseMotion(const SDL_Event& sdle);
	void handleMouseButtonDown(const SDL_Event& sdle);
	void handleMouseButtonUp(const SDL_Event& sdle);
	
	// OpenGl-specific.
	void setupRenderingContext();	// OGL initial state: depth, lights, materials, etc.
	void setupMatrices();	// Init transforms: modelview, projection and other matrices.
	void doDrawing();
	void drawAxes();
	void rotateView(float dx);

	int pickObject(int x, int y);		// Returns type of the object under cursor.

	// Calculate the angle used in the perspective, we use size as a unit of length.
	float calculateAngle(float size, double zAxisDistance)
	{
		double radtheta, degtheta;
		radtheta = 2.0 * std::atan2 (size/2.0, zAxisDistance);
		degtheta = (180.0 * radtheta) / atan(1.);
		return (degtheta);
	}

	// Data load.
	std::tr1::shared_ptr<TEXTURE> loadTexture(std::string fileName);// Factory function

	// Private members.
private:
	enum {BALL, WALL, LEFT_PADDLE};	// enum hack.

	// Game drawing related.
	float flScreenWidth;			// Screen width.
	float flScreenHeight;			// Screen height.
	float flAngleFrustum;
	std::string strGameName;
	float flZaxisDistance;	// Distance from which we view the scene
	float flLengthUnit;		// Unit of measurement of length.

	// View rotation with mouse.
	float xViewOld;			// Vars for rotate view.
	float xView;
	float angleView;		// angle to rotate the view.

	float xPaddleOld;
	float yPaddleOld;

	bool bPaddlePicked;

	const int MAXHITS;
	const int selectRegion;	// region for picking.

	// Box sizes.
	float flBoxWidth;		// Add these to the constructor and use when init walls.
	float flBoxHeight;
	float flBoxThickness;
	//vector_3d vLeftPaddleCntr;	// Init left paddle position.
	//vector_3d vRightPaddleCntr;
	// int leftWallIdx;
	// int rightWallIdx;
	int leftPaddleIdx;

	// A vector of pointers to Shapes. Ball is stored in the 1st item.	
	std::vector<std::tr1::shared_ptr<Shape> > shapes;// Smart pointers for memory management.

	// Data.
	// Textures.
	std::string strBallTexture;
	std::vector<std::tr1::shared_ptr<TEXTURE> > textures;

	// Game logic related.
	bool bRunning;			// True if the game is going on.
	bool bGameOver;			// True if the player has lost.
	float deltaTime;
	Uint32 lastMillisec;
	SDL_Event sdlEvent;
};