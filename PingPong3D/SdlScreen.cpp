// SdlScreen.cpp - implementation of the SdlScreen class and all its child classes.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#include "SdlScreen.h"

/*________________________________*/
//  SdlScreen implementation.
SdlScreen::SdlScreen(float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
		FMOD::System *sys, std::vector<FMOD::Sound*> snd) :
		flWidth(w), flHeight(h), surfaceOnWhichWeRender(s),
		textures(t), fonts(fnt), system(sys), sounds(snd)
{
	 bLeftMouseButton = false;
}

SdlScreen::~SdlScreen(void)
{
}

int SdlScreen::drawText(const std::string &txt, GLfloat x, GLfloat y, GLfloat w, GLfloat h,
	TTF_Font *font, Logic &logic)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffer for new data.
	int mode;

	SDL_Color textColor;
	SDL_Surface *text;
	GLuint textTexture;

	textColor.r = 1;
    textColor.g = 0;
    textColor.b = 0;
	
	text = TTF_RenderText_Blended(font, txt.c_str(), textColor);
	SDL_Delay(25);			// Small delay to prevent from full CPU load.

	if (text->format->BytesPerPixel == 3) // RGB 24bit.
    { 
        mode = GL_RGB;
    } 
    else if (text->format->BytesPerPixel == 4) // RGBA 32bit.
    { 
        mode = GL_RGBA;
    } 
    else 
    {
        printf("Could not determine pixel format of the surface");
        SDL_FreeSurface(text);
        return NULL;
    }

	// Draw the surface onto the OGL screen using gextures.
    glPushMatrix();
	glDisable(GL_LIGHTING);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);

    glGenTextures(1, &textTexture);
    glBindTexture(GL_TEXTURE_2D, textTexture);

	// Create texture bound to our text surface.
    glTexImage2D(GL_TEXTURE_2D, 0, 4, text->w, text->h, 0, mode, GL_UNSIGNED_BYTE, text->pixels );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
    glColor3f(0.0f, 1.0f, 1.0f);

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

