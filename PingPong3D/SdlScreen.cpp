// SdlScreen.cpp - implementation of the SdlScreen class and all its child classes.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#include "SdlScreen.h"

/*________________________________*/
//  SdlScreen implementation.
SdlScreen::SdlScreen(int idExt, float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
		FMOD::System *sys, std::vector<FMOD::Sound*> snd) :
		id(idExt), flWidth(w), flHeight(h), surfaceOnWhichWeRender(s),
		textures(t), fonts(fnt), system(sys), sounds(snd)
{
	 bLeftMouseButton = false;
}

SdlScreen::~SdlScreen(void)
{
}

void SdlScreen::setScreenSize(float w, float h)
{
	flWidth = w;
	flHeight = h;
}

int SdlScreen::drawText(const std::string &txt, GLfloat x, GLfloat y, GLfloat w, GLfloat h,
	TTF_Font *font, Logic &logic, SDL_Color textColor)
{
	int mode;
	SDL_Surface *text;
	GLuint textTexture;
	SDL_Color bg;

	bg.r =0 ;
	bg.g = 0;
	bg.b = 0;
	
	text = TTF_RenderText_Blended(font, txt.c_str(), textColor);

	if (text->format->BytesPerPixel == 3) {		// RGB 24bit.
		mode = GL_RGB;
    } 
    else if (text->format->BytesPerPixel == 4) {// RGBA 32bit. 
        mode = GL_RGBA;
    } 
    else {
		std::cerr << "Could not determine pixel format of the surface." << std::endl;
        SDL_FreeSurface(text);
        exit(1);
    }

	// Draw the surface onto the OGL screen using textures.
    glPushMatrix();
	glDisable(GL_LIGHTING);

    glGenTextures(1, &textTexture);
    glBindTexture(GL_TEXTURE_2D, textTexture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create texture bound to our text surface.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text->w, text->h, 
		0, mode, GL_UNSIGNED_BYTE, text->pixels );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTranslatef(x, y, 0.0f);

	// Draw.
    glBegin( GL_QUADS );
	glTexCoord3d(0.0, 1.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord3d(1.0, 1.0, 0.0); glVertex3d(w, 0.0, 0.0);
	glTexCoord3d(1.0, 0.0, 0.0); glVertex3d(w, h, 0.0);
	glTexCoord3d(0.0, 0.0, 0.0); glVertex3d(0.0, h, 0.0);	
	glEnd();
	
	glEnable(GL_LIGHTING);

	glPopMatrix();

    SDL_FreeSurface(text);
    glDeleteTextures(1, &textTexture);

	return 0;
}

void SdlScreen::drawBackgroundTexture(int id, float z)
{
	glPushMatrix();
	glMatrixMode (GL_PROJECTION);	// Switch to the projection matrix.
	glLoadIdentity ();				// Clean up the projection matrix.

	glMatrixMode (GL_MODELVIEW);	// Switch to the modelview matrix.
	glLoadIdentity ();				// Clean up the modelview matrix.

	glDisable(GL_LIGHTING);
	glColor3f(1., 1., 1.);			// White light.
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textures[id]->id);
	
	glTranslatef(-1., -1., z);		// The coordinate of the bottom left corner of the OGL.
	glBegin( GL_QUADS );
	glTexCoord3d(0.0, 1.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord3d(1.0, 1.0, 0.0); glVertex3d(2., 0.0, 0.0);
	glTexCoord3d(1.0, 0.0, 0.0); glVertex3d(2., 2., 0.0);
	glTexCoord3d(0.0, 0.0, 0.0); glVertex3d(0.0, 2., 0.0);	
	glEnd();

	glDisable( GL_TEXTURE_2D );
	glEnable(GL_LIGHTING);

	glPopMatrix();
}

void SdlScreen::doInput(Logic &logic, SDL_Event sdlEvent)
{    
    switch(sdlEvent.type)
    {
	case SDL_MOUSEBUTTONDOWN:
		bLeftMouseButton = true;	// Prevent button captions from flickering.
		handleMouseButtonDown(sdlEvent, logic);
		break;

	case SDL_MOUSEBUTTONUP:
		if(bLeftMouseButton){		// Prevent button captions from flickering.
			bLeftMouseButton = false;
			handleMouseButtonUp(sdlEvent, logic);	
		}
		break;

	case SDL_MOUSEMOTION:
		handleMouseMotion(sdlEvent, logic);
		break;
	
	case SDL_KEYDOWN:
		handleKeyDown(sdlEvent, logic);
		break;

	case SDL_KEYUP:
		handleKeyUp(sdlEvent, logic);
		break;

	case SDL_VIDEORESIZE:
		handleResize(sdlEvent, logic);
		break;			

	case SDL_QUIT:
		logic.bAppRunning = false;
		break;
	}
}

void SdlScreen::handleKeyDown(const SDL_Event& sdle, Logic &l)
{
}

void SdlScreen::handleKeyUp(const SDL_Event& sdle, Logic &l)
{
}

void SdlScreen::handleResize(const SDL_Event& sdle, Logic &l)
{
	glViewport (0, 0, (GLsizei) sdle.resize.w, (GLsizei) sdle.resize.h);
	flWidth = sdle.resize.w;
	flHeight = sdle.resize.h;
	l.flScreenWidth = flWidth;
	l.flScreenHeight = flHeight;
}

void SdlScreen::handleMouseMotion(const SDL_Event& sdle, Logic &l)
{
}

void SdlScreen::handleMouseButtonDown(const SDL_Event& sdle, Logic &l)
{
}

void SdlScreen::handleMouseButtonUp(const SDL_Event& sdle, Logic &l)
{
}

void SdlScreen::notify(Subject* s)
{
}

/*________________________________*/
// ButtonScreen implementation.
ButtonScreen::ButtonScreen(int idExt, float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
		FMOD::System *sys, std::vector<FMOD::Sound*> snd) :
		SdlScreen(idExt, w, h, s, t, fnt, sys, snd)
{
	bPlayButtonSound = true;
	bMouseOverButton = false;
	bKeyDown = false;
	iPrevFocusButton = -1;
}

ButtonScreen::~ButtonScreen()
{
}

// Press the button under cursor.
int ButtonScreen::pressButton(float x, float y)
{	
	for(map_iter iterator = guiObjects.begin(); iterator != guiObjects.end(); iterator++){
		if( iterator->second->isPressed(x, y) ){
			iterator->second->setPressed(true);
			return iterator->second->getId();
		}
	}

	return -1;
}

// Unpress all the buttons.
void ButtonScreen::setUnpressed()
{	
	for(map_iter iterator = guiObjects.begin(); iterator != guiObjects.end(); iterator++)
		iterator->second->setPressed(false);	
}

// Get the id of the highlighted button.
int ButtonScreen::getHighlightedButton()
{	
	for(map_iter iterator = guiObjects.begin(); iterator != guiObjects.end(); iterator++){
		if( iterator->second->hasFocus() )
			return iterator->second->getId();		
	}

	return -1;
}

// Get the id of the button under cursor.
int ButtonScreen::pickButton(float px, float py)
{	
	for(map_iter iterator = guiObjects.begin(); iterator != guiObjects.end(); iterator++){
		if( iterator->second->isPressed(px, py) ){			
			return iterator->second->getId();
		}
	}

	return -1;
}

// Change logic structure depending on the id parameter of the button.
void ButtonScreen::processButton(std::size_t id, Logic & logic)
{
	switch(id)
	{ 
	case START_BTN:
		logic.bShowStartScreen = false;
		logic.bShowOptionsScreen = false;
		logic.bShowPlayScreen = true;		// Start the game.
		logic.bGamePaused = false;
		logic.bTrain = false;
		logic.notifyObservers();		// Tell registered observers to change their settings.
		break;

	case OPTIONS_BTN:
		logic.bShowStartScreen = false;
		logic.bShowOptionsScreen = true;
		logic.bShowPlayScreen = false;
		logic.bNewOptionsScreen = true;	// Create new buttons with the correct settings.
		break;

	case HOWTO_BTN:
		logic.bShowStartScreen = false;
		logic.bShowOptionsScreen = false;
		logic.bShowHowtoScreen = true;
		break;

	case TRAIN_BTN:
		logic.bShowStartScreen = false;
		logic.bShowOptionsScreen = false;
		logic.bShowPlayScreen = true;
		logic.bGamePaused = false;
		logic.bTrain = true;
		logic.notifyObservers();
		break;

	case BACKGRSND_BTN:					// Switch the bacground sound On/Off.
		if(logic.bBackgroundSound){
			logic.bBackgroundSound = false;
			((OptionsButton*)guiObjects[id].get())->setCaption(std::string("Off"));
		}
		else{
			logic.bBackgroundSound = true;
			((OptionsButton*)guiObjects[id].get())->setCaption(std::string("On"));
		}
		logic.notifyObservers(); // Tell registered observers to change their settings.
		break;

	case ACTIONSND_BTN:
		if(logic.bActionsSound){
			logic.bActionsSound = false;
			((OptionsButton*)guiObjects[id].get())->setCaption(std::string("Off"));
		}
		else{
			logic.bActionsSound = true;
			((OptionsButton*)guiObjects[id].get())->setCaption(std::string("On"));
		}
		logic.notifyObservers(); // Tell registered observers to change their settings.
		break;

	case ROUND_BTN:
		logic.iRound = logic.iRound % logic.iRoundMax + 1;
		((OptionsButton*)guiObjects[id].get())->setCaption(std::to_string((_ULonglong)logic.iRound));
		logic.notifyObservers(); 
		break;

	case BACK_BTN:
		logic.bShowOptionsScreen = false;
		logic.bShowStartScreen = true;
		break;
	}// End switch(id).

	if(bPlayButtonSound)
		::playSound(system, sounds[4], channel);
}

void ButtonScreen::doDrawing(Logic &logic)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffer for new data.

	glPushMatrix();					// Save current matrix.

	glMatrixMode (GL_PROJECTION);	// Switch to the projection matrix.
	glLoadIdentity ();				// Clean up the projection matrix.

	glMatrixMode (GL_MODELVIEW);	// Switch to the modelview matrix.
	glLoadIdentity ();				// Clean up the modelview matrix.

	// Draw the background image.
	drawBackgroundTexture(id, 0.);

	// Draw buttons.
	glEnable(GL_TEXTURE_2D);
	
	for(map_iter iterator = guiObjects.begin(); iterator != guiObjects.end(); iterator++)
		iterator->second->draw(fonts[0]);

	// Draw text at the top of the screen.
	std::string txt = "Ping Pong 3D";
	SDL_Color textColor;
	textColor.b = textColor.g = textColor.r = 0.;
	drawText(txt, -0.2, 0.8, 0.4, 0.15, fonts[0], logic, textColor);

	txt = "(c) Nikolay Prodanov, 2013. All rights reserved.";
	drawText(txt, -0.5, 0.6, 1., 0.11, fonts[0], logic, textColor);

	glDisable( GL_TEXTURE_2D );

	glFlush();	

    glPopMatrix();					// Restore the previous state.
}

