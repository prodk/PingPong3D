// SdlScreen.h - declaration of the SdlScreen class and all its children.
// (c) Nikolay Prodanov, Juelich, summer 2013.

#ifndef SDL_SCREEN		// Replace by #define for portability.
#define SDL_SCREEN

#include "GuiObject.h"
#include "Ball.h"
#include "Wall.h"
#include "Paddle.h"
#include <map>

typedef std::vector<std::tr1::shared_ptr<TEXTURE> > TEXTURE_PTR_ARRAY;

// Abstract base class for screens.
class SdlScreen : public Observer
{
public:
	SdlScreen(float w, float h, SDL_Surface*, TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
		FMOD::System *sys, std::vector<FMOD::Sound*> snd);
	virtual ~SdlScreen(void);

	// Public methods to override. Inherit interfaces with the default implementation.
	virtual void doInput(Logic &l, SDL_Event sdlEvent);	// Not pure virtual, Keyboard/mouse.
	virtual void doDrawing(Logic &logic) = 0;
	virtual void notify(Subject* s);

	void setScreenSize(float w, float h);

protected:
	int drawText(const std::string & txt, GLfloat x, GLfloat y, GLfloat w, GLfloat h,
		TTF_Font *font, Logic &logic);

	virtual void handleKeyDown(const SDL_Event& sdle, Logic &l);
	virtual void handleKeyUp(const SDL_Event& sdle, Logic &l);
	virtual void handleResize(const SDL_Event& sdle, Logic &l);
	virtual void handleMouseMotion(const SDL_Event& sdle, Logic &l);
	virtual void handleMouseButtonDown(const SDL_Event& sdle, Logic &l);
	virtual void handleMouseButtonUp(const SDL_Event& sdle, Logic &l);

protected:
	enum{START_BTN, OPTIONS_BTN, HOWTO_BTN, TRAIN_BTN,			// Start screen buttons.
		 BACKGRSND_BTN, ACTIONSND_BTN, ROUND_BTN, BACK_BTN};	// Options screen buttons.

	float flWidth;								// Screen width.
	float flHeight;								// Screen height.
	SDL_Surface* surfaceOnWhichWeRender;
	bool bLeftMouseButton;						// Whether left mouse button was pressed.

	TEXTURE_PTR_ARRAY textures;
	FMOD::System *system;
	std::vector<FMOD::Sound*> sounds;
	FMOD::Channel *channel;
	TTF_Font** fonts;

	SDL_Event sdlEvent;
};

/*_________________________*/
// ButtonScreen
class ButtonScreen : public SdlScreen
{
public:
	ButtonScreen(float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
		FMOD::System *sys, std::vector<FMOD::Sound*> snd);
	virtual ~ButtonScreen();
	virtual void doDrawing(Logic &logic);
	virtual void notify(Subject* s);

protected:
	typedef std::map<std::size_t, std::tr1::shared_ptr<GuiObject> >::iterator map_iter;
	virtual void handleMouseButtonDown(const SDL_Event& sdle, Logic &l);
	virtual void handleMouseButtonUp(const SDL_Event& sdle, Logic &l);
	virtual void handleMouseMotion(const SDL_Event& sdle, Logic &logic);
	virtual void handleKeyDown(const SDL_Event& sdle, Logic &logic);
	virtual void handleKeyUp(const SDL_Event& sdle, Logic &logic);

	// Button management.
	int pressButton(float x, float y);		// Reconsider these names!
	void setUnpressed();
	int pickButton(float px, float py);
	void processButton(std::size_t id, Logic & logic);
	int getHighlightedButton();

protected:
	std::map<std::size_t, std::tr1::shared_ptr<GuiObject> > guiObjects;
	bool bPlayButtonSound;
	bool bMouseOverButton;					// To prevent from several higlight messages.
	bool bKeyDown;
	int iPrevFocusButton;
};

/*________________________________*/
//    StartScreen.
class StartScreen : public ButtonScreen
{
public:
	StartScreen(float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
		FMOD::System *sys, std::vector<FMOD::Sound*> snd);
	~StartScreen(void);
	
private:
	void addButtons();
};

