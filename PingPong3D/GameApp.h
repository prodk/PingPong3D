// GameApp.h - GameApp class declaration.
// (c) Nikolay Prodanov, Juelich, summer 2013.

#pragma once			// !Later replace by macro for portability between OS.
#include <string>
//#include "Ball.h"
//#include "Wall.h"
//#include "Paddle.h"
#include "SdlScreen.h"

class GameApp
{
public:
	GameApp(void);
	~GameApp(void);
	void manageGame();	

	// Private methods.
private:
	int initLibraries();	// Initialize: SDL, OGL, sound, timers, game params, rand numbers.
	void loadData();		// Load sounds, textures.
	void shutDown();		// Clean up resources.
	void setupSound();		// Init fmod library.
	void setupTimers();
	void updateTimers();
	void setupNewGame();	// Init game-specific variables/objects.
	//void doLogic();			// Manage the game depending on the inner state of the system.
	void swapBuffers();		// Swap double buffers.

	// SDL-specific.
	int setupSDL();

// Moved to SdlScreen:
	void doInput();			// Keyboard/mouse.
	void doDrawing();

	// moved to OptionsScreen:
	

	// moved to PlayScreen
	void doLogic();			// Manage the game depending on the inner state of the system.
	void play();			// Play the game.
	void addShapes();

	// I/O
	void handleKeyDown(const SDL_Event& sdle );
	void handleKeyUp(const SDL_Event& sdle );
	void handleResize(const SDL_Event& sdle);
	void handleMouseMotion(const SDL_Event& sdle);
	void handleMouseButtonDown(const SDL_Event& sdle);
	void handleMouseButtonUp(const SDL_Event& sdle);

	// OpenGl-specific.
	void drawAxes();
	void initView();		// Avoid code duplication in doDrawing() and pickObjects().
	void initResize();		// Avoid code dupl. in setupMatrices() and handleResize().
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

// End moved to screen.			
	
	// OpenGl-specific.
	void setupRenderingContext();	// OGL initial state: depth, lights, materials, etc.
	void setupMatrices();	// Init transforms: modelview, projection and other matrices.

	// Data load.
	std::tr1::shared_ptr<TEXTURE> loadTexture(std::string fileName);// Factory function

	//void draw2DTextureEx(float _x, float _y, float _z, float _alpha,  TEXTURE* _tex);

	// Private members.
private:

// Members moved to PlayScreen
	enum {BALL, WALL, LEFT_PADDLE};	// enum hack.

	// Game drawing related.
	float flScreenWidth;			// Screen width.
	float flScreenHeight;			// Screen height.
	float flAngleFrustum;
	float flZaxisDistance;	// Distance from which we view the scene
	float flLengthUnit;		// Unit of measurement of length.

	// View rotation with mouse.
	float xViewOld;			// Vars for rotate view.
	float xView;	//! Check whether this var is used in the code!
	float angleViewY;		// Angle to rotate the view around Y.
	float angleViewZ;		// Rotate view around X.
	float flScaleAll;

	float xPaddleOld;
	float yPaddleOld;

	bool bPaddlePicked;

	// Box sizes.
	float flBoxWidth;		// Add these to the constructor and use when init walls.
	float flBoxHeight;
	float flBoxThickness;
	//vector_3d vLeftPaddleCntr;	// Init left paddle position.
	//vector_3d vRightPaddleCntr;
	// int leftWallIdx;
	// int rightWallIdx;
	int leftPaddleIdx;

	SDL_Event sdlEvent;

	// A vector of pointers to Shapes. Ball is stored in the 1st item.	
	std::vector<std::tr1::shared_ptr<Shape> > shapes;// Smart pointers for memory management.
// End members moved to PlayScreen.

	std::string strGameName;
	SDL_Surface* surface;
	std::tr1::shared_ptr<OptionsScreen> optionsScreen;
	std::tr1::shared_ptr<PlayScreen> playScreen;

	// Game logic related.
	Logic logic;
	float deltaTime;
	Uint32 lastMillisec;
	//SDL_Event sdlEvent;

	// Data.
	// Textures.
	std::vector<std::tr1::shared_ptr<TEXTURE> > textures;
};