void ButtonScreen::handleMouseButtonDown(const SDL_Event& sdle, Logic &logic)
{
	switch(sdle.button.button)
	{
	case SDL_BUTTON_LEFT:
		{
			// Normalize screen coordinates.
			// OpenGL coords (used for rendering) go from -1 to 1 with the origin in the middle of the screen.
			// Clicking coords have (0, 0) in the upper left corner.
			float xnorm = (sdle.button.x - 0.5*flWidth) / (0.5*flWidth);
			float ynorm = (0.5*flHeight - sdle.button.y) / (0.5*flHeight);
			
			pressButton(xnorm, ynorm);
		}
		break;
	}// End switch.
}

void ButtonScreen::handleMouseButtonUp(const SDL_Event& sdle, Logic &logic)
{
	switch(sdle.button.button)
	{
	case SDL_BUTTON_LEFT:
		{
			// Normalize screen coordinates.
			// OpenGL coords (used for rendering) go from -1 to 1 with the origin in the middle.
			// Clicking coords have (0, 0) in the upper left corner.
			float xnorm = (sdle.button.x - 0.5*flWidth) / (0.5*flWidth);
			float ynorm = (0.5*flHeight - sdle.button.y) / (0.5*flHeight);
			
			int btnId = pickButton(xnorm, ynorm);
			if(btnId >= 0){
				guiObjects[btnId]->setPressed(false);	// Unpress button.
				processButton(btnId, logic);			// Perform actions depending on btn id.
			}
		}
		break;
	}// End switch.
	setUnpressed();
}

