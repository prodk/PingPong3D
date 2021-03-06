// SdlScreen.h - declaration of the SdlScreen class and all its children.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#ifndef SDL_SCREEN
#define SDL_SCREEN

#include "GuiObject.h"
#include "Ball.h"
#include "Wall.h"
#include "Paddle.h"
#include <map>

typedef std::vector<std::tr1::shared_ptr<TEXTURE> > TEXTURE_PTR_ARRAY;

// Abstract base class for screens.
class SdlScreen : public Observer		// All screens are observers.
{
public:
	SdlScreen(int idExt, float w, float h, SDL_Surface*, TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
		FMOD::System *sys, std::vector<FMOD::Sound*> snd);
	virtual ~SdlScreen(void);

	// Public methods to override. Some of them have the default implementation.
	virtual void doInput(Logic &l, SDL_Event sdlEvent);	// Not pure virtual, Keyboard/mouse.
	virtual void doDrawing(Logic &logic) = 0;
	virtual void notify(Subject* s);

	// Class specific method with fixed implementation.
	void setScreenSize(float w, float h);

protected:
	int drawText(const std::string & txt, GLfloat x, GLfloat y, GLfloat w, GLfloat h,
		TTF_Font *font, Logic &logic, SDL_Color textColor);
	void drawBackgroundTexture(int id, float z);

	// Event handlers.
	virtual void handleKeyDown(const SDL_Event& sdle, Logic &l);
	virtual void handleKeyUp(const SDL_Event& sdle, Logic &l);
	virtual void handleResize(const SDL_Event& sdle, Logic &l);
	virtual void handleMouseMotion(const SDL_Event& sdle, Logic &l);
	virtual void handleMouseButtonDown(const SDL_Event& sdle, Logic &l);
	virtual void handleMouseButtonUp(const SDL_Event& sdle, Logic &l);

protected:
	enum{START_BTN, OPTIONS_BTN, HOWTO_BTN, TRAIN_BTN,			// Start screen buttons.
		 BACKGRSND_BTN, ACTIONSND_BTN, ROUND_BTN, BACK_BTN};	// Options screen buttons.

	int id;
	float flWidth;								// Screen width.
	float flHeight;								// Screen height.
	SDL_Surface* surfaceOnWhichWeRender;
	bool bLeftMouseButton;						// Whether left mouse button was pressed.
	SDL_Event sdlEvent;

	TEXTURE_PTR_ARRAY textures;
	FMOD::System *system;
	std::vector<FMOD::Sound*> sounds;
	FMOD::Channel *channel;
	TTF_Font** fonts;	
};

/*_________________________*/
// ButtonScreen.
class ButtonScreen : public SdlScreen
{
public:
	ButtonScreen(int idExt, float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
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
	int pressButton(float x, float y);	// Sets a button under cursor pressed and returns its id.
	void setUnpressed();				// Sets all the buttons unpressed.
	int pickButton(float px, float py);	// Returns the id of the button under the cursor.
	void processButton(std::size_t id, Logic & logic);	// Perform actions with logic depending on the button id.
	int getHighlightedButton();			// Gets the id of the highlighted button.

protected:
	std::map<std::size_t, std::tr1::shared_ptr<GuiObject> > guiObjects;
	bool bPlayButtonSound;
	bool bMouseOverButton;				// To prevent from several higlight messages.
	bool bKeyDown;
	int iPrevFocusButton;				// Previously highlighted button.
};

/*________________________________*/
// StartScreen is a ButtonScreen.
class StartScreen : public ButtonScreen
{
public:
	StartScreen(int idExt, float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
		FMOD::System *sys, std::vector<FMOD::Sound*> snd);
	~StartScreen(void);
	
private:
	void addButtons();
};

/*________________________________*/
// OptionsScreen is a ButtonScreen.
class OptionsScreen : public ButtonScreen
{
public:
	OptionsScreen(int idExt, float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
		FMOD::System *sys, std::vector<FMOD::Sound*> snd, Logic &logic);
	~OptionsScreen(void);

