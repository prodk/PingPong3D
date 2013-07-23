// GuiObject.cpp - implementation of the GuiObject class and all its child classes.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#include "GuiObject.h"

/*________________________________*/
// GuiObject class implementation.
GuiObject::GuiObject(float xExt, float yExt, float wExt, float hExt,
	std::size_t idExt, std::string n) :
	x(xExt), y(yExt), w(wExt), h(hExt), id(idExt), name(n)
{
	bIsPressed = false;
	//bPlaySound = true;
}

GuiObject::~GuiObject(void)
{
}


std::size_t GuiObject::getId() const
{
	return id;
}

//FMOD_RESULT GuiObject::setSound(FMOD::System *sys, FMOD::Sound *snd)
//{
//	if(sys == NULL) return FMOD_ERR_FILE_BAD;
//	if(snd == NULL) return FMOD_ERR_FILE_BAD;
//	system = sys;
//	sound = snd;
//
//	// Start paused sound.
//	//FMOD_RESULT result = system->playSound(sound, 0, true, &channel);
//
//	return FMOD_OK;
//}

//void GuiObject::notify(Subject* s) 
//{
//	bPlaySound = ((Logic*) s)->bActionsSound;
//}

int GuiObject::drawText(const std::string &txt, GLfloat x, GLfloat y, GLfloat w, GLfloat h,
	TTF_Font *font)
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
	SDL_Delay(4);			// Small delay to prevent from full CPU load.

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

bool GuiObject::isPressed(float ptx, float pty)
{
	if( ptInRect(ptx, pty) )
		return true;
	
	return false;
}

bool GuiObject::isPressed()
{
	if(bIsPressed)
		return true;
	
	return false;
}

void GuiObject::setPressed(bool pressed)
{
	bIsPressed = pressed;
}

bool GuiObject::ptInRect(float ptx, float pty)
{
	float xMin = 0.;
	float xMax = xMin + w;
	float yMin = 0.;
	float yMax = yMin + h;

	if( (ptx >= x) && (ptx <= x + w) && (pty >= y ) && (pty <= y+h) )
		return true;

	return false;
}

/*________________________________*/
//Button class implementation.
Button::Button(float xExt, float yExt, float wExt, float hExt, 
	std::size_t idExt, std::string n, int tid) :
GuiObject(xExt, yExt, wExt, hExt, idExt, n), textureId(tid)
{
	//caption = name;	
}

Button::~Button()
{
}