void ButtonScreen::handleMouseMotion(const SDL_Event& sdle, Logic &logic)
{
	if(bMouseOverButton)
		bMouseOverButton = false;
	else {
		bMouseOverButton = true;		
		float xnorm = (sdle.button.x - 0.5*flWidth) / (0.5*flWidth);
		float ynorm = (0.5*flHeight - sdle.button.y) / (0.5*flHeight);

		int btnId = pickButton(xnorm,ynorm);
		// Handle mouse highlighting (take into account keyboard management of the buttons).
		if( (btnId >= 0) && (btnId != iPrevFocusButton) && (iPrevFocusButton >= 0) ){
			// Unset previously highlighted buttons.
			guiObjects[getHighlightedButton()]->setFocus(false);
			guiObjects[iPrevFocusButton]->setFocus(false);
			// Set new current highlighted button.
			guiObjects[btnId]->setFocus(true);
			iPrevFocusButton = btnId;
		}
	}
}

void ButtonScreen::handleKeyDown(const SDL_Event& sdle, Logic &logic)
{	
	bKeyDown = true;

 	switch(sdle.key.keysym.sym)
	{
	case SDLK_RETURN:		// Press the highlighted button.
		{
			int btnId = getHighlightedButton();			
			if(btnId >= 0)
				guiObjects[btnId]->setPressed(true);
		}
		break;
	}
}

void ButtonScreen::handleKeyUp(const SDL_Event& sdle, Logic &logic)
{	
	map_iter iter;

 	switch(sdle.key.keysym.sym)
	{
	case SDLK_RETURN:		// Press the highlighted button.
		if(bKeyDown)
		{
			int btnId = getHighlightedButton();
			if( btnId >= 0 ){
				guiObjects[btnId]->setPressed(false);
				processButton(btnId, logic);
				bKeyDown = false;
			}
			bKeyDown = false;
		}
		break;

	case SDLK_UP:			// Move button highlighting up.
		if(bKeyDown)
		{
			int btnId = getHighlightedButton();
			if( btnId >= 0 ){
				iter = guiObjects.find(btnId);	// Get iterator to the current element.
				iter->second->setFocus(false);

				if( iter == guiObjects.begin() )
					iter = --(guiObjects.end());
				else					
					--iter;
				iter->second->setFocus(true);
			}
			bKeyDown = false;
		}
		break;

	case SDLK_DOWN:				// Move button highlighting down.
		if(bKeyDown)
		{
			int btnId = getHighlightedButton();
			if( btnId >= 0 ){
				iter = guiObjects.find(btnId);	// Get iterator to the current element.
				iter->second->setFocus(false);

				if( iter == --(guiObjects.end()) )
					iter = guiObjects.begin();
				else					
					++iter;
				iter->second->setFocus(true);
			}
			bKeyDown = false;
		}
		break;
	}
}

void ButtonScreen::notify(Subject* s)
{
	bPlayButtonSound = ((Logic*) s)->bActionsSound;
}


/*________________________________*/
// StartScreen implementation.
// In StartScreen buttons are added only once, during the creation of the object.
StartScreen::StartScreen(int idExt, float w, float h, SDL_Surface* s, 
	TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
	FMOD::System *sys, std::vector<FMOD::Sound*> snd):
	ButtonScreen(idExt, w, h, s, t, fnt, sys, snd)
{
	addButtons();
	bLeftMouseButton = false;
	iPrevFocusButton = START_BTN;
}

StartScreen::~StartScreen()
{
}

void StartScreen::addButtons()
{
	float x = -0.5;
	float y = 0.3;
	float w = 1.;
	float h = 0.25;
	std::string name = "Start";

	try {
		// Button 0.
		GuiObject* pGuiObj = new Button(x, y, w, h, START_BTN, name, textures[0]->id);	
		pGuiObj->setFocus(true);
		guiObjects.insert(std::make_pair(START_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));

		// Button 1.
		y -= 1.5*h;
		name = "Options";
		pGuiObj = new Button(x, y, w, h, OPTIONS_BTN, name, textures[0]->id);	// The same texture id.
		guiObjects.insert(std::make_pair(OPTIONS_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));

		// Button 2.
		y -= 1.5*h;
		name = "How to play";
		pGuiObj = new Button(x, y, w, h, HOWTO_BTN, name, textures[0]->id);	// The same texture id.
		guiObjects.insert(std::make_pair(HOWTO_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));

		// Button 3.
		y -= 1.5*h;
		name = "Training";
		pGuiObj = new Button(x, y, w, h, TRAIN_BTN, name, textures[0]->id);	// The same texture id.
		guiObjects.insert(std::make_pair(TRAIN_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));
	} // End try.
	catch(std::bad_alloc& ba) {
		std::cerr << "Failed to create one of the buttons in StartScreen: memory error, " + std::string(ba.what()) << std::endl;		
		exit(1);
	}
}