/*________________________________*/
// OptionsScreen declaration.
class OptionsScreen : public ButtonScreen
{
public:
	OptionsScreen(float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
		FMOD::System *sys, std::vector<FMOD::Sound*> snd, Logic &logic);
	~OptionsScreen(void);
	
private:
	void addButtons(Logic & logic);
};

/*________________________________*/
// HowtoScreen declaration.
class HowtoScreen : public ButtonScreen
{
public:
	HowtoScreen(float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
		FMOD::System *sys, std::vector<FMOD::Sound*> snd);
	~HowtoScreen(void);

	// Overridden virtual functions.
	void doDrawing(Logic &logic);
	
private:
	void handleMouseButtonUp(const SDL_Event& sdle, Logic &l);
	void handleKeyDown(const SDL_Event& sdle, Logic &l);
};

/*________________________________*/
// PlayScreen declaration.
class PlayScreen : public SdlScreen
{
public:
	PlayScreen(float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt, 
		FMOD::System *sys, std::vector<FMOD::Sound*> snd, 
		RoundParamsVector & rp);
	~PlayScreen(void);

	// Overridden virtual functions.
	void play(Logic &l, SDL_Event sdlEvent);			// Play the game.
	void doInput(Logic &l, SDL_Event sdlEvent);			// Keyboard/mouse.
	void doDrawing(Logic &logic);
	void notify(Subject* s);

	// Private methods.
private:
	// PlayScreen-specific methods.
	void doLogic(Logic &l);
	void setupNewRound(Logic &logic);

	// I/O.
	void handleKeyDown(const SDL_Event& sdle, Logic &l);
	void handleResize(const SDL_Event& sdle, Logic &l);
	void handleMouseMotion(const SDL_Event& sdle, Logic &l);
	void handleMouseButtonDown(const SDL_Event& sdle, Logic &l);
	void handleMouseButtonUp(const SDL_Event& sdle, Logic &l);

	// OpenGl-specific.
	//void setScreenSize(float w, float h);
	void drawAxes() const;
	void initView();			// Avoid code duplication in doDrawing() and pickObjects().
	void initResize();			// Avoid code dupl. in setupMatrices() and handleResize().
	void rotateView(float dx);
	int pickObject(int x, int y);		// Returns type of the object under cursor.
	//void unregisterObservers(Logic &logic);
	//void registerObservers(Logic &logic);
	void initMembers(const Logic &l);
	void addShapes(Logic &l);

	// Calculate the angle used in the perspective, we use size as a unit of length.
	float calculateAngle(float size, double zAxisDistance)
	{
		double radtheta, degtheta;
		radtheta = 2.0 * std::atan2 (size/2.0, zAxisDistance);
		degtheta = (180.0 * radtheta) / atan(1.);
		return (degtheta);
	}

	// Private members.
private:
	enum {BALL, WALL, LEFT_PADDLE, RIGHT_PADDLE};	// enum hack.

	// A vector of pointers to Shapes. Ball is stored in the 1st item.	
	std::map<std::size_t, std::tr1::shared_ptr<Shape> > shapes;// Smart pointers for memory management.

	// Game drawing related.
	float flAngleFrustum;
	float flZaxisDistance;			// Distance from which we view the scene
	float flLengthUnit;				// Unit of measurement of length.

	// View rotation with mouse.
	float xViewOld;			// Vars for rotate view.
	float angleViewY;		// Angle to rotate the view around Y.
	float angleViewZ;		// Rotate view around X.
	float flScaleAll;

	float xPaddleOld;
	float yPaddleOld;

	bool bPaddlePicked;
	RoundParamsVector roundParams;
	std::size_t iCurRound;
	bool bPlaySound;

	// Box sizes.
	float flBoxWidth;		// Add these to the constructor and use when init walls.
	float flBoxHeight;
	float flBoxThickness;
	float flPaddleRadius;
	float flBallVel;
	float flBallDeltaVel;
	float flCompPaddleVel;
	int leftPaddleIdx;
	int rightPaddleIdx;
	int userWallIdx;
	int compWallIdx;
	float flScaleMax;
	float flScaleMin;
};

#endif // SDL_SCREEN