	void setupNewScreen(Logic &logic);
	
private:
	void addButtons(Logic & logic);
};

/*________________________________*/
// HowtoScreen is a ButtonScreen.
class HowtoScreen : public ButtonScreen
{
public:
	HowtoScreen(int idExt, float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
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
	PlayScreen(int idExt, float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt, 
		FMOD::System *sys, std::vector<FMOD::Sound*> snd, 
		RoundParamsVector & rp);
	~PlayScreen(void);

	// Overridden virtual functions.	
	void doInput(Logic &l, SDL_Event sdlEvent);			// Keyboard/mouse.
	void doDrawing(Logic &logic);
	void notify(Subject* s);

	// PlayScreen-specific.
	void play(Logic &l, SDL_Event sdlEvent);			// Play the game.

	// Private methods.
private:
	// PlayScreen-specific methods.
	void doLogic(Logic &l);
	void setupNewRound(Logic &logic);

	// Event handlers overridden.
	void handleKeyDown(const SDL_Event& sdle, Logic &l);
	void handleResize(const SDL_Event& sdle, Logic &l);
	void handleMouseMotion(const SDL_Event& sdle, Logic &l);
	void handleMouseButtonDown(const SDL_Event& sdle, Logic &l);
	void handleMouseButtonUp(const SDL_Event& sdle, Logic &l);

	// OpenGl-specific.
	void drawAxes() const;			// Currently not used, but exists just in case.
	void initView();				// Avoid code duplication in doDrawing() and pickObjects().
	void initResize();				// Avoid code dupl. in setupMatrices() and handleResize().
	void rotateView(float dx);
	int pickObject(int x, int y);	// Returns the type of the object under cursor.
	void initMembers(const Logic &l);
	void addShapes(Logic &l);
	void drawScoreAndRound(Logic &l);// Currently not used, as it is too computationally demanding.

	// Inline: calculate the angle used in the perspective.
	float calculateAngle(float size, double zAxisDistance) {
		double radtheta, degtheta;
		radtheta = 2.0 * std::atan2 (size/2.0, zAxisDistance);
		degtheta = (180.0 * radtheta) / atan(1.);
		return (degtheta);
	}

	// Optimized text output.
	std::tr1::shared_ptr<TEXTURE> createStringTexture(const std::string &txt, 
		TTF_Font *font, SDL_Color textColor);	// Create string texture only once.
	void drawStringTexture(int symbolIdx, float x, float y, float z, float w, float h, 
		float angley);
	void drawScoreAndRoundOptimized(Logic &logic);

	// Private members.
private:
	enum {BALL, WALL, LEFT_PADDLE, RIGHT_PADDLE};	// Enum hack for shape types.

	// A vector of pointers to Shapes. Ball is stored in the 0th item.	
	std::map<std::size_t, std::tr1::shared_ptr<Shape> > shapes;// Smart pointers for memory management.

	// Game drawing related.
	float flAngleFrustum;
	float flZaxisDistance;			// Distance from which we view the scene
	float flLengthUnit;				// Unit of measurement of length. The smaller - the closer we see the scene.

	// View rotation with mouse.
	float xViewOld;					// Vars for rotate view.
	float angleViewY;				// Angle to rotate the view around Y.
	float angleViewZ;				// Rotate view around Z.
	float flScaleAll;				// Zooming factor.
	float angleViewYMax;			// Initial rotation before a round starts.
	float deltaAngleY;				// Increments of the initial rotation.
	float xPaddleOld;				// Previous positions of a paddle.
	float yPaddleOld;

	bool bPaddlePicked;				// True if paddle is picked by the mouse.
	RoundParamsVector roundParams;
	std::size_t iCurRound;
	bool bPlaySound;

	// Sizes/other parameters.
	float flBoxWidth;
	float flBoxHeight;
	float flBoxThickness;
	float flPaddleRadius;
	float flBallVel;
	float flBallDeltaVel;			// Increment of the Ball's velocity after reflection.
	float flCompPaddleVel;
	int leftPaddleIdx;
	int rightPaddleIdx;
	int userWallIdx;
	int compWallIdx;
	float flScaleMax;				// Limit scaling the scene.
	float flScaleMin;

	float flDeltaAngleViewZ;
	float flDeltaScale;				// Scaling factor increment for keyboard handling.
	float flDeltaPaddle;			// Move paddle by this amount using keyboard.

	int iShowMessageCount;			// How many frames the msg "You/Comp lost!" is shown.
	int iMaxShowMessageCount;			// How many frames the msg "You/Comp lost!" is shown.

	// Members for optimized text output.
	int iNumOfStringTextures;
	// Enum hack - contains all the symbols we want to render on the scene.	
	enum{ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE,
		R, C, U, YOU_LOST, COMP_LOST};
	// String texture array created in constructor for optimized string output.
	std::vector<std::tr1::shared_ptr<TEXTURE> > stringTextures;
	void loadStringTextures();
};

#endif // SDL_SCREEN