/*________________________________*/
// OptionsScreen implementation.
// In OptionsScreen buttons are added every time this screen is shown, not just during creation.
OptionsScreen::OptionsScreen(int idExt, float w, float h, SDL_Surface* s, 
	TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
	FMOD::System *sys, std::vector<FMOD::Sound*> snd, Logic &logic) :
	ButtonScreen(idExt, w, h, s, t, fnt, sys, snd)
{
	setupNewScreen(logic);			// Create new buttons to use the uptodate logic values.
}

OptionsScreen::~OptionsScreen()
{
}

void OptionsScreen::setupNewScreen(Logic &logic)
{
	addButtons(logic);
	bLeftMouseButton = false;
	iPrevFocusButton = BACKGRSND_BTN;
}

void OptionsScreen::addButtons(Logic &logic)
{
	// 'magic numbers'.
	float x = -0.5;
	float y = 0.3;
	float w = 1.;
	float h = 0.25;
	std::string name = "Background Sound";
	std::string caption = " ";

	guiObjects.clear();				// Empty the map of buttons.
	
	try {
		// Button 0.
		if(logic.bBackgroundSound)
			caption = "On";
		else 
			caption = "Off";
		GuiObject* pGuiObj = new OptionsButton(x, y, w, h, 
			BACKGRSND_BTN, name, textures[0]->id, caption);
		pGuiObj->setFocus(true);
		guiObjects.insert(std::make_pair(BACKGRSND_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));

		// Button 1.
		y -= 1.5*h;
		name = "Actions Sound";
		if(logic.bActionsSound)
			caption = "On";
		else 
			caption = "Off";
		pGuiObj = new OptionsButton(x, y, w, h, 
			ACTIONSND_BTN, name, textures[0]->id, caption);	// The same texture id.
		guiObjects.insert(std::make_pair(ACTIONSND_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));

		// Button 2.
		y -= 1.5*h;
		name = "Round";
		caption = std::to_string((_ULonglong)logic.iRound);
		pGuiObj = new OptionsButton(x, y, w, h, 
			ROUND_BTN, name, textures[0]->id, caption);	// The same texture id.
		guiObjects.insert(std::make_pair(ROUND_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));

		// Button 3.
		y -= 1.5*h;
		name = "Back";
		pGuiObj = new Button(x, y, w, h, 
			BACK_BTN, name, textures[0]->id);	// The same texture id.
		guiObjects.insert(std::make_pair(BACK_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));
	}// End try.
	catch(std::bad_alloc& ba) {
		std::cerr << "Failed to create one of the buttons in OptionsScreen: memory error, " + std::string(ba.what()) << std::endl;		
		exit(1);
	}
}

/*________________________________*/
// HowtoScreen implementation.
HowtoScreen::HowtoScreen(int idExt, float w, float h, SDL_Surface* s, 
	TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
	FMOD::System *sys, std::vector<FMOD::Sound*> snd) :
	ButtonScreen(idExt, w, h, s, t, fnt, sys, snd)
{
	bLeftMouseButton = false;
}

HowtoScreen::~HowtoScreen()
{
}

void HowtoScreen::doDrawing(Logic &logic)
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffer for new data.

	glPushMatrix();					// Save current matrix.

	glMatrixMode (GL_PROJECTION);	// Switch to the projection matrix.
	glLoadIdentity ();				// Clean up the projection matrix.

	glMatrixMode (GL_MODELVIEW);	// Switch to the modelview matrix.
	glLoadIdentity ();				// Clean up the modelview matrix.

	// Draw the background image.
	drawBackgroundTexture(id, 0.);

	// Text.
	glEnable( GL_TEXTURE_2D );
	
	SDL_Color textColor;
	textColor.r = 0;
	textColor.g = 0.;
	textColor.b = 0.;
	// 'magic numbers'.
	std::string txt = "Keys (click or space to exit from here)";
	drawText(txt, -0.55, 0.75, 1.1, 0.15, fonts[0], logic, textColor);
	txt = "_______________________________________";
	drawText(txt, -0.55, 0.73, 1.1, 0.15, fonts[0], logic, textColor);

	// 'magic numbers' are below.
	float y = 0.55;
	float h = 0.15;
	float txtX = -0.8;
	float fixedX = 0.3;
	// Keys.
	txt = "Action";
	drawText(txt, txtX, y, 0.2, h, fonts[0], logic, textColor);
	txt = "Key(s)/Mouse";
	drawText(txt, fixedX, y, 0.42, h, fonts[0], logic, textColor);

	y -= 0.2*h;
	txt = "______";
	drawText(txt, txtX, y, 0.2, h, fonts[0], logic, textColor);
	txt = "____________";
	drawText(txt, fixedX, y, 0.42, h, fonts[0], logic, textColor);

	y -= 1.5*h;
	txt = "go back to the start screen:";
	drawText(txt, txtX, y, 0.8, h, fonts[0], logic, textColor);
	txt = "b";
	drawText(txt, fixedX, y, 0.045, h, fonts[0], logic, textColor);

	y -= 1.3*h;
	txt = "pause/unpause:";
	drawText(txt, txtX, y, 0.45, h, fonts[0], logic, textColor);
	txt = "v/space";
	drawText(txt, fixedX, y, 0.23, h, fonts[0], logic, textColor);

	y -= 1.3*h;
	txt = "paddle manipulation:";
	drawText(txt, txtX, y, 0.65, h, fonts[0], logic, textColor);
	txt = "arrows/asdw/mouse";
	drawText(txt, fixedX, y, 0.57, h, fonts[0], logic, textColor);

	y -= 1.3*h;
	txt = "reset view to the initial position:";
	drawText(txt, txtX, y, 1.0, h, fonts[0], logic, textColor);
	txt = "=";
	drawText(txt, fixedX, y, 0.05, h, fonts[0], logic, textColor);

	y -= 1.3*h;
	txt = "zoom:";
	drawText(txt, txtX, y, 0.2, h, fonts[0], logic, textColor);
	txt = "m/p";
	drawText(txt, fixedX, y, 0.12, h, fonts[0], logic, textColor);

	y -= 1.3*h;
	txt = "rotate around y/z axes:";
	drawText(txt, txtX, y, 0.67, h, fonts[0], logic, textColor);
	txt = "mouse(left)/wheel";
	drawText(txt, fixedX, y, 0.51, h, fonts[0], logic, textColor);
	
	txt = "(c) Nikolay Prodanov, 2013. All rights reserved.";
	drawText(txt, -0.5, -0.92, 1., 0.11, fonts[0], logic, textColor);

	SDL_Delay(50);

	glFlush();

	glDisable( GL_TEXTURE_2D );

    glPopMatrix();					// Restore the previous state.
}

