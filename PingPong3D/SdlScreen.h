#pragma once

#include "GuiObject.h"
#include "Ball.h"
#include "Wall.h"
#include "Paddle.h"

typedef std::vector<std::tr1::shared_ptr<TEXTURE> > TEXTURE_PTR_ARRAY;

// Abstract base class for screens.
class SdlScreen
{
public:
	SdlScreen(float w, float h, SDL_Surface*);
	virtual ~SdlScreen(void) = 0;

	// Public methods which should be overriden. Inherit interfaces with the default implementation.
	virtual void doInput(Logic &l, SDL_Event sdlEvent);			// Keyboard/mouse.
	virtual void doDrawing(Logic &logic);

	// Public methods that should not be overriden.

protected:
	float flWidth;			// Screen width.
	float flHeight;			// Screen height.
	SDL_Surface* surfaceOnWhichWeRender;
	SDL_Event sdlEvent;
};


//________________________________
// OptionsScreen declaration.
class OptionsScreen : public SdlScreen
{
public:
	OptionsScreen(float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt);
	~OptionsScreen(void);

	// Overridden virtual functions.
	void doInput(Logic &l, SDL_Event sdlEvent);			// Keyboard/mouse.
	void doDrawing(Logic &logic);

	// OptionsScreen-specific methods.
	
private:
	void handleMouseButtonDown(const SDL_Event& sdle, Logic &l);
	void handleMouseButtonUp(const SDL_Event& sdle, Logic &l);
	void handleKeyDown(const SDL_Event& sdle, Logic &l);
	void addButtons();

private:
	enum{START_BUTTON, OPTIONS_BUTTON};
	TEXTURE_PTR_ARRAY textures;
	std::vector<std::tr1::shared_ptr<GuiObject> > guiObjects;
	TTF_Font** fonts;
	bool bLeftMouseButton;		// Prevent buttons captions from flickering!
};

//________________________________
// PlayScreen declaration.
class PlayScreen : public SdlScreen
{
public:
	PlayScreen(float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, 
		FMOD::System *sys, std::vector<FMOD::Sound*> snd);
	~PlayScreen(void);

	// Overridden virtual functions.
	void doInput(Logic &l, SDL_Event sdlEvent);			// Keyboard/mouse.
	void doDrawing(Logic &logic);

	// PlayScreen-specific methods.
	void doLogic(const Logic &l);
	void play(Logic &l, SDL_Event sdlEvent);			// Play the game.
	void addShapes();

	// I/O
	void handleKeyDown(const SDL_Event& sdle, Logic &l);
	void handleKeyUp(const SDL_Event& sdle, Logic &l);
	void handleResize(const SDL_Event& sdle, Logic &l);
	void handleMouseMotion(const SDL_Event& sdle, Logic &l);
	void handleMouseButtonDown(const SDL_Event& sdle, Logic &l);
	void handleMouseButtonUp(const SDL_Event& sdle, Logic &l);

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

	// Private methods.
private:
	void initMembers();


	// Public members.
public:
	// A vector of pointers to Shapes. Ball is stored in the 1st item.	
	std::vector<std::tr1::shared_ptr<Shape> > shapes;// Smart pointers for memory management.
	// Reconsider sound:
	FMOD::System *system;
	std::vector<FMOD::Sound*> sounds;

private:
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

	//SDL_Event sdlEvent;

	TEXTURE_PTR_ARRAY textures;
};