void SdlScreen::doInput(Logic &logic, SDL_Event sdlEvent)
{    
    switch(sdlEvent.type)
    {
	case SDL_MOUSEBUTTONDOWN:
		bLeftMouseButton = true;	// Prevent button captions from flickering.
		handleMouseButtonDown(sdlEvent, logic);
		break;

	case SDL_MOUSEBUTTONUP:
		if(bLeftMouseButton){
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

void SdlScreen::handleResize(const SDL_Event& sdle, Logic &l)
{
	SDL_SetVideoMode( sdlEvent.resize.w, sdlEvent.resize.h, 32, SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE );
	glViewport (0, 0, (GLsizei) sdle.resize.w, (GLsizei) sdle.resize.h);
	flWidth = sdle.resize.w;
	flHeight = sdle.resize.h;
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
// Set bIsPressed button flag and get the id of the pressed button.
ButtonScreen::ButtonScreen(float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
		FMOD::System *sys, std::vector<FMOD::Sound*> snd) :
		SdlScreen(w, h, s, t, fnt, sys, snd)
{
	bPlayButtonSound = true;
}

ButtonScreen::~ButtonScreen()
{
}

int ButtonScreen::pressButton(float x, float y)
{
	typedef std::map<std::size_t, std::tr1::shared_ptr<GuiObject> >::iterator map_iter;
	
	for(map_iter iterator = guiObjects.begin(); iterator != guiObjects.end(); iterator++){
		if( iterator->second->isPressed(x, y) ){
			iterator->second->setPressed(true);
			return iterator->second->getId();
		}
	}

	return -1;
}

int ButtonScreen::pickButton(float px, float py)
{
	typedef std::map<std::size_t, std::tr1::shared_ptr<GuiObject> >::iterator map_iter;
	
	for(map_iter iterator = guiObjects.begin(); iterator != guiObjects.end(); iterator++){
		if( iterator->second->isPressed(px, py) ){			
			return iterator->second->getId();
		}
	}

	return -1;
}

void ButtonScreen::processButton(std::size_t id, Logic & logic)
{
	switch(id)
	{ 
	case START_BTN:
		logic.bShowStartScreen = false;
		logic.bShowOptionsScreen = false;
		logic.bShowPlayScreen = true;
		logic.bGamePaused = false;
		logic.bTrain = false;
		logic.notifyObservers(); // Tell registered observers to change their settings.
		break;

	case OPTIONS_BTN:
		logic.bShowStartScreen = false;
		logic.bShowOptionsScreen = true;
		logic.bShowPlayScreen = false;
		//logic.notifyObservers();
		break;

	case HOWTO_BTN:
		logic.bShowStartScreen = false;
		logic.bShowOptionsScreen = false;
		logic.bShowHowtoScreen = true;			
		//logic.notifyObservers(); // Tell registered observers to change their settings.
		break;

	case TRAIN_BTN:
		logic.bShowStartScreen = false;
		logic.bShowOptionsScreen = false;
		logic.bShowPlayScreen = true;
		logic.bGamePaused = false;
		logic.bTrain = true;
		logic.notifyObservers();
		break;

	case BACKGRSND_BTN:
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
		//logic.bActionsSound = !logic.bActionsSound;
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
		//caption = std::to_string((_ULonglong)logic.iRound);
		((OptionsButton*)guiObjects[id].get())->setCaption(std::to_string((_ULonglong)logic.iRound));
		logic.notifyObservers(); 
		break;

	case BACK_BTN:
		logic.bShowOptionsScreen = false;
		logic.bShowStartScreen = true;
		break;
	}// end switch(id)

	if(bPlayButtonSound)
		::playSound(system, sounds[1], channel);
}

//std::map<std::size_t, std::tr1::shared_ptr<GuiObject> > & ButtonScreen::getGuiObjects()
//{
//	return guiObjects;
//}

void ButtonScreen::doDrawing(Logic &logic)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffer for new data.

	glPushMatrix();					// Save current matrix.

	glMatrixMode (GL_PROJECTION);	// Switch to the projection matrix.
	glLoadIdentity ();				// Clean up the projection matrix.

	glMatrixMode (GL_MODELVIEW);	// Switch to the modelview matrix.
	glLoadIdentity ();				// Clean up the modelview matrix.

	glEnable( GL_TEXTURE_2D );
	
	// Change this using drawPressed/unpressed.
	typedef std::map<std::size_t, std::tr1::shared_ptr<GuiObject> >::iterator map_iter;
	
	for(map_iter iterator = guiObjects.begin(); iterator != guiObjects.end(); iterator++){
		if( iterator->second->isPressed() )
			iterator->second->drawPressed(fonts[0]);
		else
			iterator->second->drawUnpressed(fonts[0]);
	}

	glFlush();

	glDisable( GL_TEXTURE_2D );

    glPopMatrix();					// Restore the previous state.
}

void ButtonScreen::handleMouseButtonDown(const SDL_Event& sdle, Logic &logic)
{
	switch(sdle.button.button)
	{
	case SDL_BUTTON_LEFT:
		{
			// Normalize screen coordinates.
			// OpenGL coords (used for rendering) go from -1 to 1 with the origin in the middle.
			// While clicking coords have (0, 0) in the upper left corner.
			float xnorm = (sdle.button.x - 0.5*flWidth) / (0.5*flWidth);
			float ynorm = (0.5*flHeight - sdle.button.y) / (0.5*flHeight);
			
			//std::size_t btnId = pickButton(xnorm, ynorm);
			pressButton(xnorm, ynorm);
		}
		break;
	}// end switch
}

void ButtonScreen::handleMouseButtonUp(const SDL_Event& sdle, Logic &logic)
{
	switch(sdle.button.button)
	{
	case SDL_BUTTON_LEFT:
		{
			// Normalize screen coordinates.
			// OpenGL coords (used for rendering) go from -1 to 1 with the origin in the middle.
			// While clicking coords have (0, 0) in the upper left corner.
			float xnorm = (sdle.button.x - 0.5*flWidth) / (0.5*flWidth);
			float ynorm = (0.5*flHeight - sdle.button.y) / (0.5*flHeight);
			
			int btnId = pickButton(xnorm, ynorm);
			if(btnId >= 0){
				guiObjects[btnId]->setPressed(false);	// Unpress button.
				processButton(btnId, logic);
			}
		}
		break;
	}// end switch
}

void ButtonScreen::notify(Subject* s)
{
	bPlayButtonSound = ((Logic*) s)->bActionsSound;
}


/*________________________________*/
// StartScreen implementation.
StartScreen::StartScreen(float w, float h, SDL_Surface* s, 
	TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
	FMOD::System *sys, std::vector<FMOD::Sound*> snd):
	ButtonScreen(w, h, s, t, fnt, sys, snd)//, iNumOfButtons(4)
{
	addButtons();
	bLeftMouseButton = false;
}

StartScreen::~StartScreen()
{
}

void StartScreen::addButtons()
{
	//guiObjects.resize(iNumOfButtons);
	float x = -1;
	float y = 0.5;
	float w = 1.;
	float h = 0.25;
	std::string name = "Start";

	// Later put the code for adding a button into a method to avoid code duplication!
	
	// Button 0.
	Button* pButton = new Button(x, y, w, h, START_BTN, name, textures[0]->id);
	GuiObject* pGuiObj = dynamic_cast<GuiObject*>(pButton);	
	//pButton->setSound(system, sounds[1]);
	//guiObjects[START_BTN] = std::tr1::shared_ptr<GuiObject>(pGuiObj);
	guiObjects.insert(std::make_pair(START_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));

	// Button 1.
	y = 0.125;
	name = "Options";
	pButton = new Button(x, y, w, h, OPTIONS_BTN, name, textures[0]->id);	// The same texture id.
	pGuiObj = dynamic_cast<GuiObject*>(pButton);
	//pButton->setSound(system, sounds[1]);
	//guiObjects[OPTIONS_BTN] = std::tr1::shared_ptr<GuiObject>(pGuiObj);
	guiObjects.insert(std::make_pair(OPTIONS_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));

	// Button 2.
	y = -0.25;
	name = "How to play";
	pButton = new Button(x, y, w, h, HOWTO_BTN, name, textures[0]->id);	// The same texture id.
	pGuiObj = dynamic_cast<GuiObject*>(pButton);
	//pButton->setSound(system, sounds[1]);
	//guiObjects[HOWTO_BTN] = std::tr1::shared_ptr<GuiObject>(pGuiObj);
	guiObjects.insert(std::make_pair(HOWTO_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));

	// Button 3.
	y = -0.625;
	name = "Train on your own";
	pButton = new Button(x, y, w, h, TRAIN_BTN, name, textures[0]->id);	// The same texture id.
	pGuiObj = dynamic_cast<GuiObject*>(pButton);
	//pButton->setSound(system, sounds[1]);
	//guiObjects[TRAIN_BTN] = std::tr1::shared_ptr<GuiObject>(pGuiObj);
	guiObjects.insert(std::make_pair(TRAIN_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));
}

//void StartScreen::doDrawing(Logic &logic)
//{	
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffer for new data.
//
//	glPushMatrix();					// Save current matrix.
//
//	glMatrixMode (GL_PROJECTION);	// Switch to the projection matrix.
//	glLoadIdentity ();				// Clean up the projection matrix.
//
//	glMatrixMode (GL_MODELVIEW);	// Switch to the modelview matrix.
//	glLoadIdentity ();				// Clean up the modelview matrix.
//
//	glEnable( GL_TEXTURE_2D );
//	
//	// Change this using drawPressed/unpressed.
//	for(std::size_t i = 0; i < guiObjects.size(); i++){
//		if( guiObjects[i]->isPressed() )
//			guiObjects[i]->drawPressed(fonts[0], logic);
//		else
//			guiObjects[i]->drawUnpressed(fonts[0], logic);
//	}
//
//	glFlush();
//
//	glDisable( GL_TEXTURE_2D );
//
//    glPopMatrix();					// Restore the previous state.
//}


//void StartScreen::doInput(Logic &logic, SDL_Event sdlEvent)
//{
//    switch(sdlEvent.type)
//    {
//	case SDL_MOUSEBUTTONDOWN:
//		bLeftMouseButton = true;	// Prevent button captions from flickering.
//		break;
//
//	case SDL_MOUSEBUTTONUP:
//		if(bLeftMouseButton){
//			bLeftMouseButton = false;
//			handleMouseButtonUp(sdlEvent, logic);			
//		}
//		break;
//
//	case SDL_KEYDOWN:
//		handleKeyDown(sdlEvent, logic);
//		break;
//
//	case SDL_QUIT:
//		logic.bAppRunning = false;
//		break;
//	}
//}

// !Later: add button manipulation here.
//void StartScreen::handleKeyDown(const SDL_Event& sdle, Logic &logic)
//{	
//	// Check keyboard.
// 	switch(sdle.key.keysym.sym)
//	{
//	case SDLK_h:		
//		break;
//	case SDLK_k:	
//		break;
//	}
//}

/*________________________________*/
// OptionsScreen implementation.
OptionsScreen::OptionsScreen(float w, float h, SDL_Surface* s, 
	TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
	FMOD::System *sys, std::vector<FMOD::Sound*> snd, Logic &logic) :
	ButtonScreen(w, h, s, t, fnt, sys, snd)
{
	addButtons(logic);
	bLeftMouseButton = false;
}

OptionsScreen::~OptionsScreen()
{
}

void OptionsScreen::addButtons(Logic &logic)
{
	//guiObjects.resize(iNumOfButtons);
	float x = -0.5;
	float y = 0.5;
	float w = 1.;
	float h = 0.25;
	std::string name = "Background Sound";
	std::string caption = " ";

	// Later put the code for adding a button into a method to avoid code duplication!
	
	// Button 0.
	if(logic.bBackgroundSound)
		caption = "On";
	else 
		caption = "Off";
	GuiObject* pGuiObj = new OptionsButton(x, y, w, h, 
		BACKGRSND_BTN, name, textures[0]->id, caption);
	//GuiObject* pGuiObj = dynamic_cast<GuiObject*>(pButton);	
	//pButton->setSound(system, sounds[1]);	
	//guiObjects[BACKGRSND_BTN] = std::tr1::shared_ptr<GuiObject>(pGuiObj);
	guiObjects.insert(std::make_pair(BACKGRSND_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));

	// Button 1.
	y = 0.125;
	name = "Actions Sound";
	if(logic.bActionsSound)
		caption = "On";
	else 
		caption = "Off";
	pGuiObj = new OptionsButton(x, y, w, h, 
		ACTIONSND_BTN, name, textures[0]->id, caption);	// The same texture id.
	//pGuiObj = dynamic_cast<GuiObject*>(pButton);
	//pButton->setSound(system, sounds[1]);	
	//guiObjects[ACTIONSND_BTN] = std::tr1::shared_ptr<GuiObject>(pGuiObj);
	guiObjects.insert(std::make_pair(ACTIONSND_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));

	// Button 2.
	y = -0.25;
	name = "Round";
	caption = std::to_string((_ULonglong)logic.iRound);
	pGuiObj = new OptionsButton(x, y, w, h, 
		ROUND_BTN, name, textures[0]->id, caption);	// The same texture id.
	//pGuiObj = dynamic_cast<GuiObject*>(pButton);
	//pButton->setSound(system, sounds[1]);
	//pButton->setCaption( std::to_string((_ULonglong)logic.iRound) );
	//guiObjects[ROUND_BTN] = std::tr1::shared_ptr<GuiObject>(pGuiObj);
	guiObjects.insert(std::make_pair(ROUND_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));

	// Button 3.
	y = -0.625;
	name = "Back";
	//caption = " ";
	pGuiObj = new Button(x, y, w, h, 
		BACK_BTN, name, textures[0]->id/*, caption*/);	// The same texture id.
	//pGuiObj = dynamic_cast<GuiObject*>(pButton);
	//pButton->setSound(system, sounds[1]);
	//pButton->setCaption("  ");
	//guiObjects[BACK_BTN] = std::tr1::shared_ptr<GuiObject>(pGuiObj);
	guiObjects.insert(std::make_pair(BACK_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));
}

//void OptionsScreen::doDrawing(Logic &logic)
//{	
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffer for new data.
//
//	glPushMatrix();					// Save current matrix.
//
//	glMatrixMode (GL_PROJECTION);	// Switch to the projection matrix.
//	glLoadIdentity ();				// Clean up the projection matrix.
//
//	glMatrixMode (GL_MODELVIEW);	// Switch to the modelview matrix.
//	glLoadIdentity ();				// Clean up the modelview matrix.
//
//	glEnable( GL_TEXTURE_2D );
//	
//	for(std::size_t i = 0; i < guiObjects.size(); i++)
//		guiObjects[i]->draw(fonts[0], logic);
//
//	glFlush();
//
//	glDisable( GL_TEXTURE_2D );
//
//    glPopMatrix();					// Restore the previous state.
//}

//void OptionsScreen::doInput(Logic &logic, SDL_Event sdlEvent)
//{
//    switch(sdlEvent.type)
//    {
//	case SDL_MOUSEBUTTONDOWN:
//		bLeftMouseButton = true;	// Prevent button captions from flickering.
//		break;
//
//	case SDL_MOUSEBUTTONUP:
//		if(bLeftMouseButton){			
//			handleMouseButtonUp(sdlEvent, logic);
//			bLeftMouseButton = false;
//		}
//		break;
//
//	case SDL_KEYDOWN:
//		handleKeyDown(sdlEvent, logic);
//		break;
//
//	case SDL_QUIT:
//		logic.bAppRunning = false;
//		break;
//	}
//}

// !Later: add button manipulation here.
//void OptionsScreen::handleKeyDown(const SDL_Event& sdle, Logic &logic)
//{	
//	// Check keyboard.
// 	switch(sdle.key.keysym.sym)
//	{
//	case SDLK_h:
//		break;
//	case SDLK_k:
//		break;
//	}
//}

//void OptionsScreen::handleMouseButtonUp(const SDL_Event& sdle, Logic &logic)
//{
//	switch(sdle.button.button)
//	{
//	case SDL_BUTTON_LEFT:
//		{
//			// Normalize screen coordinates.
//			// OpenGL coords (used for rendering) go from -1 to 1 with the origin in the middle.
//			// While clicking coords have (0, 0) in the upper left corner.
//			float xnorm = (sdle.button.x - 0.5*flWidth) / (0.5*flWidth);
//			float ynorm = (0.5*flHeight - sdle.button.y) / (0.5*flHeight);
//			
//			for(std::size_t i = 0; i < guiObjects.size(); i++){
//				guiObjects[i]->handleMouseButtonUp(logic, xnorm, ynorm);
//			}
//		}
//		break;
//	}// end switch
//}

/*________________________________*/
// HowtoScreen implementation.
HowtoScreen::HowtoScreen(float w, float h, SDL_Surface* s, 
	TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
	FMOD::System *sys, std::vector<FMOD::Sound*> snd) :
	ButtonScreen(w, h, s, t, fnt, sys, snd)
{
	//addButtons();
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

	glEnable( GL_TEXTURE_2D );
	
	//for(std::size_t i = 0; i < guiObjects.size(); i++)
		//guiObjects[i]->draw(fonts[0], logic);
	std::string txt = "Keys:";
	drawText(txt, -0.75, 0.75, 0.25, 0.125, fonts[0], logic);

	txt = "B - go back to the start screen";
	drawText(txt, -0.75, 0.5, 0.5, 0.125, fonts[0], logic);

	txt = "v/space - pause/unpause";
	drawText(txt, -0.75, 0.25, 0.5, 0.125, fonts[0], logic);

	txt = "arrows/asdw - paddle manipulation";
	drawText(txt, -0.75, 0.0, 0.5, 0.125, fonts[0], logic);

	txt = "= - reset view to the initial position";
	drawText(txt, -0.75, -0.25, 0.5, 0.125, fonts[0], logic);

	txt = "Click or press space to go back.";
	drawText(txt, -0.75, -0.5, 0.75, 0.25, fonts[0], logic);

	// Add: zoom m/p, rotate around z with wheel.

	txt = "(c) Nikolay Prodanov, 2013. All rights reserved.";
	drawText(txt, -0.75, -0.75, 0.75, 0.25, fonts[0], logic);

	glFlush();

	glDisable( GL_TEXTURE_2D );

    glPopMatrix();					// Restore the previous state.
}

//void HowtoScreen::doInput(Logic &logic, SDL_Event sdlEvent)
//{
//    switch(sdlEvent.type)
//    {
//	case SDL_MOUSEBUTTONDOWN:
//		bLeftMouseButton = true;	// Prevent button captions from flickering.
//		break;
//
//	case SDL_MOUSEBUTTONUP:
//		if(bLeftMouseButton){
//			bLeftMouseButton = false;
//			handleMouseButtonUp(sdlEvent, logic);	
//		}
//		break;
//
//	case SDL_KEYDOWN:
//		handleKeyDown(sdlEvent, logic);
//		break;
//
//	//case SDL_QUIT:
//		//logic.bAppRunning = false;
//		//break;
//	}
//}

// !Later: add button manipulation here.
void HowtoScreen::handleKeyDown(const SDL_Event& sdle, Logic &logic)
{	
	// Check keyboard.
 	switch(sdle.key.keysym.sym)
	{
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
	}// end switch
}

/*________________________________*/
// PlayScreen implementation.
PlayScreen::PlayScreen(float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt, 
	 FMOD::System *sys,std::vector<FMOD::Sound*> snd, int nofshapes,
	 std::vector<std::tr1::shared_ptr<RoundParameters> > & rp) :
	 SdlScreen(w, h, s, t, fnt, sys, snd), iNumOfShapes(nofshapes), roundParams(rp)
{
	//initMembers();
	//addShapes();
}

PlayScreen::~PlayScreen()
{
}

void PlayScreen::initMembers(const Logic &logic)
{
	iCurRound = logic.iRound - 1;
	flBoxWidth = roundParams[iCurRound]->flBoxWidth ;//1.3f;		// Add these to the constructor and use when init walls.
	flBoxHeight = roundParams[iCurRound]->flBoxHeight;//1.0f;
	flBoxThickness = 0.5*roundParams[iCurRound]->flBoxHeight;//0.5f;
	flPaddleRadius = roundParams[iCurRound]->flPaddleRadius;//0.05*flBoxWidth;
	flBallVel = roundParams[iCurRound]->flBallVelocity;
	flBallDeltaVel = roundParams[iCurRound]->flBallDeltaVel;
	flCompPaddleVel = roundParams[iCurRound]->flComputerPaddleVel;

	xViewOld = 0.; //xView = 0.;
	angleViewY = 115.;		// Initial angle (around y) of the view.
	angleViewZ = 0.;
	flScaleAll = 1.;
	bPaddlePicked = false;
	xPaddleOld = yPaddleOld = 0.;
	flZaxisDistance = 2.2f;
	flLengthUnit = 0.4f;
}

void PlayScreen::unregisterObservers(Logic &logic)
{
	typedef std::map<std::size_t, std::tr1::shared_ptr<Shape> >::iterator map_iter;
	
	for(map_iter iterator = shapes.begin(); iterator != shapes.end(); iterator++)
		logic.unregisterObserver();
}

void PlayScreen::registerObservers(Logic &logic)
{
	typedef std::map<std::size_t, std::tr1::shared_ptr<Shape> >::iterator map_iter;
	
	for(map_iter iterator = shapes.begin(); iterator != shapes.end(); iterator++)
		logic.registerObserver( dynamic_cast<Observer*>(iterator->second.get()) );
}

void PlayScreen::addShapes(Logic &logic)
{
	// Cleanup the shapes from the previous round.
	//unregisterObservers(logic);
	shapes.clear();// .resize(0);
	// Add shapes to the game.
	//shapes.resize(iNumOfShapes);	// ball + 6 walls + 2 paddles

	// Add ball.
	vector_3d ambient = vector_3d(0.0, 0.5, 0.0);
	vector_3d diffuse = vector_3d(0.0, 1.0, 0.0);
	vector_3d specular = vector_3d(0.0, 0.0, 0.0);
	float alpha = 1.0;				// Opaque ball.
	float shine = 0.;
	vector_3d center(0.0f, 0.0f, 0.0f);				// Ball starts at the center of the scene.
	vector_3d velocity(0.4*flBallVel, 0.2*flBallVel, 0.2*flBallVel);// !ADD Random initial velocity.
	Shape* pShape = new Ball(BALL, center, 0.05*flBoxHeight, velocity, flBallDeltaVel,
		ambient, diffuse, specular, shine, alpha);
	//Shape* pShape = dynamic_cast<Shape*>(pBall);
	//shapes[0] = std::tr1::shared_ptr<Shape>(pShape);
	shapes.insert(std::make_pair(BALL, std::tr1::shared_ptr<Shape>(pShape)));	
	
	// !Reconsider this code: add some method to avoid code duplication.

	// Add walls.
	// Left wall.
	center = vector_3d(-0.5*flBoxWidth, 0., 0.);
	vector_3d n = vector_3d(-1., 0., 0.);
	pShape = new AbsorbingWall( WALL, center, flBoxThickness, flBoxHeight, n);
	//pShape = dynamic_cast<Shape*>(pWall);
	pShape->setSound(system, sounds[2]);
	//shapes[1] = std::tr1::shared_ptr<Shape>(pShape);
	shapes.insert(std::make_pair(shapes.size(), std::tr1::shared_ptr<Shape>(pShape)));

	// Right wall.
	center = vector_3d(0.5*flBoxWidth, 0., 0.);
	n = vector_3d(1., 0., 0.);
	if(!logic.bTrain){
		pShape = new AbsorbingWall( WALL, center, flBoxThickness, flBoxHeight, n);
		pShape->setSound(system, sounds[2]);
	}
	else{
		pShape = new Wall( WALL, center, flBoxThickness, flBoxHeight, n);
		pShape->setSound(system, sounds[1]);
	}
	//pShape = dynamic_cast<Shape*>(pShape);
	
	//shapes[2] = std::tr1::shared_ptr<Shape>(pShape);
	shapes.insert(std::make_pair(shapes.size(), std::tr1::shared_ptr<Shape>(pShape)));

	// Top wall.
	ambient = vector_3d(0.0, 0.0, 0.5);
	diffuse = vector_3d(0.0, 0.0, 1.0);
	specular = vector_3d(0.0, 0.0, 0.2);
	alpha = 0.1;	// A bit transparent wall.
	shine = 20.;
	center = vector_3d(0., 0.5*flBoxHeight, 0.);
	n = vector_3d(0., 1., 0.);
	pShape = new Wall( WALL, center, flBoxWidth, flBoxThickness, n,
		ambient, diffuse, specular, shine, alpha);
	//pShape = dynamic_cast<Shape*>(pWall);
	pShape->setSound(system, sounds[1]);
	//shapes[3] = std::tr1::shared_ptr<Shape>(pShape);
	shapes.insert(std::make_pair(shapes.size(), std::tr1::shared_ptr<Shape>(pShape)));

	// Bottom wall.
	ambient = vector_3d(0.0, 0.0, 0.5);
	diffuse = vector_3d(0.0, 0.0, 1.0);
	specular = vector_3d(0.0, 0.0, 0.2);
	alpha = 0.1;	// Abit transparent wall.
	shine = 20.;
	center = vector_3d(0., -0.5*flBoxHeight, 0.);
	n = vector_3d(0., -1., 0.);
	pShape = new Wall( WALL, center, flBoxWidth, flBoxThickness, n,
		ambient, diffuse, specular, shine, alpha);
	//pShape = dynamic_cast<Shape*>(pWall);
	pShape->setSound(system, sounds[1]);
	//shapes[4] = std::tr1::shared_ptr<Shape>(pShape);
	shapes.insert(std::make_pair(shapes.size(), std::tr1::shared_ptr<Shape>(pShape)));

	// Front wall.
	center = vector_3d(0., 0., 0.5*flBoxThickness);
	n = vector_3d(0., 0., 1.);
	pShape = new Wall( WALL, center, flBoxWidth, flBoxHeight, n);
	//pShape = dynamic_cast<Shape*>(pWall);
	pShape->setSound(system, sounds[1]);
	//shapes[5] = std::tr1::shared_ptr<Shape>(pShape);
	shapes.insert(std::make_pair(shapes.size(), std::tr1::shared_ptr<Shape>(pShape)));

	// Back wall.
	center = vector_3d(0., 0., -0.5*flBoxThickness);
	n = vector_3d(0., 0., -1.);
	pShape = new Wall( WALL, center, flBoxWidth, flBoxHeight, n);
	//pShape = dynamic_cast<Shape*>(pWall);
	pShape->setSound(system, sounds[1]);
	//shapes[6] = std::tr1::shared_ptr<Shape>(pShape);
	shapes.insert(std::make_pair(shapes.size(), std::tr1::shared_ptr<Shape>(pShape)));

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
	pShape = new Paddle( LEFT_PADDLE, center, n, flPaddleRadius, 0.01*flBoxWidth, 
		angle, 0.5*flBoxHeight, 0.5*flBoxThickness,
		ambient, diffuse, specular, shine, alpha);
	//pShape = dynamic_cast<Shape*>(pPaddle);
	pShape->setSound(system, sounds[3]);
	leftPaddleIdx = 7;
	//shapes[leftPaddleIdx] = std::tr1::shared_ptr<Shape>(pShape);
	shapes.insert(std::make_pair(shapes.size(), std::tr1::shared_ptr<Shape>(pShape)));

	// Computer (right) paddle.
	if(!logic.bTrain){
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
			ambient, diffuse, specular, shine, alpha);
		//pShape = dynamic_cast<Shape*>(pPaddle);
		pShape->setSound(system, sounds[3]);
		rightPaddleIdx = 8;
		//shapes[rightPaddleIdx] = std::tr1::shared_ptr<Shape>(pShape);
		shapes.insert(std::make_pair(shapes.size(), std::tr1::shared_ptr<Shape>(pShape)));
	}

	// Register the shapes as observers to handle sound.
	//registerObservers(logic);
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

void PlayScreen::doDrawing(Logic &logic)
{
	initResize();
	initView();
	
	// Draw all the shapes.
	for(std::size_t i = 0; i < shapes.size(); i++){
		shapes[i]->draw();
	}

	glFlush();
}

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
		angleViewZ -= 0.3;			// Rotate around the horizontal axis.
		break;

	case SDL_BUTTON_WHEELDOWN:
		angleViewZ += 0.3;			// !!Replace this magic number by a var.
		break;
	}// end switch
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
			angleViewY += sdle.motion.x - xViewOld;
			xViewOld = sdle.motion.x;
		}
		else{
			vector_3d dr(0., -0.0025*(sdle.motion.y - yPaddleOld), 
				0.0025*(sdle.motion.x - xPaddleOld) );
			shapes[leftPaddleIdx]->move(0., dr, false);
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
	case SDLK_EQUALS:		// Reset view.
		angleViewY = 115;	// !Reconsider magic numbers!
		angleViewZ = 0.;
		flScaleAll = 1.;
		break;

	case SDLK_b:
		if(!logic.bShowStartScreen)
		{
			logic.bShowStartScreen = true;
			//logic.bGamePaused = true;
			logic.bNewRound = true;		// Start from scratch.
		}
		logic.notifyObservers();	// Tell observers to change their sound behavior.
		break;

	case SDLK_p:	 //Scale.
		flScaleAll += 0.01;			// !Reconsider magic numbers!
		break;

	case SDLK_m:
		flScaleAll -= 0.01;
		break;

	case SDLK_v:
		logic.bGamePaused = true;
		logic.bNewRound = false;		// May be redundant! Check later!
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
		dr = vector_3d(0., 0.0, 0.01);		// Reconsider magic number!
		shapes[leftPaddleIdx]->move(0., dr, false);
		break;

	case SDLK_d:			// Fall through.
	case SDLK_RIGHT:
		dr = vector_3d(0., 0.0, -0.01);
		shapes[leftPaddleIdx]->move(0., dr, false);
		break;

	case SDLK_w:			// Fall through.
	case SDLK_UP:
		dr = vector_3d(0., 0.01, 0.);
		shapes[leftPaddleIdx]->move(0., dr, false);
		break;

	case SDLK_s:			// Fall through.
	case SDLK_DOWN:
		dr = vector_3d(0., -0.01, 0.);
		shapes[leftPaddleIdx]->move(0., dr, false);
		break;
	}
}