void HowtoScreen::handleKeyDown(const SDL_Event& sdle, Logic &logic)
{	
	// Check keyboard.
 	switch(sdle.key.keysym.sym)
	{
	case SDLK_RETURN:				// Fall through.
	case SDLK_SPACE:
		logic.bShowStartScreen = true;
		logic.bShowHowtoScreen = false;
		break;
	}
}

void HowtoScreen::handleMouseButtonUp(const SDL_Event& sdle, Logic &logic)
{
	switch(sdle.button.button)
	{
	case SDL_BUTTON_LEFT:
		logic.bShowStartScreen = true;
		logic.bShowHowtoScreen = false;
		break;

	case SDL_QUIT:
		logic.bAppRunning = false;
		break;
	}
}

/*________________________________*/
// PlayScreen implementation.
PlayScreen::PlayScreen(int idExt, float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt, 
	 FMOD::System *sys,std::vector<FMOD::Sound*> snd,
	 std::vector<std::tr1::shared_ptr<RoundParameters> > & rp) :
	 SdlScreen(idExt, w, h, s, t, fnt, sys, snd), roundParams(rp)
{
	userWallIdx = -1;
	compWallIdx = -1;
	iShowMessageCount = 0;
	flDeltaAngleViewZ = 0.3;
	flDeltaScale = 0.01;
}

PlayScreen::~PlayScreen()
{
}

void PlayScreen::initMembers(const Logic &logic)
{
	iCurRound = logic.iRound - 1;
	flBoxWidth = roundParams[iCurRound]->flBoxWidth;
	flBoxHeight = roundParams[iCurRound]->flBoxHeight;
	flBoxThickness = 0.5*roundParams[iCurRound]->flBoxHeight;
	flPaddleRadius = roundParams[iCurRound]->flPaddleRadius;
	flBallVel = roundParams[iCurRound]->flBallVelocity;
	flBallDeltaVel = roundParams[iCurRound]->flBallDeltaVel;
	flCompPaddleVel = roundParams[iCurRound]->flComputerPaddleVel;

	// 'magic numbers'.
	xViewOld = 0.;
	angleViewY = 0.;					// Initial angle (around y) of the view.
	angleViewYMax = 75.;				// Final initial angle view.
	deltaAngleY = angleViewYMax * 0.02;	// Increments of the angle to rotate at the beginning.
	angleViewZ = 0.;
	bPaddlePicked = false;
	xPaddleOld = yPaddleOld = 0.;
	flZaxisDistance = 2.f;
	flLengthUnit = 0.42f;			// This influences the distance from the camera.
									// The smaller the value, the closer the scene to the camera.
	flScaleAll = 1.;					// Scaling factor.
	flScaleMax = 4.;
	flScaleMin = 0.1;
}