void Button::drawPressed(TTF_Font *font)
{		
	glPushMatrix();
	glDisable(GL_LIGHTING);

	glBindTexture( GL_TEXTURE_2D, textureId );
	
	glColor4f(1.0, 0., 0., 1.0);
	glTranslatef(x, y, 0.);

	glBegin( GL_QUADS );
	glTexCoord3d(0.0, 1.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord3d(1.0, 1.0, 0.0); glVertex3d(w, 0.0, 0.0);
	glTexCoord3d(1.0, 0.0, 0.0); glVertex3d(w, h, 0.0);
	glTexCoord3d(0.0, 0.0, 0.0); glVertex3d(0.0, h, 0.0);	
	glEnd();	

	glEnable(GL_LIGHTING);
	glPopMatrix();
	
	// Print button's text.
	SDL_Delay(4);				// Additional delay because we draw only one text.
	drawText(name, x, y, w, h, font);
}

void Button::drawUnpressed(TTF_Font *font)
{		
	glPushMatrix();
	glDisable(GL_LIGHTING);

	glBindTexture( GL_TEXTURE_2D, textureId );
	
	glColor4f(1.0, 0., 0., 1.0);
	glTranslatef(x, y, 0.);

	glBegin( GL_QUADS );
	glTexCoord3d(0.0, 1.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord3d(1.0, 1.0, 0.0); glVertex3d(w, 0.0, 0.0);
	glTexCoord3d(1.0, 0.0, 0.0); glVertex3d(w, h, 0.0);
	glTexCoord3d(0.0, 0.0, 0.0); glVertex3d(0.0, h, 0.0);	
	glEnd();	

	glEnable(GL_LIGHTING);
	glPopMatrix();
	
	// Print button's text.
	SDL_Delay(4);				// Additional delay because we draw only one text.
	drawText(name, x, y, w, h, font);
}


//void Button::handleMouseButtonUp(Logic &logic, float x, float y)
//{
//	if( ptInRect(x, y) )
//	{		
//		switch(id)
//		{ 
//		case START_BTN:
//			logic.bShowStartScreen = false;
//			logic.bShowOptionsScreen = false;
//			logic.bShowPlayScreen = true;
//			logic.bGamePaused = false;
//			logic.bTrain = false;
//			logic.notifyObservers(); // Tell registered observers to change their settings.
//			break;
//
//		case OPTIONS_BTN:
//			logic.bShowStartScreen = false;
//			logic.bShowOptionsScreen = true;
//			logic.bShowPlayScreen = false;
//			//logic.notifyObservers();
//			break;
//
//		case HOWTO_BTN:
//			logic.bShowStartScreen = false;
//			logic.bShowOptionsScreen = false;
//			logic.bShowHowtoScreen = true;			
//			//logic.notifyObservers(); // Tell registered observers to change their settings.
//			break;
//
//		case TRAIN_BTN:
//			logic.bShowStartScreen = false;
//			logic.bShowOptionsScreen = false;
//			logic.bShowPlayScreen = true;
//			logic.bGamePaused = false;
//			logic.bTrain = true;
//			logic.notifyObservers();
//			break;
//		}// end switch(id)
//
//		if(bPlaySound)
//			::playSound(system, sound, channel);
//	}// End if ptInRect.
//}

//void Button::handleMouseButtonDown(Logic &logic, float x, float y)
//{
//	if( ptInRect(x, y) )
//	{		
//	}// End if ptInRect.
//}

/*________________________________*/
// OptionsButton class implementation.
OptionsButton::OptionsButton(float xExt, float yExt, float wExt, float hExt, 
	std::size_t idExt, std::string n, int tid, std::string cap) :
Button(xExt, yExt, wExt, hExt, idExt, n, tid), caption(cap)
{
	xSmall = x + 1.7*w;
	wSmall = 0.25*w;
	//long double str;	
}

OptionsButton::~OptionsButton()
{
}

void OptionsButton::drawPressed(TTF_Font *font)
{	
	// Maybe just call Button::drawPressed here instead.
	glPushMatrix();
	glDisable(GL_LIGHTING);

	glBindTexture( GL_TEXTURE_2D, textureId );
	
	glColor4f(1.0, 0., 0., 1.0);
	glTranslatef(x, y, 0.);

	// Draw larger button.
	glBegin( GL_QUADS );
	glTexCoord3d(0.0, 1.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord3d(1.0, 1.0, 0.0); glVertex3d(w, 0.0, 0.0);
	glTexCoord3d(1.0, 0.0, 0.0); glVertex3d(w, h, 0.0);
	glTexCoord3d(0.0, 0.0, 0.0); glVertex3d(0.0, h, 0.0);	
	glEnd();	

	glTranslatef(xSmall, 0., 0.);
	// Draw smaller button.
	glBegin( GL_QUADS );
	glTexCoord3d(0.0, 1.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord3d(1.0, 1.0, 0.0); glVertex3d(wSmall, 0.0, 0.0);
	glTexCoord3d(1.0, 0.0, 0.0); glVertex3d(wSmall, h, 0.0);
	glTexCoord3d(0.0, 0.0, 0.0); glVertex3d(0.0, h, 0.0);	
	glEnd();

	glEnable(GL_LIGHTING);
	glPopMatrix();

	drawText(name, x, y, w, h, font);				// Button's name.
	drawText(caption, x+xSmall, y, wSmall, h, font);	// Parameter's value.
}

void OptionsButton::drawUnpressed(TTF_Font *font)
{		
	glPushMatrix();
	glDisable(GL_LIGHTING);

	glBindTexture( GL_TEXTURE_2D, textureId );
	
	glColor4f(1.0, 0., 0., 1.0);
	glTranslatef(x, y, 0.);

	// Draw larger button.
	glBegin( GL_QUADS );
	glTexCoord3d(0.0, 1.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord3d(1.0, 1.0, 0.0); glVertex3d(w, 0.0, 0.0);
	glTexCoord3d(1.0, 0.0, 0.0); glVertex3d(w, h, 0.0);
	glTexCoord3d(0.0, 0.0, 0.0); glVertex3d(0.0, h, 0.0);	
	glEnd();	

	glTranslatef(xSmall, 0., 0.);
	// Draw smaller button.
	glBegin( GL_QUADS );
	glTexCoord3d(0.0, 1.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord3d(1.0, 1.0, 0.0); glVertex3d(wSmall, 0.0, 0.0);
	glTexCoord3d(1.0, 0.0, 0.0); glVertex3d(wSmall, h, 0.0);
	glTexCoord3d(0.0, 0.0, 0.0); glVertex3d(0.0, h, 0.0);	
	glEnd();

	glEnable(GL_LIGHTING);
	glPopMatrix();

	drawText(name, x, y, w, h, font);				// Button's name.
	drawText(caption, x+xSmall, y, wSmall, h, font);	// Parameter's value.
}

//void OptionsButton::handleMouseButtonUp(Logic &logic, float x, float y)
//{
//	if( ptInRect(x, y) )
//	{		
//		switch(id)
//		{		
//		case BACKGRSND_BTN:
//			if(logic.bBackgroundSound){
//				logic.bBackgroundSound = false;
//				caption = "Off";
//			}
//			else{
//				logic.bBackgroundSound = true;
//				caption = "On";
//			}
//			logic.notifyObservers(); // Tell registered observers to change their settings.
//			break;
//
//		case ACTIONSND_BTN:
//			//logic.bActionsSound = !logic.bActionsSound;
//			if(logic.bActionsSound){
//				logic.bActionsSound = false;
//				caption = "Off";
//			}
//			else{
//				logic.bActionsSound = true;
//				caption = "On";
//			}
//			logic.notifyObservers(); // Tell registered observers to change their settings.
//			break;
//
//		case ROUND_BTN:
//			logic.iRound = logic.iRound % logic.iRoundMax + 1;
//			caption = std::to_string((_ULonglong)logic.iRound);
//			logic.notifyObservers(); 
//			break;
//
//		case BACK_BTN:
//			logic.bShowOptionsScreen = false;
//			logic.bShowStartScreen = true;
//			break;
//		}// end switch(id)
//
//		if(bPlaySound)
//			::playSound(system, sound, channel);
//	}// End if ptInRect.
//}

void OptionsButton::setCaption(const std::string & c)
{
	caption = c;
}

//bool OptionsButton::ptInRect(float ptx, float pty)
//{
//	float xMin = 0.;
//	float xMax = xMin + w;
//	float yMin = 0.;
//	float yMax = yMin + h;
//
//	if( (ptx >= x) && (ptx <= x + w) && (pty >= y ) && (pty <= y+h) )
//		return true;
//
//	return false;
//}