void PlayScreen::handleResize(const SDL_Event& sdle, Logic &logic)
{
	SDL_SetVideoMode( sdlEvent.resize.w, sdlEvent.resize.h, 32, SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE );
	glViewport (0, 0, (GLsizei) sdle.resize.w, (GLsizei) sdle.resize.h);
	flWidth = sdle.resize.w;
	flHeight = sdle.resize.h;

	initResize();		// Avoid code duplication with setup matrices.
}

// Object selection using back buffer.
int PlayScreen::pickObject(int x, int y)
{
	// Render the scene into the back buffer with colors corresponding to
	// names of the objects.
	initView();		// Avoid code duplication with doDrawing().

	glDisable(GL_LIGHTING);
	// Draw all the shapes into the double buffer.
	for(std::size_t i = 0; i < shapes.size(); i++)
	{
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffer for new data.
	glLoadIdentity ();             // Clear the current matrix.
	// Move camera to the point (0,0,5) in eye coords, look at point (0,0,0), 
	// camera orientation - the normal is along (0,1,0)
	gluLookAt (0.0, 0.0, flZaxisDistance, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	glRotatef(angleViewY, 0.0f, 1.0f, 0.0f);
	glRotatef(angleViewZ, 0.0f, 0.0f, 1.0f);
	glScalef(flScaleAll, flScaleAll,flScaleAll);

	drawAxes();	
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

void PlayScreen::doLogic(const Logic &logic)
{
	double deltaTime = 0.;		// Stub, ss not really used.
    
    // If the player hasn't lost, do stuff.
    if(!logic.bGameOver)
    {		
		// Move the shapes.
		bool bReset = false;	// Whether to move the ball to the origin.
		for(std::size_t i = 0; i < shapes.size(); i++)
			shapes[i]->move(deltaTime, vector_3d(0., 0., 0.), bReset);

		// Detect collisions for all shapes with the ball.
		bool bCollided = false;
		for(std::size_t i = 0; i < shapes.size(); i++){
			bCollided = shapes[i]->collide(shapes[0].get()); 
			if(bCollided && bPlaySound)
				shapes[i]->playSound();
		}
    } // if(!game_over)
    
    // Update other general logic
    // theta += time_delta * 0.01;
    // secondary timer for move_down input so you cant drop a brick too fast!
    //if(secondary_timer > 0)
        //secondary_timer -= time_delta * 0.1;
        
    // update scoring glow
    //if(score_glow > GLOW_MIN)
        //score_glow -= time_delta * 0.001;
    //if(score_glow < GLOW_MIN)
        //score_glow = GLOW_MIN;
        
    // update other stuff
    //updateStarRank();
    //updateSkillLevel();
    
    // if the player lost, do game over effects
    //if(game_over){
        //updateGameOver();
    //}   
}

void PlayScreen::play(Logic &logic, SDL_Event sdlEvent)
{
	if(logic.bNewRound){
		logic.bNewRound = false;
		setupNewRound(logic);
	}

	doInput(logic, sdlEvent);
	
	if(!logic.bGamePaused)
		doLogic(logic);
	
	doDrawing(logic);	
}

//std::map<std::size_t, std::tr1::shared_ptr<Shape> > & PlayScreen::getShapes()
//{
//	typedef std::map<std::size_t, std::tr1::shared_ptr<Shape> >::iterator map_iter;
//	//std::map<std::size_t, std::tr1::shared_ptr<Shape> > shapes = playScreen->getShapes();
//	for(map_iter iterator = shapes.begin(); iterator != shapes.end(); iterator++)
//		//logic.registerObserver( dynamic_cast<Observer*>(iterator->second.get()) );
//		iterator->second.get();
//
//	return shapes;
//}

void PlayScreen::setupNewRound(Logic &logic)
{
	initMembers(logic);
	addShapes(logic);
}

// Observer pattern method.
void PlayScreen::notify(Subject* s) 
{
	bPlaySound = ((Logic*) s)->bActionsSound;
}