void PlayScreen::addShapes(Logic &logic)
{
	// Cleanup the shapes from the previous round.
	shapes.clear();
	// Add shapes to the game.
	// 'magic numbers' - a lot of them are below.
	try {
		// Add ball.
		vector_3d ambient = vector_3d(0.0, 0.5, 0.0);
		vector_3d diffuse = vector_3d(0.0, 1.0, 0.0);
		vector_3d specular = vector_3d(0.0, 0.0, 0.0);
		float alpha = 1.0;						// Opaque ball.
		float shine = 0.;
		float maxv = 0.05;
		vector_3d center(0.0f, 0.0f, 0.0f);		// Ball starts at the center of the scene.
		// Random velocity.
		// x component.
		float sign = 1.;
		if( generateRand(-1., 1.) < 0)
			sign = -1.;
		float vx = sign*flBallVel;
		if( generateRand(-1., 1.) < 0)
			sign = -1.;
		else
			sign = 1.;
		// y component.
		float vy = sign*flBallVel;
		if( generateRand(-1., 1.) < 0)
			sign = -1.;
		else
			sign = 1.;
		// z component.
		float vz = sign*flBallVel;
		vector_3d velocity(
			generateRand(0.95*vx, vx), 
			generateRand(0.95*vy, vy), 
			generateRand(0.95*vz, vz)
			);
		Shape* pShape = new Ball(BALL, center, 0.1*flBoxThickness, velocity, flBallDeltaVel,
			ambient, diffuse, specular, shine, alpha, flBallVel);
		shapes.insert(std::make_pair(BALL, std::tr1::shared_ptr<Shape>(pShape)));	

		// Add walls.		
		// Left wall.
		center = vector_3d(-0.5*flBoxWidth, 0., 0.);
		vector_3d n = vector_3d(-1., 0., 0.);
		pShape = new AbsorbingWall( WALL, center, flBoxThickness, flBoxHeight, n);
		pShape->setSound(system, sounds[5]);
		userWallIdx = shapes.size();			// Save the index of the user's wall.
		shapes.insert(std::make_pair(shapes.size(), std::tr1::shared_ptr<Shape>(pShape)));

		// Right wall.
		center = vector_3d(0.5*flBoxWidth, 0., 0.);
		n = vector_3d(1., 0., 0.);
		pShape = new AbsorbingWall( WALL, center, flBoxThickness, flBoxHeight, n);
		pShape->setSound(system, sounds[5]);
		compWallIdx = shapes.size();			// Use this only when absorbing wall.

		shapes.insert(std::make_pair(shapes.size(), std::tr1::shared_ptr<Shape>(pShape)));

		// Top wall.
		ambient = vector_3d(0.0, 0.5, 0.5);
		diffuse = vector_3d(0.0, 1.0, 1.0);
		specular = vector_3d(0.0, 0.2, 0.2);
		alpha = 0.4;	// A bit transparent wall.
		shine = 20.;
		center = vector_3d(0., 0.5*flBoxHeight, 0.);
		n = vector_3d(0., 1., 0.);
		pShape = new Wall( WALL, center, flBoxWidth, flBoxThickness, n,
			ambient, diffuse, specular, shine, alpha);
		pShape->setSound(system, sounds[4]);
		shapes.insert(std::make_pair(shapes.size(), std::tr1::shared_ptr<Shape>(pShape)));

		// Bottom wall.
		ambient = vector_3d(0.0, 0.5, 0.5);
		diffuse = vector_3d(0.0, 1.0, 1.0);
		specular = vector_3d(0.0, 0.2, 0.2);
		alpha = 0.4;	// A bit transparent wall.
		shine = 20.;
		center = vector_3d(0., -0.5*flBoxHeight, 0.);
		n = vector_3d(0., -1., 0.);
		pShape = new Wall( WALL, center, flBoxWidth, flBoxThickness, n,
			ambient, diffuse, specular, shine, alpha);
		pShape->setSound(system, sounds[4]);
		shapes.insert(std::make_pair(shapes.size(), std::tr1::shared_ptr<Shape>(pShape)));

		// Front wall.
		center = vector_3d(0., 0., 0.5*flBoxThickness);
		n = vector_3d(0., 0., 1.);
		pShape = new Wall( WALL, center, flBoxWidth, flBoxHeight, n);
		pShape->setSound(system, sounds[4]);
		shapes.insert(std::make_pair(shapes.size(), std::tr1::shared_ptr<Shape>(pShape)));

		// Back wall.
		center = vector_3d(0., 0., -0.5*flBoxThickness);
		n = vector_3d(0., 0., -1.);
		pShape = new Wall( WALL, center, flBoxWidth, flBoxHeight, n);
		pShape->setSound(system, sounds[4]);
		shapes.insert(std::make_pair(shapes.size(), std::tr1::shared_ptr<Shape>(pShape)));

		// Paddles.
		// User (left) paddle.
		center = vector_3d(-0.5*flBoxWidth, 0., 0.);
		n = vector_3d(-1., 0., 0.);			// Norm is -x.
		float angle = 90.0;

		// Setup colors.
		ambient = vector_3d(0.5, 0.5, 0.0);
		diffuse = vector_3d(1.0, 1.0, 0.0);
		specular = vector_3d(0.5, 0.5, 0.0);
		alpha = 0.9;	// A bit transparent paddle.
		shine = 10.;

		// Add user or computer paddle depending on the options.
		if(logic.bTrain)
			pShape = new ComputerPaddle( LEFT_PADDLE, center, n, flPaddleRadius, 0.01*flBoxWidth, 
			angle, 0.5*flBoxHeight, 0.5*flBoxThickness,
			ambient, diffuse, specular, shine, alpha, flBallDeltaVel, flCompPaddleVel);
		else
			pShape = new Paddle( LEFT_PADDLE, center, n, flPaddleRadius, 0.01*flBoxWidth, 
			angle, 0.5*flBoxHeight, 0.5*flBoxThickness,
			ambient, diffuse, specular, shine, alpha, flBallDeltaVel);
		pShape->setSound(system, sounds[6]);
		leftPaddleIdx = 7;
		shapes.insert(std::make_pair(shapes.size(), std::tr1::shared_ptr<Shape>(pShape)));

		// Computer (right) paddle.
		center = vector_3d(0.5*flBoxWidth, 0., 0.);
		n = vector_3d(1., 0., 0.);			// Norm is x.
		angle = -90.0;
		// Setup colors.
		ambient = vector_3d(0.5, 0.0, 0.5);
		diffuse = vector_3d(1.0, 0.0, 1.0);
		specular = vector_3d(0.5, 0.0, 0.5);
		pShape = 
			new ComputerPaddle( RIGHT_PADDLE, center, n, flPaddleRadius, 0.01*flBoxWidth, 
			angle, 0.5*flBoxHeight, 0.5*flBoxThickness,
			ambient, diffuse, specular, shine, alpha, flBallDeltaVel, flCompPaddleVel);
		pShape->setSound(system, sounds[6]);
		rightPaddleIdx = 8;
		shapes.insert(std::make_pair(shapes.size(), std::tr1::shared_ptr<Shape>(pShape)));
	} // End try.
	catch(std::bad_alloc& ba) {
		std::cerr << "Failed to create one of the shapes in PlayScreen: memory error, " + std::string(ba.what()) << std::endl;		
		exit(1);
	}
}

void PlayScreen::doInput(Logic &logic, SDL_Event sdlEvent)
{    
    switch(sdlEvent.type)
    {
	case SDL_MOUSEBUTTONDOWN:
		if(!logic.bGamePaused)
			handleMouseButtonDown(sdlEvent, logic);
		break;

	case SDL_MOUSEBUTTONUP:
		if(!logic.bGamePaused)
			handleMouseButtonUp(sdlEvent, logic);
		break;

	case SDL_MOUSEMOTION:
		if(!logic.bGamePaused)
			handleMouseMotion(sdlEvent, logic);
		break;
	
	case SDL_KEYDOWN:
		handleKeyDown(sdlEvent, logic);
		break;

	case SDL_VIDEORESIZE:
		handleResize(sdlEvent, logic);
		break;			

	case SDL_QUIT:
		logic.bAppRunning = false;
		break;
	}
}

void PlayScreen::drawScoreAndRound(Logic &logic)
{
	glPushMatrix();					// Save current matrix.

	glEnable( GL_TEXTURE_2D );
	glRotatef(-90, 0.0f, 1.0f, 0.0f);

	SDL_Color textColor;
	textColor.r = 0;
	textColor.g = 0.;
	textColor.b = 1.;
	
	std::stringstream str0;
	str0 << "R:" << logic.iRound << " U:" << logic.iUserScore << " C:" << logic.iCompScore;
	drawText(str0.str(), -0.195, -0.5*flBoxHeight, 0.35, 0.1, fonts[0], logic, textColor);

	if(logic.bRoundFinished){
		glTranslatef(0., 0., 0.3);
		std::string str3;
		if(logic.bGameOver){
			str3 = "You lost!";
			drawText(str3, -0.195, -0.5*flBoxHeight, 0.4, 0.1, fonts[0], logic, textColor);
		}
		else{
			str3 = "Comp lost!";
			drawText(str3, -0.195, -0.5*flBoxHeight, 0.4, 0.1, fonts[0], logic, textColor);
		}		
	}

	glDisable( GL_TEXTURE_2D);
	
	glPopMatrix();
}

void PlayScreen::doDrawing(Logic &logic)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffer for new data.
	// Draw the background image.	
	drawBackgroundTexture( (id+(logic.iRound-1)%3), 0.5);
	initResize();
	initView();

	// Draw all the shapes.
	for(std::size_t i = 0; i < shapes.size(); i++){
		shapes[i]->draw();
	}

	// Draw the score.
	drawScoreAndRound(logic);

	glFlush();
}

// 'magic numbers' are below.
void PlayScreen::handleMouseButtonDown(const SDL_Event& sdle, Logic &logic)
{
	int objName = 0;
	switch(sdle.button.button)
	{
	case SDL_BUTTON_LEFT:
		objName = pickObject(sdle.button.x, sdle.button.y);
		if(objName == LEFT_PADDLE){
			bPaddlePicked = true;
			xPaddleOld = sdle.button.x;
			yPaddleOld = sdle.button.y;
		}
		xViewOld = sdle.button.x;
		break;

	case SDL_BUTTON_WHEELUP:
		angleViewZ -= flDeltaAngleViewZ;			// Rotate around the horizontal axis.
		break;

	case SDL_BUTTON_WHEELDOWN:
		angleViewZ += flDeltaAngleViewZ;
		break;
	}// End switch.
}

void PlayScreen::handleMouseButtonUp(const SDL_Event& sdle, Logic &logic)
{
	switch(sdle.button.button)
	{
	case SDL_BUTTON_LEFT:
		bPaddlePicked = false;		// Paddle is released.
		break;
	}
}

void PlayScreen::handleMouseMotion(const SDL_Event& sdle, Logic &logic)
{
	switch(sdle.motion.state)
	{
	case SDL_BUTTON_LEFT:
		if(!bPaddlePicked){
			angleViewY += sdle.motion.x - xViewOld;	// If paddle is not chosen, rotate view.
			xViewOld = sdle.motion.x;
		}
		else{
			// 'magic numbers'.
			float ratioX = 3./flWidth;		// Make these members.
			float ratioY = 2.8/flHeight;
			vector_3d dr(0., -ratioX*(sdle.motion.y - yPaddleOld), 
				ratioY*(sdle.motion.x - xPaddleOld) );
			shapes[leftPaddleIdx]->move(dr, false);
			xPaddleOld = sdle.motion.x;
			yPaddleOld = sdle.motion.y;
		}
		break;
	}
}

void PlayScreen::handleKeyDown(const SDL_Event& sdle, Logic &logic)
{	
	vector_3d dr;
	// Check keyboard.
 	switch(sdle.key.keysym.sym)
	{
	case SDLK_EQUALS:					// Reset view.
		angleViewY = angleViewYMax;
		angleViewZ = 0.;
		flScaleAll = 1.;
		break;

	case SDLK_b:
		if(!logic.bShowStartScreen) {
			logic.bShowStartScreen = true;
			logic.bNewRound = true;		// Start from scratch.
		}
		logic.notifyObservers();		// Tell observers to change their sound behavior.
		break;

	case SDLK_p:	 //Scale.
		if(flScaleAll < flScaleMax)
			flScaleAll += flDeltaScale;			// 'magic number'.
		break;

	case SDLK_m:
		if(flScaleAll > flScaleMin)
			flScaleAll -= flDeltaScale;
		break;

	case SDLK_v:
		logic.bGamePaused = true;
		logic.bNewRound = false;
		logic.notifyObservers();
		break;

	case SDLK_SPACE:
		logic.bGamePaused = false;
		logic.notifyObservers();
		break;

	case SDLK_ESCAPE:
		logic.bAppRunning = false;
		break;	

		// Arrow keys manipulation of the paddle.
	case SDLK_a:			// Fall through.
	case SDLK_LEFT:
		dr = vector_3d(0., 0.0, -0.01);		// Reconsider magic number!
		shapes[leftPaddleIdx]->move(dr, false);
		break;

	case SDLK_d:			// Fall through.
	case SDLK_RIGHT:
		dr = vector_3d(0., 0.0, 0.01);
		shapes[leftPaddleIdx]->move(dr, false);
		break;

	case SDLK_w:			// Fall through.
	case SDLK_UP:
		dr = vector_3d(0., 0.01, 0.);
		shapes[leftPaddleIdx]->move(dr, false);
		break;

	case SDLK_s:			// Fall through.
	case SDLK_DOWN:
		dr = vector_3d(0., -0.01, 0.);
		shapes[leftPaddleIdx]->move(dr, false);
		break;
	}
}

void PlayScreen::handleResize(const SDL_Event& sdle, Logic &logic)
{
	glViewport (0, 0, (GLsizei) sdle.resize.w, (GLsizei) sdle.resize.h);
	flWidth = sdle.resize.w;
	flHeight = sdle.resize.h;
	logic.flScreenWidth = flWidth;
	logic.flScreenHeight = flHeight;

	initResize();		// Avoid code duplication with setup matrices.
}

// Object selection using back buffer.
int PlayScreen::pickObject(int x, int y)
{
	// Render onto the back buffer using colors corresponding to names of the objects.
	initView();		// Avoid code duplication with doDrawing().

	glDisable(GL_LIGHTING);
	// Draw all the shapes into the double buffer.
	for(std::size_t i = 0; i < shapes.size(); i++) {
		int name = shapes[i]->getId();
		// Use not material colors, but the shape-specific color for identification.
		glColor3f (0.3*name, 0., 0.);		// !Reconsider this later! May be too simple.
		shapes[i]->draw();
	}

	glFlush();

	// Now read the pixel under cursor and analyze the color.
	GLint viewport[4];	
	glGetIntegerv (GL_VIEWPORT, viewport);

	GLint mouse_x = x;
	GLint mouse_y = viewport[3] - y;

	float pixel[3];
    glReadPixels(mouse_x, mouse_y, 1, 1, GL_RGB, GL_FLOAT, pixel);
    float r = pixel[0];
    float g = pixel[1];
    float b = pixel[2];

	int iFigureType = (int)(r/0.3);

	glEnable(GL_LIGHTING);
   
	return iFigureType;
}

void PlayScreen::initResize()
{
	glMatrixMode (GL_PROJECTION);		// Switch to the projection matrix.
	glLoadIdentity ();					// Clean up the projection matrix.
	gluPerspective(calculateAngle(flLengthUnit, flZaxisDistance), 
		flWidth/flHeight, 1.0, 1024.0);	// Set up the projection matrix with the same units in x and y.
	glMatrixMode (GL_MODELVIEW);		// Switch to the modelview matrix.
	glLoadIdentity ();
}

void PlayScreen::initView()
{
	glLoadIdentity ();             // Clear the current matrix.
	// Move camera to the point (0,0,zdistance) in eye coords, look at point (0,0,0), 
	// camera orientation - the normal is along (0,1,0)
	gluLookAt (0.0, 0.0, flZaxisDistance, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(angleViewY, 0.0f, 1.0f, 0.0f);
	glRotatef(angleViewZ, 0.0f, 0.0f, 1.0f);
	glScalef(flScaleAll, flScaleAll,flScaleAll);

	//drawAxes();	
}

void PlayScreen::drawAxes() const
{
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	// x, red
	/*glColor3f(2.f, 0.f, 0.f);
	glVertex3f(-0.5f, 0.f, 0.f);
	glVertex3f(0.5f, 0.f, 0.f);*/
	// y, green
	glColor3f(0.f, 2.f, 0.f);
	glVertex3f(0.f, -0.5f, 0.f);
	glVertex3f(0.f, 0.5f, 0.f);
	// z, blue
	/*glColor3f(0.f, 0.f, 2.f);
	glVertex3f(0.f, 0.f, -0.5f);
	glVertex3f(0.f, 0.f, 0.5f);*/
	glEnd();

	glEnable(GL_LIGHTING);
}

void PlayScreen::doLogic(Logic &logic)
{
	// Move the shapes.
	bool bReset = false;	// Whether to move the ball to the origin.
	for(std::size_t i = 0; i < shapes.size(); i++)
		shapes[i]->move(vector_3d(0., 0., 0.), bReset);

	// Detect collisions for all shapes with the ball.
	bool bCollided = false;
	for(int i = shapes.size() - 1; i >= 0; i--){
		bCollided = shapes[i]->collide(shapes[0].get()); 
		if(bCollided){
			if(bPlaySound)
				shapes[i]->playSound();
			if(i == userWallIdx){		// User lost.
				logic.iCompScore += 1;
			}
			if(i == compWallIdx){		// Comp lost.
				logic.iUserScore += 1;
			}
			break;
		}
	}

	// Check whether it's time to change the round.
	if(logic.iUserScore >= logic.iMaxScore){
		logic.iRound = logic.iRound % logic.iRoundMax + 1;
		logic.bRoundFinished = true;	// This flag provides a pause.
	}
    
    // If the player lost, do game over effects
	if(logic.iCompScore >= logic.iMaxScore){
		logic.bGameOver = true;
		logic.bRoundFinished = true;
	}
}

void PlayScreen::play(Logic &logic, SDL_Event sdlEvent)
{
	if(logic.bNewRound){
		setScreenSize(logic.flScreenWidth, logic.flScreenHeight);
		initResize();
		
		setupNewRound(logic);

		logic.bNewRound = false;
		logic.bRotated = false;
		logic.bGameOver = false;
	}

	// Rotate the view at the beginning of a new round.
	if( (angleViewY < angleViewYMax) && (!logic.bRotated ) ) {
		angleViewY += deltaAngleY;
	}
	else
		logic.bRotated = true;	
	
	// Process input if the round started and is going on.
	if(logic.bRotated  && !logic.bRoundFinished){
		doInput(logic, sdlEvent);
		if( !logic.bGamePaused)
			doLogic(logic);
	}

	// If round finished, show a msg about who has lost.
	if(logic.bRoundFinished){
		if(iShowMessageCount < 100)
			++iShowMessageCount;
		else{
			iShowMessageCount = 0;
			logic.bRoundFinished = false;
			logic.bNewRound = true;
		}
	}
	
	// Draw the scene.
	doDrawing(logic);	
}

void PlayScreen::setupNewRound(Logic &logic)
{	
	// Reset scores.
	logic.iCompScore = 0;
	logic.iUserScore = 0;

	initMembers(logic);
	addShapes(logic);
	logic.notifyObservers();			// Tell the GameApp to use a new round sound.
}

// Observer pattern method.
void PlayScreen::notify(Subject* s) 
{
	bPlaySound = ((Logic*) s)->bActionsSound;
}